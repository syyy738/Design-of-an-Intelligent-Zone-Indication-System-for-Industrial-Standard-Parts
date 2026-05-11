#include "usart2.h"
#include "usart.h"
#include "delay.h"
#include "timer.h"

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdarg.h"

/*********************************************************************************
 * @Function	:  ״̬����
 **********************************************************************************/
#if UART2_RXBUF_SIZE

u8 USART2_RX_BUF[UART2_RXBUF_SIZE]; // ���ջ���,���USART3_MAX_RECV_LEN���ֽ�.
u8 USART2_TX_BUF[UART2_RXBUF_SIZE]; // ���ͻ���,���USART3_MAX_SEND_LEN�ֽ�

U16 Uart2_RecvCount = 0;   // ���ڽ��ռ���
U8 Uart2_EnterRecvFlg = 0; // ���������ݽ��ձ�ʶ

extern U16 Usart2_Time; // ���������ݽ��ձ�ʶ
extern U8 bTimeout10ms; /** < 10���붨ʱ����ʱ��־ */

/*********************************************************************************
 * @Function	:  ����Ƕ�������жϿ�����NVIC
 **********************************************************************************/
static void NVIC_Configuration(U8 PreemptionPriority, U8 SubPriority)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Ƕ�������жϿ�������ѡ�� */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = PreemptionPriority;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = SubPriority;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
/*********************************************************************************
 * @Function	:  USART GPIO ����,������������
 **********************************************************************************/
void Usart2_Init(U32 bound, U8 PreemptionPriority, U8 SubPriority)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	// �򿪴���GPIO��ʱ��
	USART2_GPIO_APBxClkCmd(USART2_GPIO_CLK, ENABLE);
	// �򿪴��������ʱ��
	USART2_APBxClkCmd(USART2_CLK, ENABLE);
	// ��USART Tx��GPIO����Ϊ���츴��ģʽ
	GPIO_InitStructure.GPIO_Pin = USART2_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(USART2_TX_GPIO_PORT, &GPIO_InitStructure);
	// ��USART Rx��GPIO����Ϊ��������ģʽ
	GPIO_InitStructure.GPIO_Pin = USART2_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(USART2_RX_GPIO_PORT, &GPIO_InitStructure);

	// ���ô��ڵĹ�������
	// ���ò�����
	USART_InitStructure.USART_BaudRate = bound;
	// ���� �������ֳ�
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	// ����ֹͣλ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	// ����У��λ
	USART_InitStructure.USART_Parity = USART_Parity_No;
	// ����Ӳ��������
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	// ���ù���ģʽ���շ�һ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	// ��ɴ��ڵĳ�ʼ������
	USART_Init(USART_2, &USART_InitStructure);

	// �����ж����ȼ�����
	NVIC_Configuration(PreemptionPriority, SubPriority);
	// ʹ�ܴ��ڽ����ж�
	USART_ITConfig(USART_2, USART_IT_RXNE, ENABLE);
	// ʹ�ܴ���
	USART_Cmd(USART_2, ENABLE);
	// ���������ɱ�־
	// USART_ClearFlag(USART1, USART_FLAG_TC);
}
/*********************************************************************************
 * @Function	:  Usart4 ��һЩ��������,�������ջ�������,��־λ��
 **********************************************************************************/
void Uart2_funtction(Usart2_Funtctions CMD)
{
	switch (CMD)
	{
	case PRINTF2: // ������յ��Ĵ�����Ϣ
		printf("uart2 recv: %d, %s\r\n", Uart2_RecvCount, USART2_RX_BUF);
		break;
	case CLEAN2: // ��մ�����Ϣ��״̬
		memset(USART2_RX_BUF, 0, UART2_RXBUF_SIZE);
		Uart2_RecvCount = 0;
		Uart2_EnterRecvFlg = 0;
#if GENERAL_TIM4
		Usart2_Time = 0;
#endif
		break;
	case OTHER2:
		break;
	default:
		break;
	}
}

/*********************************************************************************
 * @Function	:  յڶ
 **********************************************************************************/
void Send_Usart2(void)
{
	// 检测是否接收到数据（通过超时判断一帧数据接收完成）
	// 修改条件：Usart2_Time > 5 或者 bTimeout10ms 为真
	if (Usart2_Time > 5 || bTimeout10ms == MY_TRUE)
	{
		Uart2_EnterRecvFlg = 1;
	}
	
	if (Uart2_EnterRecvFlg)
	{
		// 打印接收到的数据用于调试
		printf("uart2 recv: %d, %s\r\n", Uart2_RecvCount, USART2_RX_BUF);
		
		// 解析 K230 发送的数据
		if (Uart2_RecvCount > 0)
		{
			massage_k210((char *)USART2_RX_BUF);
		}
		
		// 清空缓冲区
		Uart2_funtction(CLEAN2);
	}
}
}
// ȷ��һ�η������ݲ�����USART3_MAX_SEND_LEN�ֽ�
void u2_printf(char *fmt, ...)
{
	u16 i, j;
	va_list ap;
	va_start(ap, fmt);
	vsprintf((char *)USART2_TX_BUF, fmt, ap);
	va_end(ap);
	i = strlen((const char *)USART2_TX_BUF); // �˴η������ݵĳ���
	for (j = 0; j < i; j++)					 // ѭ����������
	{
		while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
			; // ѭ������,ֱ���������
		USART_SendData(USART2, USART2_TX_BUF[j]);
	}
}

/*********************************************************************************
 * @Function	:  ����3�շ��ж�
 * @Input		:  None
 * @Output		:  None
 * @Return		:  None
 * @Others		:  None
 * @Date			:  2022-07-30
 **********************************************************************************/

void USART2_IRQHandler(void)
{
	uint8_t ucCh;
	if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		ucCh = USART_ReceiveData(USART2);
		if (Uart2_RecvCount < (UART2_RXBUF_SIZE - 1))
		{
			// Ԥ��1���ֽ�д������
			USART2_RX_BUF[Uart2_RecvCount++] = ucCh;
			// printf("%s\n:",USART2_RX_BUF);
		}
#if GENERAL_TIM4
		Usart2_Time = 1;
		bTimeout10ms = MY_TRUE; /** < 10���붨ʱ����ʱ��־ */
#endif
		USART_ClearFlag(USART2, USART_FLAG_RXNE);
		ucCh = 0;
	}
}
#endif
