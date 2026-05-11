#ifndef __GIT__H
#define __GIT__H


//  设备使用外设定义
#define OLED 1			// 是否使用OLED
#define NET_SERVE 1		// 平台选择
#define NETWORK_CHAEK 1 // 是否开启掉线检测
#define KEY_OPEN 1		// 是否开启长按和短按检测
#define USART2_OPEN 1	// 是否使用串口二
#define SIZE 10

// 数据定义
typedef unsigned char U8;
typedef signed char S8;
typedef unsigned short U16;
typedef signed short S16;
typedef unsigned int U32;
typedef signed int S32;
typedef float F32;

//  C库
#include "cjson.h"
#include <string.h>
#include <stdio.h>
// 单片机头文件
#include "sys.h"
#include "usart.h"	 
// 网络协议层
#include "Net.h"
// 网络设备
#include "usart3.h"
// 任务栏
#include "task.h"
#include "timer.h"
// 硬件驱动
#include "delay.h"
#include "usart.h"
#include "git.h"
#include "led.h"
#include "key.h"
#include "timer.h"
#include "flash.h"
#include "HX711.h"
#include "usart2.h"
#include "sg90.h"
#include "relay.h"

#if OLED // OLED文件存在
#include "oled.h"
#endif


// 服务器信息
#define SSID "NET"		// 路由器SSID名称
#define PASS "12345678" // 路由器密码
#if NET_SERVE == 0
// 多协议服务器（Onenet旧版支持）
#define ServerIP "183.230.40.39" // 服务器IP地址
#define ServerPort 6002			 // 服务器IP地址端口号
#elif NET_SERVE == 1
// 物联网开发平台（阿里云支持）
#define ServerIP "iot-06z00axdhgfk24n.mqtt.iothub.aliyuncs.com" // 服务器IP地址
#define ServerPort 1883											// 服务器IP地址端口号
#elif NET_SERVE == 2
// EMQX平台（自主）
#define ServerIP "broker.emqx.io" // 服务器IP地址
#define ServerPort 1883			  // 服务器IP地址端口号
#elif NET_SERVE == 3
// 华为云平台
#define ServerIP "4a33038a2b.st1.iotda-device.cn-north-4.myhuaweicloud.com" // 服务器IP地址
#define ServerPort 1883			  // 服务器IP地址端口号
#endif
// 设备信息
#define PROID "smartdevice&h9sjbUhCD1T"															 // 产品ID
#define DEVID "h9sjbUhCD1T.smartdevice|securemode=2,signmethod=hmacsha256,timestamp=1734662773090|" // 设备ID
#define AUTH_INFO "d6635f71abe5f3d5650cd62010cc863ac6a1bbc1e18985095d7e9d86c5996b7c"						 // 鉴权信息
// MQTT主题 /broadcast/
#define S_TOPIC_NAME "/broadcast/h9sjbUhCD1T/test1" // 需要订阅的主题
#define P_TOPIC_NAME "/broadcast/h9sjbUhCD1T/test2" // 需要发布的主题


#define P_TOPIC_CMD "/sys/h9sjbUhCD1T/smartdevice/thing/event/property/post"

// 自定义布尔类型
typedef enum
{
	MY_TRUE,
	MY_FALSE
} myBool;

// 自定义执行结果类型
typedef enum
{
	MY_SUCCESSFUL = 0x01, // 成功
	MY_FAIL = 0x00		  // 失败

} mySta; // 成功标志位

typedef enum
{
	OPEN = 0x01, // 打开
	CLOSE = 0x00 // 关闭

} On_or_Off_TypeDef; // 成功标志位

typedef enum
{
	DERVICE_SEND = 0x00, // 设备->平台
	PLATFORM_SEND = 0x01 // 平台->设备

} Send_directino; // 发送方向

typedef struct
{
	U8 App;			 // 指令模式
	U8 App_S_Time;			 // 发送倒计时
	U8 Time;
	U8 Device_State; // 模式
	U8 Page;		 // 页面
	U8 Error_Time;
	U8 time_cut_page; // 页面

	U8 Flage;		 // 模式选择

	F32 git_wight; //累计重量
	U8 wight_time;			 // 重量时间
	F32 average_wight; //平均重量
	F32 add_wight; //累计重量

	
	U8 category; // 类别
	
	U16 ls; // 螺丝
	U16 lm; // 螺母
	
	F32 wight_1; //重量
	F32 wight_2;
	
} Data_TypeDef; // 数据参数结构体

typedef struct
{

	U16 ls_v; // 螺丝
	U16 lm_v; // 螺母

	
} Threshold_Value_TypeDef; // 数据参数结构体

typedef struct
{
	U8 check_device; // 状态
	U8 check_open;	 // 监测是否由物体
	
	U8 door_1;	 // 门1
	U8 door_2;	 // 门2
	
	U8 LED;	 // 照明灯
	
} Device_Satte_Typedef; // 状态参数结构体

// 全局引用
extern Data_TypeDef Data_init;
extern Device_Satte_Typedef device_state_init; // 设备状态
extern Threshold_Value_TypeDef threshold_value_init; // 设备阈值设置结构体

// 获取数据参数
mySta Read_Data(Data_TypeDef *Device_Data);
// 初始化
mySta Reset_Threshole_Value(Threshold_Value_TypeDef *Value, Device_Satte_Typedef *device_state);
// 更新OLED显示屏中内容
mySta Update_oled_massage(void);
// 更新设备状态
mySta Update_device_massage(void);
// 解析json数据
mySta massage_parse_json(char *message);
// 按键
void Check_Key_ON_OFF(void);
// 定时
void Automation_Close(void);
// 解析数据
mySta massage_k210(char *message);


#endif
