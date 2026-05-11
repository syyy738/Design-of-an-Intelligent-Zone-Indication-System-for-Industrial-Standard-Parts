# Embedded Systems and IoT Project

A comprehensive multi-platform embedded systems and IoT project containing Android application, Python algorithms, and STM32 firmware.

## 📁 Project Structure

```
De/
├── APP/                          # Android Application (Kotlin)
│   ├── app/                      # Main Android app module
│   ├── gradle/                   # Gradle wrapper files
│   └── build.gradle              # Root build configuration
│
├── K230/                         # Python Algorithms
│   ├── det_video_*.py            # Video detection scripts
│   └── main.py                   # Main Python entry point
│
└── STM32F103C8T6/               # STM32 Embedded Firmware (C)
    ├── CORE/                     # CMSIS core files
    ├── HARDWARE/                 # Hardware drivers
    │   ├── ADC/                  # ADC driver
    │   ├── ADXL345/              # Accelerometer driver
    │   ├── DHT11/                # Temperature/Humidity sensor
    │   ├── HC_SR04/              # Ultrasonic sensor
    │   ├── HX711/                # Load cell amplifier
    │   ├── LCD1602/              # LCD display driver
    │   ├── LED/                  # LED control
    │   ├── MAX30102/             # Heart rate sensor
    │   ├── OLED-IIC/             # OLED display (I2C)
    │   ├── OLED-SPI/             # OLED display (SPI)
    │   ├── RELAY/                # Relay control
    │   └── SG90/                 # Servo motor driver
    ├── NET/                      # Network modules
    │   ├── Json/                 # JSON parsing (cJSON)
    │   ├── device/               # Device communication
    │   ├── massage/              # Message handling
    │   ├── mqtt/                 # MQTT protocol
    │   └── net/                  # Network utilities
    ├── SYSTEM/                   # System level drivers
    │   ├── delay/                # Delay functions
    │   ├── sys/                  # System configuration
    │   └── usart/                # UART communication
    ├── TASK/                     # Task management
    └── USER/                     # User application code
```

## 🚀 Features

### Android Application
- Real-time data monitoring
- User history tracking
- MQTT communication with embedded devices
- Modern Material Design interface

### Python Algorithms
- Video processing and analysis
- Computer vision capabilities
- Integration with embedded systems

### STM32 Firmware
- Multi-sensor data acquisition
- Real-time operating system support
- Wireless communication (ESP8266 + MQTT)
- Comprehensive hardware driver library

## 🛠️ Installation & Setup

### Prerequisites

- **Android Development**: Android Studio 4.0+, Gradle 7.0+, JDK 11+
- **Python**: Python 3.8+
- **Embedded**: Keil MDK 5.0+, STM32CubeMX (optional)

### Android App

```bash
# Navigate to APP directory
cd APP

# Build debug APK
./gradlew assembleDebug

# Install on connected device
./gradlew installDebug
```

### Python Scripts

```bash
# Navigate to K230 directory
cd K230

# Install dependencies (if any)
pip install -r requirements.txt

# Run main script
python main.py
```

### STM32 Firmware

1. Open `STM32F103C8T6/USER/LED.uvprojx` in Keil MDK
2. Configure your programmer (J-Link/ST-Link)
3. Build and download to target device

## 📖 Usage

### Android App
1. Install the APK on an Android device
2. Configure the MQTT broker settings
3. Connect to the network
4. Start monitoring sensor data

### Python Scripts
```bash
python main.py
```

### STM32 Firmware
- Configure WiFi credentials in `net_config.h`
- The device will automatically connect to MQTT broker
- Monitor serial output for debugging

## 🤝 Contributing

Contributions are welcome! Please feel free to submit a Pull Request. For major changes, please open an issue first to discuss what you would like to change.

### Development Workflow

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

### Coding Standards

- **C Code**: Follow MISRA C guidelines
- **Kotlin Code**: Follow Android Kotlin guidelines
- **Python Code**: Follow PEP 8 style guide
- Use meaningful variable and function names
- Comment complex logic
- Write unit tests for new features

## 📝 License

This project is currently not licensed and is for personal/educational use only.

If you intend to use this code commercially, please contact the author for licensing terms.

---

## 📧 Contact

- **Author**: SunYang
- **Project Link**: (To be added after GitHub upload)

## 🙏 Acknowledgments

- STM32 HAL drivers by STMicroelectronics
- Android Open Source Project
- cJSON library contributors
- MQTT community for protocol specifications

## 🔗 Related Resources

- [STM32 Official Documentation](https://www.st.com/en/microcontrollers-microprocessors/stm32f103c8.html)
- [Android Developers](https://developer.android.com/)
- [MQTT Protocol](https://mqtt.org/)
- [Keil MDK Documentation](https://www.keil.com/support/man/docs/mdk5/)
