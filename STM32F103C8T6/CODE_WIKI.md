# 智能设备控制终端 - Code Wiki

## 项目概述

本项目是一个基于STM32F103C8T6微控制器的智能设备控制终端，集成了多种传感器、通信模块和云平台连接功能。系统采用模块化设计，支持重量检测、心率监测、温湿度监测、超声波测距等多种功能，并通过WiFi模块（ESP8266）连接至各大物联网云平台，实现远程监控与控制。

### 技术规格

| 参数 | 规格 |
|------|------|
| 微控制器 | STM32F103C8T6 (ARM Cortex-M3) |
| 主频 | 72MHz |
| 通信接口 | USART (3个)、I2C、SPI |
| 网络模块 | ESP8266 WiFi |
| 支持平台 | OneNET、阿里云IoT、EMQX、华为云 |

---

## 系统架构

### 整体架构图

```
┌─────────────────────────────────────────────────────────────┐
│                         应用层                               │
│  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐       │
│  │  Task1 (1s)  │  │  Task2 (250ms) │  │  Task3 (4s)  │       │
│  │  自动化控制   │  │  数据采集与上报 │  │  应用逻辑    │       │
│  └──────────────┘  └──────────────┘  └──────────────┘       │
├─────────────────────────────────────────────────────────────┤
│                       业务逻辑层                             │
│  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐       │
│  │ git模块      │  │ massage模块   │  │ 定时器模块    │       │
│  │ (核心业务)   │  │ (消息解析)    │  │ (任务调度)    │       │
│  └──────────────┘  └──────────────┘  └──────────────┘       │
├─────────────────────────────────────────────────────────────┤
│                        网络层                                │
│  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐       │
│  │ ESP8266驱动  │  │ MQTT协议栈    │  │ cJSON解析    │       │
│  │ (Usart3)    │  │ (MqttKit)    │  │ (Json解析)   │       │
│  └──────────────┘  └──────────────┘  └──────────────┘       │
├─────────────────────────────────────────────────────────────┤
│                       硬件抽象层                              │
│  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐       │
│  │ LED/GPIO    │  │ USART驱动    │  │ PWM/TIM     │       │
│  │ 按键/继电器  │  │ 通信接口     │  │ 定时器输出   │       │
│  └──────────────┘  └──────────────┘  └──────────────┘       │
├─────────────────────────────────────────────────────────────┤
│                       传感器层                                │
│  ┌─────────┐ ┌─────────┐ ┌─────────┐ ┌─────────┐ ┌────────┐│
│  │ HX711   │ │MAX30102│ │ DHT11   │ │ HC-SR04 │ │ ADXL345││
│  │ 重量    │ │ 心率    │ │ 温湿度  │ │ 超声波   │ │ 加速度 ││
│  └─────────┘ └─────────┘ └─────────┘ └─────────┘ └────────┘│
├─────────────────────────────────────────────────────────────┤
│                        芯片层                                 │
│  STM32F103C8T6 + ESP8266 + K210 AI模块                       │
└─────────────────────────────────────────────────────────────┘
```

### 目录结构

```
02.NET/
├── USER/                          # 用户应用程序
│   ├── main.c                     # 主程序入口
│   ├── stm32f10x_it.c             # 中断服务程序
│   ├── system_stm32f10x.c         # 系统时钟配置
│   └── LED.uvprojx                # Keil工程文件
│
├── SYSTEM/                        # 系统基础模块
│   ├── sys/                       # 系统定义与GPIO操作宏
│   ├── delay/                     # 延时函数
│   └── usart/                     # 串口通信
│
├── HARDWARE/                      # 硬件驱动层
│   ├── LED/                       # LED指示灯
│   ├── KEY/                       # 按键输入
│   ├── RELAY/                     # 继电器控制
│   ├── OLED-IIC/                  # OLED显示屏(I2C接口)
│   ├── OLED-SPI/                  # OLED显示屏(SPI接口)
│   ├── HX711/                     # 重量传感器
│   ├── MAX30102/                  # 心率血氧传感器
│   ├── DHT11/                     # 温湿度传感器
│   ├── HC_SR04/                   # 超声波测距
│   ├── ADXL345/                   # 三轴加速度计
│   ├── SG90/                      # 舵机控制
│   ├── FLASH/                     # Flash读写
│   ├── USART2/                    # 串口2(K210通信)
│   └── EXTI/                      # 外部中断
│
├── TASK/                          # 任务与业务逻辑
│   ├── task/                      # 任务调度器
│   ├── git/                       # 核心业务逻辑
│   └── timer/                     # 定时器配置
│
├── NET/                           # 网络通信层
│   ├── net/                       # 网络初始化
│   ├── device/                    # ESP8266驱动
│   ├── massage/                   # 消息处理
│   ├── mqtt/                      # MQTT协议实现
│   └── Json/                      # JSON解析库
│
├── CORE/                          # ARM Cortex-M3核心
│   ├── core_cm3.c/h               # 内核访问函数
│   └── startup_stm32f10x_hd.s     # 启动文件
│
└── STM32F10x_FWLib/              # STM32标准外设库
    ├── inc/                       # 头文件
    └── src/                       # 源文件
```

---

## 核心模块详解

### 1. 主程序模块 (USER/main.c)

#### 初始化流程

```c
int main(void)
{
    SoftWare_Init();    // 1. 软件初始化(定时器任务)
    Hardware_Init();    // 2. 硬件初始化
    Net_Init();         // 3. 网络连接
    TIM_Cmd(TIM4, ENABLE); // 4. 启动定时器
    Get_Maopi();        // 5. 获取称重毛皮值
    BeepNo(250);        // 6. 蜂鸣器提示
    
    while(1)
    {
        timer_loop();           // 任务调度
        ESP8266_GetIPD(2);       // 网络数据接收
        Send_Usart2();           // K210通信
        Check_Key_ON_OFF();      // 按键处理
    }
}
```

#### 三大任务

| 任务 | 执行周期 | 功能说明 |
|------|----------|----------|
| Task1 | 1000ms | 自动化关闭、更新OLED显示 |
| Task2 | 250ms | 读取传感器数据、上报设备状态 |
| Task3 | 5000ms | 应用逻辑处理 |

---

### 2. 任务调度器 (TASK/task/)

#### 设计原理

采用链表实现的轻量级定时器调度器，无需操作系统即可实现多任务管理。

```
┌─────────────────────────────────────────┐
│            head_handle (链表头)         │
└──────────────┬──────────────────────────┘
               │
               ▼
┌─────────────────────────────────────────┐
│  Timer Node 1                           │
│  ├─ timeout_cb: task1_callback          │
│  ├─ timeout: 1000ms                     │
│  ├─ repeat: 1000ms                      │
│  └─ next: ───────────────► Node 2       │
└─────────────────────────────────────────┘
```

#### 核心数据结构

```c
struct Timer {
    void (*timeout_cb)(void);  // 回调函数指针
    unsigned int timeout;       // 超时时间点
    unsigned int repeat;        // 重复周期(0=单次)
    struct Timer *next;         // 链表下一节点
};
```

#### 关键函数

| 函数 | 功能 | 参数说明 |
|------|------|----------|
| `timer_init()` | 初始化定时器结构 | handle: 定时器句柄, timeout_cb: 回调函数, timeout: 超时时间, repeat: 重复周期 |
| `timer_start()` | 启动定时器 | handle: 定时器句柄 |
| `timer_stop()` | 停止定时器 | handle: 定时器句柄 |
| `timer_loop()` | 轮询检查定时器 | 需在主循环中调用 |
| `timer_ticks()` | 定时器节拍递增 | 1ms调用一次 |

---

### 3. 核心业务模块 (TASK/git/)

#### 全局配置定义

```c
// 功能开关配置
#define OLED 1              // OLED显示功能
#define NET_SERVE 1         // 云平台选择 (0:OneNET, 1:阿里云, 2:EMQX, 3:华为云)
#define NETWORK_CHAEK 1      // 网络状态检测
#define KEY_OPEN 1          // 按键功能
#define USART2_OPEN 1       // K210通信
```

#### 云平台配置

```c
// 阿里云IoT配置
#define ServerIP "iot-06z00axdhgfk24n.mqtt.iothub.aliyuncs.com"
#define ServerPort 1883
#define PROID "smartdevice&h9sjbUhCD1T"
#define DEVID "h9sjbUhCD1T.smartdevice|securemode=2,signmethod=hmacsha256..."
#define AUTH_INFO "d6635f71abe5f3d5650cd62010cc863ac6a1bbc1e18985095d7e9d86c5996b7c"
```

#### 核心数据结构

```c
// 数据参数结构体
typedef struct {
    U8 App;              // 应用模式
    U8 App_S_Time;       // 应用开始时间
    U8 Time;             // 时间戳
    U8 Device_State;     // 设备状态
    U8 Page;             // 页面索引
    U8 Error_Time;       // 错误计数
    U8 time_cut_page;    // 切页时间
    U8 Flage;            // 模式标志
    
    F32 git_wight;       // 累计重量
    U8 wight_time;       // 称重时间
    F32 average_wight;   // 平均重量
    F32 add_wight;       // 累计增加值
    
    U8 category;         // 分类
    U16 ls;              // 滤水
    U16 lm;              // 滤棉
    F32 wight_1;         // 重量值1
    F32 wight_2;         // 重量值2
} Data_TypeDef;

// 阈值配置结构体
typedef struct {
    U16 ls_v;            // 滤水阈值
    U16 lm_v;            // 滤棉阈值
} Threshold_Value_TypeDef;

// 设备状态结构体
typedef struct {
    U8 check_device;     // 设备检查状态
    U8 check_open;       // 检查开关
    U8 door_1;           // 门1状态
    U8 door_2;           // 门2状态
    U8 LED;              // LED指示
} Device_Satte_Typedef;
```

#### 公共API

| 函数 | 返回值 | 功能 |
|------|--------|------|
| `Read_Data()` | mySta | 读取并更新传感器数据 |
| `Reset_Threshole_Value()` | mySta | 初始化阈值配置 |
| `Update_oled_massage()` | mySta | 更新OLED显示内容 |
| `Update_device_massage()` | mySta | 上报设备状态至云平台 |
| `massage_parse_json()` | mySta | 解析云平台下发的JSON指令 |
| `Check_Key_ON_OFF()` | void | 处理按键事件 |
| `Automation_Close()` | void | 自动化关闭控制 |
| `massage_k210()` | mySta | K210通信消息处理 |

---

### 4. 网络通信模块 (NET/)

#### ESP8266驱动 (device/usart3.h)

| 函数 | 功能 |
|------|------|
| `ESP8266_Init()` | 初始化ESP8266 WiFi模块 |
| `ESP8266_SendData()` | 发送TCP数据 |
| `ESP8266_SendMqtt()` | 发送MQTT数据包 |
| `ESP8266_GetIPD()` | 获取网络数据(带超时) |

#### MQTT协议 (mqtt/)

基于MqttKit库实现，支持订阅、发布功能：

```c
// MQTT操作
_Bool DevLink();                    // 连接云平台认证
_Bool Subscribe(topics[], 1);       // 订阅主题
_Bool Publish(topic, msg);          // 发布消息
void RevPro(cmd);                   // 消息处理回调
```

#### 消息解析 (massage/)

```c
// JSON消息初始化
mySta heart_json_init();    // 心跳包
mySta login_json_init();    // 登录认证包
mySta data_json_init();     // 数据上报包

// 消息处理
mySta massage_parse_json(char *message);  // 解析云平台指令
mySta Update_device_massage();            // 更新设备状态
```

---

### 5. 硬件驱动层 (HARDWARE/)

#### LED指示灯 (LED/)

```c
// GPIO定义
#define State    PCout(13)   // 状态指示灯 (PC13)
#define LED1     PBout(6)    // LED1 (PB6)
#define LED2     PBout(7)    // LED2 (PB7)
#define LED3     PBout(8)    // LED3 (PB8)
#define LED4     PBout(9)    // LED4 (PB9)
#define BEEP     PBout(6)    // 蜂鸣器 (PB6)

// 操作接口
void LED_Init(void);              // LED初始化
void Mqtt_LED(U8 Cmd);           // MQTT控制LED
void System_PB34_setIO(void);     // PB3/PB4配置
```

#### 按键输入 (KEY/)

| 按键 | GPIO | 引脚 |
|------|------|------|
| KEY1 | GPIOB | PB3 |
| KEY2 | GPIOB | PB4 |
| KEY3 | GPIOA | PA6 |

```c
void Key_GPIO_Config(void);       // GPIO初始化
u8 KEY_Scan(u8 mode);            // 按键扫描(支持连按)
void Send_Cmd(void);              // 发送按键命令
```

#### 继电器控制 (RELAY/)

```c
// 继电器GPIO定义
#define RELAY1  PBout(6)    // 继电器1
#define RELAY2  PBout(7)    // 继电器2
#define RELAY3  PBout(8)    // 继电器3
#define BEEP    PBout(9)    // 蜂鸣器

void RELAY_GPIO_Config(void);     // GPIO初始化
void BeepNo(u16 time);           // 蜂鸣器鸣叫(延时ms)
```

#### OLED显示 (OLED-IIC/)

支持I2C接口的0.96寸OLED显示屏：

```c
void OLED_Init(void);                     // 初始化
void OLED_Clear(void);                    // 清屏
void OLED_ShowString(x, y, str, size);    // 显示字符串
void OLED_ShowNum(x, y, num, len, size); // 显示数字
void OLED_ShowChinese(x, y, num, size);  // 显示中文
void OLED_ShowPicture(x0, y0, x1, y1, BMP[]); // 显示图片
void OLED_DisplayTurn(i);                 // 屏幕翻转
void OLED_ColorTurn(i);                   // 颜色反转
```

#### 重量传感器 (HX711/)

两路HX711称重传感器，用于测量物体重量：

```c
// 通道1 (GPIOA PA0/PA1)
#define HX711_SCK   PAout(0)   // 时钟引脚
#define HX711_DOUT  PAin(1)    // 数据引脚

// 通道2 (GPIOA PA4/PA5)
#define HX711_SCK1  PAout(4)
#define HX711_DOUT1 PAin(5)

void Init_HX711pin(void);      // 初始化通道1
void Init1_HX711pin(void);      // 初始化通道2
u32 HX711_Read(void);          // 读取ADC值
void Get_Maopi(void);           // 获取毛皮值(去皮)
void Get_Weight(void);          // 获取实际重量
```

#### 心率传感器 (MAX30102/)

光电式心率血氧传感器，通过I2C接口通信：

```c
void max30102_init(void);               // 传感器初始化
void max30102_reset(void);              // 复位传感器
u8 max30102_Bus_Write(addr, data);     // I2C写寄存器
u8 max30102_Bus_Read(addr);             // I2C读寄存器
void max30102_FIFO_ReadBytes(addr, *Data); // 读取FIFO数据
```

#### 温湿度传感器 (DHT11/)

单总线数字温湿度传感器：

```c
typedef struct {
    uint8_t humi_int;     // 湿度整数
    uint8_t humi_deci;    // 湿度小数
    uint8_t temp_int;     // 温度整数
    uint8_t temp_deci;    // 温度小数
    uint8_t check_sum;     // 校验和
} DHT11_Data_TypeDef;

void DHT11_GPIO_Config(void);                    // GPIO初始化
uint8_t Read_DHT11(DHT11_Data_TypeDef *data);  // 读取数据
```

#### 超声波测距 (HC_SR04/)

```c
// GPIO定义
#define SR04_Trgl2  PBout(13)   // 触发引脚
#define SR04_Echo2  PBin(12)    // 回波引脚

void HC_SR04_IO2_Init(void);              // GPIO初始化
void TIM3_Init(u16 arr, u16 psc);         // 定时器初始化
u16 Get_SR04_Distance2(void);             // 获取距离值(mm)
```

#### 舵机控制 (SG90/)

使用PWM信号控制舵机角度：

```c
void TIM2_PWM_Init(U32 f);                // TIM2 PWM初始化
void TIM3_PWM_Init(U32 f);                // TIM3 PWM初始化
void MG996R_Speed(void);                  // MG996R速度设置
void Automation_SG90_Angle(U8 cmd, U8 dirt); // 角度控制
/*
 * 角度控制参数:
 * - cmd: 角度值 (5-14: 一方向, 16-25: 另一方向)
 * - dirt: 方向选择
 * - 周期: 20ms
 */
```

---

### 6. 系统基础模块 (SYSTEM/)

#### GPIO操作宏 (sys/sys.h)

位带操作实现51单片机风格的GPIO控制：

```c
// GPIO输出操作
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr, n)
#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr, n)
#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr, n)

// GPIO输入操作
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr, n)
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr, n)
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr, n)

// 数字电平操作
#define digitalHi(p, i)  (p->BSRR = i)   // 置高
#define digitalLo(p, i)  (p->BRR = i)     // 置低
#define digitalToggle(p, i) (p->ODR ^= i) // 翻转
```

#### 延时函数 (delay/)

```c
void delay_init(void);       // 延时初始化
void delay_ms(u16 nms);      // 毫秒延时
void delay_us(u32 nus);      // 微秒延时
void Sys_Restart(void);      // 系统软复位
```

#### 串口通信 (usart/)

| 串口 | 用途 | 波特率 | 说明 |
|------|------|--------|------|
| USART1 | 调试/日志 | 9600 | PC通信 |
| USART2 | K210通信 | 115200 | AI视觉模块 |
| USART3 | ESP8266 | 115200 | WiFi模块 |

```c
void Usart1_Init(u32 bound);        // 串口1初始化
void Usart2_Init(u32 bound, x, y); // 串口2初始化
void Usart_SendByte(USARTx, ch);    // 发送单字节
void Usart_SendString(USARTx, str); // 发送字符串
void Usart_SendHalfWord(USARTx, w); // 发送16位数
```

---

## 定时器配置 (TASK/timer/)

### 定时器分配

| 定时器 | 用途 | 中断周期 | 说明 |
|--------|------|----------|------|
| TIM2 | 通用定时器 | 1ms | 可用于PWM |
| TIM3 | PWM输出 | 1ms | 舵机控制 |
| TIM4 | 系统节拍 | 1ms | 任务调度心跳 |
| TIM5 | 通用定时器 | 1ms | 备用 |
| TIM6 | 快速定时 | 100μs | 高速应用 |
| TIM7 | 通用定时器 | 1ms | 备用 |

### PWM输出通道

```c
// TIM2 PWM通道
TIM2_CH1: PA0
TIM2_CH2: PA1
TIM2_CH3: PA2
TIM2_CH4: PA3

// TIM3 PWM通道
TIM3_CH1: PA6
TIM3_CH2: PA7
TIM3_CH3: PB0
TIM3_CH4: PB1
```

---

## 通信协议

### MQTT主题定义

```c
// 订阅主题 (接收云平台指令)
#define S_TOPIC_NAME "/broadcast/h9sjbUhCD1T/test1"

// 发布主题 (上报设备数据)
#define P_TOPIC_NAME "/broadcast/h9sjbUhCD1T/test2"

// 属性上报主题
#define P_TOPIC_CMD "/sys/h9sjbUhCD1T/smartdevice/thing/event/property/post"
```

### JSON数据格式

#### 上报数据结构

```json
{
    "Device_ID": "12345678",
    "timestamp": 1700000000,
    "data": {
        "weight": 125.5,
        "door_state": 1,
        "temperature": 25.6,
        "humidity": 60.2
    }
}
```

#### 指令接收格式

```json
{
    "cmd": "control",
    "params": {
        "relay1": 1,
        "relay2": 0,
        "servo_angle": 15
    }
}
```

---

## 依赖关系

### 头文件包含关系

```
git.h (主头文件)
├── cjson.h              # JSON解析
├── sys.h                # 系统定义
├── usart.h              # 串口通信
├── Net.h               # 网络接口
├── usart3.h            # ESP8266
├── task.h              # 任务调度
├── timer.h             # 定时器
├── delay.h             # 延时
├── led.h               # LED控制
├── key.h               # 按键
├── flash.h             # Flash
├── HX711.h             # 称重
├── usart2.h            # K210通信
├── sg90.h              # 舵机
├── relay.h             # 继电器
└── oled.h              # OLED显示(条件编译)
```

### 编译依赖

| 组件 | 版本 | 说明 |
|------|------|------|
| Keil MDK | 5.x | 编译工具链 |
| STM32F10x StdPeriph Lib | 3.5.0 | 标准外设库 |
| cJSON | - | JSON解析库 |
| MqttKit | - | MQTT协议栈 |

---

## 项目运行方式

### 硬件连接

```
STM32F103C8T6
├── PA9(TX)  ────►  ESP8266 RX
├── PA10(RX) ────►  ESP8266 TX
├── PB10(TX) ────►  K210 RX
├── PB11(RX) ────►  K210 TX
├── PB6/SCL ────►  OLED SCL
├── PB7/SDA ────►  OLED SDA
├── PA0      ────►  HX711_1 SCK
├── PA1      ────►  HX711_1 DOUT
├── PA4      ────►  HX711_2 SCK
├── PA5      ────►  HX711_2 DOUT
├── PC13     ────►  状态LED
└── PB6-9   ────►  继电器/LED
```

### 编译步骤

1. 使用Keil MDK打开工程文件 `USER/LED.uvprojx`
2. 选择目标芯片 `STM32F103C8T6`
3. 点击编译按钮或使用快捷键 `F7`
4. 编译成功后，通过J-Link/ST-Link下载到芯片

### 调试配置

- 仿真器: J-Link或ST-Link
- 调试接口: SWD
- 时钟配置: 72MHz外部晶振(HSE)

### 启动流程

1. **系统上电** → 执行 `startup_stm32f10x_hd.s`
2. **主函数入口** → `main()`
3. **软件初始化** → 配置定时器和任务
4. **硬件初始化** → 初始化所有外设
5. **网络连接** → ESP8266连接WiFi和云平台
6. **进入主循环** → 任务调度和事件处理

---

## 注意事项

1. **编译宏配置**: 在 `git.h` 中根据需求修改功能开关
2. **云平台选择**: 修改 `NET_SERVE` 选择不同的物联网平台
3. **WiFi配置**: 修改 `SSID` 和 `PASS` 为实际WiFi信息
4. **设备认证**: 根据选择的云平台配置对应的设备ID和密钥
5. **K210通信**: 确保USART2波特率与K210模块配置一致

---

## 扩展开发

### 添加新传感器

1. 在 `HARDWARE/` 目录下创建新的驱动文件夹
2. 实现传感器初始化和读取函数
3. 在 `git.h` 中添加相关头文件
4. 在 `main.c` 的任务中调用传感器读取函数

### 添加新任务

1. 定义任务函数
2. 创建Timer变量
3. 调用 `timer_init()` 初始化
4. 调用 `timer_start()` 启动任务

```c
static Timer new_task_id;

void new_task(void) {
    // 任务逻辑
}

void SoftWare_Init(void) {
    timer_init(&new_task_id, new_task, 500, 1); // 500ms周期
    timer_start(&new_task_id);
}
```

### 对接新云平台

1. 在 `NET/` 目录下实现对应的协议
2. 修改 `git.h` 中的 `NET_SERVE` 宏
3. 配置新平台的服务器地址和认证信息
4. 实现对应的订阅/发布逻辑

---

## 版本信息

- **创建日期**: 2024年
- **芯片型号**: STM32F103C8T6
- **固件版本**: V1.0
- **支持平台**: OneNET、阿里云IoT、EMQX、华为云
