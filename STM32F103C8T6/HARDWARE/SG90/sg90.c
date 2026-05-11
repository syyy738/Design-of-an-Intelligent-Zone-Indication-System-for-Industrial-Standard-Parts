#include "sg90.h"
#include "delay.h"
/*********************************************************************************
 * @Function	: TIM3 PWM输出初始化
 * @Input		: F :频率（HZ）
 * @Output		: None
 * @Return		: None
 * @Others		: None
 * @Date		: 2022-11-2
 **********************************************************************************/

// SG90舵机控制
/*
  20ms脉冲
  5-14  正转，值越小，转的越快
  16-25 反转，值越大，转的越快
*/

void TIM3_PWM_Init(U32 f)
{

  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
  RCC_APB2PeriphClockCmd(TIM3_GPIO_CLK_TIME1_2, ENABLE);

  GPIO_InitStructure.GPIO_Pin = TIM3_GPIO_PIN1|TIM3_GPIO_PIN2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // 复用推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(TIM3_GPIO_PORT_TIME1_2, &GPIO_InitStructure);



  TIM_DeInit(TIM3); // 定时器1初始化
  /*--------------------时基结构体初始化-------------------------*/
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  // 自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断
  TIM_TimeBaseStructure.TIM_Period = 200 - 1;
  // 驱动CNT计数器的时钟 = Fck_int/(psc+1)
  TIM_TimeBaseStructure.TIM_Prescaler = 7200;
  // 时钟分频因子 ，配置死区时间时需要用到
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  // 计数器计数模式，设置为向上计数
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  // 重复计数器的值，没用到不用管
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
  // 初始化定时器
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
  /*--------------------输出比较结构体初始化-------------------*/
  TIM_OCInitTypeDef TIM_OCInitStructure;
  // 配置为PWM模式1
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
  // 输出使能
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  // 空闲时输出电平为低
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
  // 输出通道电平极性配置
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

  // 初始化TIM3 Channel 1 PWM
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;               // 配置为PWM2模式，TIMx_CNT>TIMx_CCR1，为有效电平
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;  // 空闲时1N也输出底电平
  TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;      // 定时器触发之后，输出低电平
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable; // 使能输出
  TIM_OC1Init(TIM3, &TIM_OCInitStructure);                        // 输出比较通道 1
  TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);

	 // 初始化TIM3 Channel 2 PWM
	 TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;               // 配置为PWM2模式，TIMx_CNT>TIMx_CCR1，为有效电平
	 TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;  // 空闲时1N也输出高电平
	 TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;      // 定时器触发之后，输出低电平
	 TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable; // 使能输出
	 TIM_OC2Init(TIM3, &TIM_OCInitStructure);                        // 输出比较通道 2
	 TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);

//	 // 初始化TIM3 Channel 3 PWM
//	 TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;               // 配置为PWM2模式，TIMx_CNT>TIMx_CCR1，为有效电平
//	 TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;  // 空闲时1N也输出底电平
//	 TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;      // 定时器触发之后，输出低电平
//	 TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable; // 使能输出
//	 TIM_OC3Init(TIM3, &TIM_OCInitStructure);                        // 输出比较通道 3
//	 TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);

//	 // 初始化TIM3 Channel 4 PWM
//	 TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;               // 配置为PWM2模式，TIMx_CNT>TIMx_CCR1，为有效电平
//	 TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;  // 空闲时1N也输出高电平
//	 TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;      // 定时器触发之后，输出低电平
//	 TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable; // 使能输出
//	 TIM_OC4Init(TIM3, &TIM_OCInitStructure);                        // 输出比较通道 4
//	 TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);

  TIM_CtrlPWMOutputs(TIM3, ENABLE);
  TIM_ARRPreloadConfig(TIM3, ENABLE); // 使能自动重装载寄存器，arr的预装载
  TIM_Cmd(TIM3, ENABLE);

	TIM3->CCR1 = 15;
	TIM3->CCR2 = 15;
//	TIM3->CCR3 = 15;
//	TIM3->CCR4 = 15;

}

