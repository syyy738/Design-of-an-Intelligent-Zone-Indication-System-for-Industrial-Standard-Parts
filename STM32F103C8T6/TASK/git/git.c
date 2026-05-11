#include "git.h"

Data_TypeDef Data_init;						  // �豸���ݽṹ��
Threshold_Value_TypeDef threshold_value_init; // �豸��ֵ���ýṹ��
Device_Satte_Typedef device_state_init;		  // �豸״̬
//DHT11_Data_TypeDef DHT11_Data;
// ��ȡ���ݲ���
mySta Read_Data(Data_TypeDef *Device_Data)
{


	return MY_SUCCESSFUL;
}
// ��ʼ��
mySta Reset_Threshole_Value(Threshold_Value_TypeDef *Value, Device_Satte_Typedef *device_state)
{

	// ��
	R_Test();
	// ״̬����
	Data_init.category = 0;
	
	return MY_SUCCESSFUL;
}
// ����OLED��ʾ��������
mySta Update_oled_massage()
{
#if OLED // �Ƿ��
	char str[50];
	if(	Data_init.category == 0){
		sprintf(str, "ʶ����: ���� ");
	}
	else if(	Data_init.category == 1){
		sprintf(str, "ʶ����: ��˿ ");
	}
	else if(	Data_init.category == 2){
		sprintf(str, "ʶ����: ��ĸ ");
	}
	else if(	Data_init.category == 3){
		sprintf(str, "ʶ����: ��� ");
	}
	OLED_ShowCH(0, 0, (unsigned char *)str);
	
	
	sprintf(str, "��˿����: %.1f  ", Data_init.wight_1);
	OLED_ShowCH(0, 2, (unsigned char *)str);
	sprintf(str, "��ĸ����: %.1f  ", Data_init.wight_2);
	OLED_ShowCH(0, 4, (unsigned char *)str);
	
	
	if(	Data_init.Error_Time == 0){
		sprintf(str, "״̬: �豸���� ");
	}
	else if(	Data_init.Error_Time == 1){
		sprintf(str, "״̬: ��˿ȱ�� ");
	}
	else if(	Data_init.Error_Time == 2){
		sprintf(str, "״̬: ��˿ȱ�� ");
	}

	OLED_ShowCH(0, 6, (unsigned char *)str);


#endif

	return MY_SUCCESSFUL;
}

// �����豸״̬
mySta Update_device_massage()
{

	// ȱ�������� 
	if( (U16) Data_init.wight_1/Data_init.ls < threshold_value_init.ls_v){
		Data_init.Error_Time = 1;
		relay1out = ~relay1out;
	}else{
		relay1out =0;
	}
	if( (U16) Data_init.wight_2/Data_init.lm  < threshold_value_init.lm_v) {
		Data_init.Error_Time = 2;
		relay2out = ~relay2out;
	}else{
		relay2out =0;
	}
	
  if( (U16) Data_init.wight_2/Data_init.lm  >= threshold_value_init.lm_v && (U16) Data_init.wight_1/Data_init.ls  > threshold_value_init.ls_v)
	{
		Data_init.Error_Time = 0;
		relay1out =0;
		relay2out =0;
		Beepout =0;
	}else{
		Beepout = ~Beepout;
	}
	
	// ���
	 if (device_state_init.door_1 == 0)  
	{
		TIM_SetCompare1(TIM3, 25);// ��
	
	}
	else 
	{
		TIM_SetCompare1(TIM3, 15);	// ��
	}
	// ���
	if (device_state_init.door_2 == 0)  
	{
		TIM_SetCompare2(TIM3, 25);	// ��
	
	}
	else 
	{
		TIM_SetCompare2(TIM3, 15);		// ��
	}
	
	// ������
	relay3out =  device_state_init.LED;
	
	return MY_SUCCESSFUL;
}

// ��ʱ��
void Automation_Close(void)
{
	
	// ��ȡ����
	Get_Weight();
	
	if (Data_init.App)
	{
		switch (Data_init.App)
		{
		case 1:
			SendMqtt(1); // �������ݵ�APP
			break;
		case 2:
			SendData(); // �������ݵ���ƽ̨
			break;
		}
		Data_init.App = 0;

	}

}
// ��ⰴ���Ƿ���
static U8 num_on = 0;
static U8 key_old = 0;
void Check_Key_ON_OFF()
{
	U8 key;
	key = KEY_Scan(1);
	// ����һ�εļ�ֵ�Ƚ� �������ȣ������м�ֵ�ı仯����ʼ��ʱ
	if (key != 0 && num_on == 0)
	{
		key_old = key;
		num_on = 1;
	}
	if (key != 0 && num_on >= 1 && num_on <= Key_Scan_Time) // 25*10ms
	{
		num_on++; // ʱ���¼��
	}
	if (key == 0 && num_on > 0 && num_on < Key_Scan_Time) // �̰�
	{
		switch (key_old)
		{
		case KEY1_PRES:
			printf("Key1_Short\n");
			// ���
			if (device_state_init.door_1 == 0)  
			{
				device_state_init.door_1 = 1;	// ��
			
			}
			else 
			{
				device_state_init.door_1 = 0;	// ��
			}
			break;
		case KEY2_PRES:
			printf("Key2_Short\n");
			// ���
			if (device_state_init.door_2 == 0)  
			{
				device_state_init.door_2 = 1;// ��
			
			}
			else 
			{
				device_state_init.door_2 = 0;	// ��
			}
			break;

		default:
			break;
		}
		num_on = 0;
	}
	else if (key == 0 && num_on >= Key_Scan_Time) // ����
	{
		switch (key_old)
		{
		case KEY1_PRES:
			printf("Key1_Long\n");
			// ������
			if (device_state_init.LED == 0)  
			{
				device_state_init.LED = 1;// ��
			
			}
			else 
			{
				device_state_init.LED = 0;	// ��
			}
			break;
		case KEY2_PRES:
			printf("Key2_Long\n");
			Get_Maopi(); // ����
			break;

		default:
			break;
		}
		num_on = 0;
	}
}
// ����json����
mySta massage_parse_json(char *message)
{

	cJSON *cjson_test = NULL; // ���json��ʽ
	//cJSON *cjson_data = NULL; // ����
	const char *massage;
	// ������������
	u8 cjson_cmd; // ָ��,����

	/* ��������JSO���� */
	cjson_test = cJSON_Parse(message);
	if (cjson_test == NULL)
	{
		// ����ʧ��
		printf("parse fail.\n");
		return MY_FAIL;
	}

	/* ���θ���������ȡJSON���ݣ���ֵ�ԣ� */
	cjson_cmd = cJSON_GetObjectItem(cjson_test, "cmd")->valueint;
	/* ����Ƕ��json���� */
	//cjson_data = cJSON_GetObjectItem(cjson_test, "data");

	switch (cjson_cmd)
	{
	case 0x01: // ��Ϣ��

		threshold_value_init.ls_v = cJSON_GetObjectItem(cjson_test, "ls_v")->valueint;
		threshold_value_init.lm_v = cJSON_GetObjectItem(cjson_test, "lm_v")->valueint;

		W_Test();
	
		if (Connect_Net && Data_init.App == 0) {      
			Data_init.App = 1;
		}
		break;
		
	case 0x02: // ��Ϣ��
		device_state_init.door_1 = cJSON_GetObjectItem(cjson_test, "door_1")->valueint;
			
		if (Connect_Net && Data_init.App == 0) {      
			Data_init.App = 1;
			
		}
	break;
	case 0x03: // ��Ϣ��
		device_state_init.door_2 = cJSON_GetObjectItem(cjson_test, "door_2")->valueint;
			
		if (Connect_Net && Data_init.App == 0) {      
			Data_init.App = 1;
			
		}
	
	break;
	case 0x04: // ��Ϣ��
		Data_init.ls = cJSON_GetObjectItem(cjson_test, "ls")->valueint;
		Data_init.lm = cJSON_GetObjectItem(cjson_test, "lm")->valueint;

		W_Test();
	
		if (Connect_Net && Data_init.App == 0) {      
			Data_init.App = 1;
			
		}
		break;
	default:
		break;
	}

	/* ���JSON����(��������)���������� */
	cJSON_Delete(cjson_test);

	return MY_SUCCESSFUL;
}
// ��������
mySta massage_k210(char *message)
{

	char *dataPtr = NULL;

	char numBuf[10];
	int num = 0;

	dataPtr = strchr(message, ':'); // ����':'

	if (dataPtr != NULL) // ����ҵ���
	{
		dataPtr++;
		while (*dataPtr >= '0' && *dataPtr <= '9') // �ж��Ƿ����·��������������
		{
			numBuf[num++] = *dataPtr++;
		}
		numBuf[num] = 0;
		num = atoi((const char *)numBuf); // תΪ��ֵ��ʽ
		if (strstr((char *)message, "kind")) // "fruits"
		{
			Data_init.category = num;
			// Դӡյ
			printf("K230 recv: kind=%d\r\n", num);
		}

	}
	// printf("%s\n", message);
	return MY_SUCCESSFUL;
}
