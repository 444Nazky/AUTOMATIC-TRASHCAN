# 🤖 Automatic Trash Can - Smart Waste Management System

A smart trash can system built with ESP32 that automatically opens when approached, monitors fill capacity using ultrasonic sensors, and integrates with Blynk IoT platform for remote monitoring and control.

![Version](https://img.shields.io/badge/Version-1.0.0-blue)
![License](https://img.shields.io/badge/License-MIT-green)
![Platform](https://img.shields.io/badge/Platform-ESP32-orange)

---

## 📋 Table of Contents

- [Features](#-features)
- [Hardware Requirements](#-hardware-requirements)
- [Software Requirements](#-software-requirements)
- [Circuit Diagram](#-circuit-diagram)
- [Installation](#-installation)
- [Configuration](#-configuration)
- [Usage](#-usage)
- [Blynk App Setup](#-blynk-app-setup)
- [Project Structure](#-project-structure)
- [Flowchart](#-flowchart)
- [Contributing](#-contributing)
- [License](#-license)

---

## ✨ Features

| Feature | Description |
|---------|-------------|
| 🔄 **Auto-Open Lid** | Automatically opens when user approaches within 20cm |
| 📊 **Capacity Monitoring** | Real-time fill level monitoring (0-100%) |
| 📱 **IoT Integration** | Remote monitoring via Blynk mobile app |
| 🔔 **Smart Notifications** | Alerts when trash is 85% full or completely full |
| 🎮 **Manual Control** | Manual override via Blynk app switch |
| 📺 **LCD Display** | Local status display with capacity and messages |
| ⏰ **Auto-Close** | Automatically closes 5 seconds after opening |

---

## 🔧 Hardware Requirements

### Main Components
| Component | Quantity | Purpose |
|-----------|----------|---------|
| ESP32 Dev Board | 1 | Main microcontroller |
| HC-SR04 Ultrasonic Sensor | 2 | Distance measurement |
| SG90 Micro Servo | 1 | Lid mechanism |
| 16x2 I2C LCD | 1 | Local status display |
| Jumper Wires | Various | Connections |
| Breadboard/PCB | 1 | Circuit organization |
| Power Supply (5V) | 1 | System power |

### Pin Configuration
```
┌─────────────────────────────────────────┐
│           ESP32 Pin Layout              │
├─────────────────────────────────────────┤
│  Trig Sensor 1  →  GPIO 5               │
│  Echo Sensor 1  →  GPIO 18              │
│  Trig Sensor 2  →  GPIO 4               │
│  Echo Sensor 2  →  GPIO 15              │
│  Servo Signal   →  GPIO 13              │
│  LCD SDA        →  GPIO 21              │
│  LCD SCL        →  GPIO 22              │
└─────────────────────────────────────────┘
```

---

## 💻 Software Requirements

- **Arduino IDE** 2.0+ or PlatformIO
- **ESP32 Board Package** (version 2.0.0+)
- **Required Libraries:**
  - ESP32Servo
  - WiFi
  - BlynkSimpleEsp32
  - LiquidCrystal_I2C
  - NewPing

---

## 🔌 Circuit Diagram

```
                    ┌─────────────────┐
                    │    5V Power     │
                    └────────┬────────┘
                             │
        ┌────────────────────┼────────────────────┐
        │                    │                    │
        ▼                    ▼                    ▼
   ┌─────────┐        ┌───────────┐        ┌─────────┐
   │  Servo  │        │   LCD     │        │  ESP32  │
   │  SG90   │        │  I2C 16x2 │        │  Dev    │
   └────┬────┘        └─────┬─────┘        └────┬────┘
        │                   │                   │
        │                   │                   │
   GPIO 13            GPIO 21 (SDA)      ┌──────┴──────┐
        │                   │             │             │
        └───────────────────┴─────────────┤ WiFi Ready  │
                                          │             │
                                          └─────────────┘
   ┌───────────────────────────────────────────────────┐
   │              Ultrasonic Sensors                   │
   │                                                   │
   │   Sensor 1 (Object Detection)   Sensor 2 (Level) │
   │   ┌─────────┐                  ┌─────────┐       │
   │   │  Trig   │→ GPIO 5          │  Trig   │→ GPIO 4
   │   │  Echo   │← GPIO 18         │  Echo   │← GPIO 15
   │   └─────────┘                  └─────────┘       │
   └───────────────────────────────────────────────────┘
```

---

## 📦 Installation

### 1. Install Arduino IDE
Download from: https://www.arduino.cc/en/software

### 2. Install ESP32 Board Package
1. Open Arduino IDE
2. Go to `File → Preferences`
3. Add this URL to "Additional Boards Manager URLs":
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```
4. Go to `Tools → Board → Boards Manager`
5. Search for "ESP32" and install

### 3. Install Required Libraries
```bash
# Using Library Manager (Sketch → Include Library → Manage Libraries)
- ESP32Servo
- Blynk
- LiquidCrystal_I2C
- NewPing
```

### 4. Upload the Code
1. Open `sketch.ino` in Arduino IDE
2. Select board: `Tools → Board → ESP32 Dev Module`
3. Select correct port
4. Click Upload (Ctrl+U)

---

## ⚙️ Configuration

### WiFi Settings (in sketch.ino)
```cpp
// Modify these values for your network
char ssid[] = "Your_WiFi_Name";      // WiFi SSID
char pass[] = "Your_WiFi_Password";  // WiFi Password
```

### Blynk Configuration
```cpp
// These are auto-defined from template
#define BLYNK_TEMPLATE_ID "TMPLxxxxxxx"
#define BLYNK_TEMPLATE_NAME "Smart Trash"
#define BLYNK_AUTH_TOKEN "Your_Auth_Token"
```

### Adjustable Constants
```cpp
#define SERVO_OPEN_ANGLE    110   // Servo angle when open
#define SERVO_CLOSE_ANGLE   0     // Servo angle when closed
#define DETECTION_DISTANCE  20    // cm - triggers lid opening
#define MAX_DISTANCE        400   // cm - sensor max range
#define CLOSE_DELAY_MS      5000  // ms - auto-close delay
```

---

## 🚀 Usage

### Initial Setup
1. Power on the ESP32
2. Wait for WiFi connection (serial monitor)
3. Observe "System Loading" on LCD
4. System is ready when LCD shows "Smart Trash!!"

### Normal Operation
1. Approach trash can
2. Lid automatically opens when within 20cm
3. Place trash inside
4. Lid closes after 5 seconds
5. LCD shows current fill capacity
6. Monitor remotely via Blynk app

### Manual Control
- Use the V4 switch in Blynk app to manually open/close lid
- Overrides automatic mode when active

### Alerts
| Capacity | Action |
|----------|--------|
| 85-99% | Warning notification + "Almost Full" display |
| 100% | Critical notification + "Full" display |

---

## 📱 Blynk App Setup

### Virtual Pins
| Pin | Function | Data Type |
|-----|----------|-----------|
| V0  | Object Distance | Integer (cm) |
| V1  | Trash Level Distance | Integer (cm) |
| V2  | Capacity % | Integer (%) |
| V3  | Status Message | String |
| V4  | Manual Control Switch | Integer (0/1) |

### Widget Setup
1. Create new project with "Smart Trash" template
2. Add **Gauge** widget → V0 (Object Distance)
3. Add **Gauge** widget → V1 (Trash Distance)
4. Add **Gauge** widget → V2 (Capacity %)
5. Add **Label** widget → V3 (Status)
6. Add **Switch** widget → V4 (Manual Control)
7. Add **Notification** widget for alerts

---

## 📁 Project Structure

```
AUTOMATIC-TRASHCAN/
├── sketch.ino          # Main ESP32 code
├── README.md           # Project documentation
├── assets/
│   └── pict1.png       # Circuit/image assets
└── flowchart/
    └── system-flowchart.svg  # System flowchart
```

---

## 🔀 Flowchart

### Main System Flowchart

```
┌─────────────────────────────────────────────────────────────────────────┐
│                        SYSTEM START                                      │
└─────────────────────────────────┬───────────────────────────────────────┘
                                  ▼
┌─────────────────────────────────────────────────────────────────────────┐
│                        INITIALIZATION                                    │
│  • Initialize Serial (115200 baud)                                       │
│  • Connect to WiFi & Blynk                                               │
│  • Attach Servo to GPIO 13                                               │
│  • Initialize LCD 16x2                                                   │
│  • Display "System Loading" animation                                   │
│  • Start timer (2 second interval for measurement)                      │
└─────────────────────────────────┬───────────────────────────────────────┘
                                  ▼
                         ┌───────────────┐
                         │  MAIN LOOP    │
                         └───────┬───────┘
          ┌──────────────────────┼──────────────────────┐
          │                      │                      │
          ▼                      ▼                      ▼
   ┌────────────┐        ┌────────────┐        ┌────────────┐
   │ Blynk.run()│        │ timer.run()│        │  (Repeat)  │
   └────────────┘        └────────────┘        └────────────┘
                                  │                      │
                                  ▼                      │
                         ┌──────────────────────────────┘
                         │
                         ▼
               ┌─────────────────────┐
               │ 2-Second Timer Tick │
               └──────────┬──────────┘
                          │
                          ▼
               ┌─────────────────────┐
               │   Measure Distance  │
               │ • Sensor 1: Object  │
               │ • Sensor 2: Trash   │
               └──────────┬──────────┘
                          │
          ┌───────────────┼───────────────┐
          │               │               │
          ▼               ▼               ▼
   ┌───────────┐   ┌───────────┐   ┌───────────────┐
   │Calculate  │   │ Check     │   │ Update LCD    │
   │Capacity   │   │ Threshold │   │ Display       │
   └─────┬─────┘   └─────┬─────┘   └───────┬───────┘
         │               │                 │
         │               │                 │
         ▼               ▼                 ▼
   ┌───────────┐   ┌───────────┐   ┌───────────────┐
   │Send to    │   │ Trigger   │   │ Show:         │
   │Blynk (V2) │   │ Alerts    │   │ - Capacity %  │
   └─────┬─────┘   └─────┬─────┘   │ - Status msg  │
         │               │         └───────────────┘
         │               │
         │               ▼
         │        ┌───────────────┐
         │        │ Trash < 85%?  │
         │        └───────┬───────┘
         │                │
         │        ┌───────┴───────┐
         │        │               │
         ▼        ▼               ▼
   ┌───────────┐           ┌───────────────┐
   │ Update    │           │ Send Blynk    │
   │ Status    │           │ Notifications │
   │ Message   │           └───────────────┘
   │ (V3)      │
   └─────┬─────┘
         │
         ▼
┌─────────────────────────────────────────────────────────────┐
│              OBJECT DETECTION & SERVO CONTROL               │
│                                                             │
│  ┌─────────────────┐                                        │
│  │ Object distance │                                        │
│  │ within 20cm?    │                                        │
│  └────────┬────────┘                                        │
│           │                                                 │
│    ┌──────┴──────┐                                          │
│    │             │                                          │
│    ▼ NO         ▼ YES                                      │
│  ┌─────┐   ┌─────────────────┐                             │
│  │Do   │   │ Is lid already  │                             │
│  │nothing│   │ open?          │                             │
│  └──┬──┘   └────────┬────────┘                             │
│     │        ┌──────┴──────┐                               │
│     │        │             │                               │
│     │        ▼ NO         ▼ YES                            │
│     │    ┌─────┐     ┌──────────┐                          │
│     │    │Open │     │ Do       │                          │
│     │    │Lid  │     │ nothing  │                          │
│     │    │(110°)│     │          │                          │
│     │    └──┬──┘     └──────────┘                          │
│     │       │                                               │
│     │       ▼                                               │
│     │  ┌─────────────────────┐                             │
│     │  │ Set 5-second timer  │                             │
│     │  │ to close lid        │                             │
│     │  └──────────┬──────────┘                             │
│     │             │                                        │
│     │  (After 5 seconds)                                   │
│     │             ▼                                        │
│     │  ┌─────────────────────┐                             │
│     │  │ Close lid (0°)      │                             │
│     │  └─────────────────────┘                             │
│     │                                                     │
│     └─────────────────────────────────────────────────────┘
│                              │
└──────────────────────────────┬──────────────────────────────────────┘
                               │
                               ▼
                    ┌─────────────────────┐
                    │ Send Distance Data  │
                    │ to Blynk (V0, V1)   │
                    └──────────┬──────────┘
                               │
                               ▼
                    ┌─────────────────────┐
                    │ Return to Main Loop │
                    └─────────────────────┘
```

### Manual Control Flow

```
┌─────────────────────────────────────────┐
│     BLYNK_WRITE(V4) - Manual Switch     │
└──────────────────┬──────────────────────┘
                   │
         ┌─────────┴─────────┐
         │                   │
         ▼                   ▼
    Switch = 1          Switch = 0
         │                   │
         ▼                   ▼
   ┌────────────┐      ┌────────────┐
   │ Open Lid   │      │ Close Lid  │
   │ (110°)     │      │ (0°)       │
   │            │      │            │
   │ Set flag:  │      │ Set flag:  │
   │ servoOpen  │      │ servoOpen  │
   │ = true     │      │ = false    │
   └────────────┘      └────────────┘
         │                   │
         └─────────┬─────────┘
                   │
                   ▼
          ┌────────────────┐
          │ Return to      │
          │ Measurement    │
          │ Loop           │
          └────────────────┘
```

---

## 🤝 Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

---

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

---

## 👨‍💻 Author

**Created by:** [Your Name]
**Version:** 1.0.0
**Last Updated:** 2024

---

## 🙏 Acknowledgments

- [Blynk IoT Platform](https://blynk.io/)
- [ESP32 Community](https://www.esp32.com/)
- [Arduino Project Hub](https://create.arduino.cc/projecthub)

---

<div align="center">

**Made with ❤️ for a cleaner world**

🌍♻️🤖

</div>

