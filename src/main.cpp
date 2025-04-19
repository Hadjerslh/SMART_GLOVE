#include <Arduino.h>
#include <WiFi.h>
#include <NTPClient.h>
#include <PulseSensorPlayground.h> 
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

#include <Wire.h>
#include <MPU6050.h>
#include <I2Cdev.h>

#define WIFI_SSID "realme 10"
#define WIFI_PASSWORD "hadjer1234"

#define API_KEY "AIzaSyDx3yxXpJ0hmZpuS2mBhqJ9TQvXJ7-NeMo"
#define DATABASE_URL "https://help-f62ac-default-rtdb.europe-west1.firebasedatabase.app"
#define BUZZER_PIN 12

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
bool signupOK = false;

const long gmtOffset_sec = 3600; // GMT offset in seconds (1 hour)
const int daylightOffset_sec = 0;

const int PulseWire = 34;       // Signal pin connected to A0
const int LED13 = 13;           // On-board LED
int Threshold = 550;   


WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", gmtOffset_sec, daylightOffset_sec);
PulseSensorPlayground pulseSensor;         // Determine signal threshold
MPU6050 mpu;


int stepCount = 0;
int lastAccelY = 0;
const int buttonPin = 2; // GPIO2

int counter = 0;
bool lastButtonState = HIGH; // Assume button is not pressed initially
bool buttonState; // Current state of the button
unsigned long lastDebounceTime = 0; // Time of last button state change
unsigned long debounceDelay = 50; 

void setup() {
  Serial.begin(9600);
  Wire.begin();
  mpu.initialize();
  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("Authentication OK");
    signupOK = true;
  } else {
    Serial.printf("Authentication failed: %s\n", config.signer.signupError.message.c_str());
  }

  config.token_status_callback = tokenStatusCallback;
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
  
  pulseSensor.analogInput(PulseWire);
  pulseSensor.blinkOnPulse(LED13);
  pulseSensor.setThreshold(Threshold);

  if (pulseSensor.begin()) {
    Serial.println("PulseSensor object created!");
  }
   timeClient.begin();
  timeClient.update();
   pinMode(buttonPin, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT); 
}

void loop() {
   timeClient.update();

  // Get current time
  int currentHour = timeClient.getHours();
  int currentMinute = timeClient.getMinutes();
  int currentSecond = timeClient.getSeconds();
  bool alarm ;
      if(Firebase.RTDB.getBool(&fbdo, "medAlarm")) {
        alarm = fbdo.intData();
        Serial.println(alarm);
        if(alarm == true){ 
     digitalWrite(BUZZER_PIN, HIGH);
     delay(1000); // Wait for one second

  // Turn the buzzer off for one second
     digitalWrite(BUZZER_PIN, LOW);
     delay(1000);
     digitalWrite(BUZZER_PIN, HIGH);
     delay(1000);
     alarm = false ;
     
     Firebase.RTDB.setBool(&fbdo, "medAlarm", alarm) ;
    }
      }
  // Check if it's time to take medicine (between 11:07 and 11:08)
  //if (!medicineTaken && currentHour == 11 && currentMinute >= 7 && currentMinute <= 8) {
    //Serial.println("It's time to take medicine!");
    // Take medicine here...
   // Firebase.RTDB.setBool(&fbdo, "medicineTaken", true); // Set medicine taken flag to true in Firebase
    //medicineTaken = true; // Set medicineTaken flag to true locally
     

  String currentTime = String(currentHour) + ":" + String(currentMinute)+":" + String(currentSecond);
  Firebase.RTDB.setString(&fbdo, "currentTime", currentTime);

  int myBPM = pulseSensor.getBeatsPerMinute();
  int16_t accelX, accelY, accelZ;
  mpu.getAcceleration(&accelX, &accelY, &accelZ);
  float temperature = mpu.getTemperature() / 340.0 + 36.53;

  if (pulseSensor.sawStartOfBeat()) {
    Serial.println("HeartBeat Detected!");
    Serial.print("BPM: ");
    Serial.println(myBPM);  
    Firebase.RTDB.setInt(&fbdo, "heart", myBPM); 
    delay(20);
  }

  if (accelY > lastAccelY && accelY > 5000) {
    stepCount++;
  }

  lastAccelY = accelY;

  Serial.print("Steps: ");
  Serial.println(stepCount);
  Serial.print("temperature: ");
  Serial.println(temperature);

  delay(1000);
  bool recieve ;
  if(Firebase.RTDB.getBool(&fbdo, "recieve")) {
        recieve = fbdo.intData();
        Serial.println(recieve); }
  // Send BPM, step count, and temperature to Firebase
  if(recieve == true) {
       Firebase.RTDB.setInt(&fbdo,"heart", myBPM);
  Firebase.RTDB.setInt(&fbdo,"Steps", stepCount);
  Firebase.RTDB.setFloat(&fbdo,"Temperature", temperature);
    
  }
 

    // Check if the button state has changed
   
   
      int buttonState = digitalRead(buttonPin);
      int total ;
      if(Firebase.RTDB.getInt(&fbdo, "waterSystem/total")) {
        total = fbdo.intData();
        Serial.println(total);
      }
    // Check if the button state is LOW (button pressed)
    if (buttonState == LOW) {
                // Increment the counter only when the button is pressed
                counter++;
                // Print the updated counter value
                Serial.print("Counter: ");
                Serial.println(counter);
                 if (counter == total) {
            // Activate the buzzer for one second
            digitalWrite(BUZZER_PIN, HIGH); // Turn on the buzzer
            delay(1000);                     // Keep the buzzer on for one second
            digitalWrite(BUZZER_PIN, LOW);  // Turn off the buzzer
        }
     
        // Delay to debounce the button press (optional)
        delay(100);
                Firebase.RTDB.setInt(&fbdo, "waterSystem/pbGlasses", counter);

              
            }
        }