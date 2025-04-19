# 🩺 Health & Activity Monitor with Firebase & ESP32

A smart health and fitness monitoring system using an **ESP32**, connected to **Firebase Realtime Database**,
that tracks **heart rate**, **steps**, **temperature**, and **water intake**, while also providing **medicine reminders** via a buzzer alarm.

## 🚀 Features

- ✅ **Heart Rate Monitoring** using a Pulse Sensor.
- 👣 **Step Counter** with MPU6050 accelerometer.
- 🌡️ **Temperature Monitoring** via MPU6050 sensor.
- ⏰ **Real-time Clock** sync using NTP server.
- 🔔 **Medicine Reminder** system controlled from Firebase.
- 💧 **Water Intake Tracking** with a button input.
- ☁️ **Cloud Integration** using Firebase Realtime Database.

## 🔧 Hardware Required

- ESP32 Dev Board
- Pulse Sensor (connected to GPIO 34)
- MPU6050 Accelerometer/Gyroscope
- Push Button (connected to GPIO 2)
- Buzzer (connected to GPIO 12)
- Jumper wires, breadboard, etc.

## 📦 Libraries Used

- [WiFi.h](https://www.arduino.cc/en/Reference/WiFi)
- [Firebase ESP Client](https://github.com/mobizt/Firebase-ESP-Client)
- [NTPClient](https://github.com/arduino-libraries/NTPClient)
- [PulseSensor Playground](https://github.com/WorldFamousElectronics/PulseSensorPlayground)
- [MPU6050](https://github.com/jrowberg/i2cdevlib)

## 🔌 Pin Configuration

| Component        | Pin        |
|------------------|------------|
| Pulse Sensor     | GPIO 34    |
| On-board LED     | GPIO 13    |
| Buzzer           | GPIO 12    |
| Button           | GPIO 2     |
| MPU6050          | I2C (SCL/SDA) |

## ☁️ Firebase Setup

1. Go to [Firebase Console](https://console.firebase.google.com/), create a new project.
2. Add a Realtime Database and set the rules to public (for testing) or secure accordingly.
3. Copy your **Database URL** and **Web API Key**.
4. Paste them in the code:
   ```cpp
   #define API_KEY "your_firebase_web_api_key"
   #define DATABASE_URL "your_database_url"
   ```

## 🧠 How It Works

- ESP32 connects to Wi-Fi and synchronizes time using an NTP server.
- Heartbeats are detected and BPM is calculated using the PulseSensor library.
- Steps are roughly estimated based on Y-axis spikes from the MPU6050.
- The buzzer is triggered remotely via Firebase for medication reminders.
- Button presses simulate drinking a glass of water and update Firebase accordingly.
- All data (BPM, steps, temperature, time) is pushed to Firebase in real-time.

## 📈 Firebase Database Structure (Example)

```json
{
  "heart": 78,
  "Steps": 120,
  "Temperature": 36.7,
  "currentTime": "11:07:30",
  "medAlarm": false,
  "recieve": true,
  "waterSystem": {
    "total": 8,
    "pbGlasses": 3
  }
}
```

## 📷 Demo (Optional)

*Include photos or videos of your setup here.*

## 📌 TODO

- Improve step detection accuracy
- Add OLED display support
- Replace delay-based buzzer alerts with non-blocking code
- Add mobile app interface using MIT App Inventor / Flutter

## 📄 License

This project is licensed under the MIT License. Feel free to use and modify it!

---

### Made with ❤️ by Hadjer Salhi
