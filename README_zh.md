# 工业标准件智能分区指示系统

一个综合性的多平台嵌入式系统和物联网项目，包含Android应用程序、Python算法和STM32固件。

---

## 🌐 Language / 语言

[English](README.md) | **中文介绍**

---

## 📁 项目结构

```
De/
├── APP/                          # Android 应用程序 (Kotlin)
│   ├── app/                      # Android 应用模块
│   ├── gradle/                   # Gradle 包装文件
│   └── build.gradle              # 根构建配置
│
├── K230/                         # Python 算法
│   ├── det_video_*.py            # 视频检测脚本
│   └── main.py                   # Python 主入口
│
└── STM32F103C8T6/               # STM32 嵌入式固件 (C语言)
    ├── CORE/                     # CMSIS 核心文件
    ├── HARDWARE/                 # 硬件驱动程序
    │   ├── ADC/                  # ADC 驱动
    │   ├── ADXL345/              # 加速度计驱动
    │   ├── DHT11/                # 温湿度传感器
    │   ├── HC_SR04/              # 超声波传感器
    │   ├── HX711/                # 称重传感器放大器
    │   ├── LCD1602/              # LCD 显示驱动
    │   ├── LED/                  # LED 控制
    │   ├── MAX30102/             # 心率传感器
    │   ├── OLED-IIC/             # OLED 显示 (I2C)
    │   ├── OLED-SPI/             # OLED 显示 (SPI)
    │   ├── RELAY/                # 继电器控制
    │   └── SG90/                 # 舵机驱动
    ├── NET/                      # 网络模块
    │   ├── Json/                 # JSON 解析 (cJSON)
    │   ├── device/               # 设备通信
    │   ├── massage/              # 消息处理
    │   ├── mqtt/                 # MQTT 协议
    │   └── net/                  # 网络工具
    ├── SYSTEM/                   # 系统级驱动
    │   ├── delay/                # 延时函数
    │   ├── sys/                  # 系统配置
    │   └── usart/                # UART 通信
    ├── TASK/                     # 任务管理
    └── USER/                     # 用户应用程序代码
```

## 🚀 功能特点

### Android 应用程序
- 实时数据监控
- 用户历史记录追踪
- 与嵌入式设备的MQTT通信
- 现代Material Design界面
- 数据库本地存储
- 图表数据可视化

### Python 算法
- 视频处理与分析
- 计算机视觉能力
- 与嵌入式系统集成

### STM32 固件
- 多传感器数据采集
- 实时操作系统支持
- 无线通信 (ESP8266 + MQTT)
- 综合硬件驱动库
- 多种显示接口支持 (LCD1602, OLED)

## 🛠️ 安装与配置

### 前置要求

- **Android 开发**: Android Studio 4.0+, Gradle 7.0+, JDK 11+
- **Python**: Python 3.8+
- **嵌入式开发**: Keil MDK 5.0+, STM32CubeMX (可选)

### Android 应用

```bash
# 进入 APP 目录
cd APP

# 构建 Debug APK
./gradlew assembleDebug

# 在连接设备上安装
./gradlew installDebug
```

### Python 脚本

```bash
# 进入 K230 目录
cd K230

# 安装依赖（如需要）
pip install -r requirements.txt

# 运行主脚本
python main.py
```

### STM32 固件

1. 在 Keil MDK 中打开 `STM32F103C8T6/USER/LED.uvprojx`
2. 配置你的编程器 (J-Link/ST-Link)
3. 构建并下载到目标设备

## 📖 使用说明

### Android 应用
1. 在 Android 设备上安装 APK
2. 配置 MQTT broker 设置
3. 连接到网络
4. 开始监控传感器数据

### Python 脚本
```bash
python main.py
```

### STM32 固件
- 在 `net_config.h` 中配置 WiFi 凭据
- 设备将自动连接到 MQTT broker
- 监控串口输出以进行调试

## 🤝 贡献指南

欢迎贡献！请随时提交 Pull Request。对于重大更改，请先开 issue 讨论您想要更改的内容。

### 开发流程

1. Fork 本仓库
2. 创建功能分支 (`git checkout -b feature/AmazingFeature`)
3. 提交更改 (`git commit -m 'Add some AmazingFeature'`)
4. 推送到分支 (`git push origin feature/AmazingFeature`)
5. 创建 Pull Request

### 代码规范

- **C 代码**: 遵循 MISRA C 规范
- **Kotlin 代码**: 遵循 Android Kotlin 指南
- **Python 代码**: 遵循 PEP 8 风格指南
- 使用有意义的变量和函数名称
- 为复杂逻辑添加注释
- 为新功能编写单元测试

## 📝 许可证

本项目目前未获得许可，仅供个人/教育使用。

如需商业使用，请联系作者获取许可条款。

---

## 📧 联系方式

- **作者**: SunYang
- **项目链接**: (上传GitHub后添加)

## 🙏 致谢

- STMicroelectronics 的 STM32 HAL 驱动
- Android 开源项目
- cJSON 库贡献者
- MQTT 社区的协议规范

## 🔗 相关资源

- [STM32 官方文档](https://www.st.com/en/microcontrollers-microprocessors/stm32f103c8.html)
- [Android 开发者](https://developer.android.com/)
- [MQTT 协议](https://mqtt.org/)
- [Keil MDK 文档](https://www.keil.com/support/man/docs/mdk5/)
