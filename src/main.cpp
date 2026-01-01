/**
 * Automatic Trash Can - Smart Waste Management System
 * Main C++ source file for ESP32 smart trash can project
 * 
 * This is a refactored C++ version of the Arduino sketch
 * to ensure proper GitHub language detection
 */

#include <ESP32Servo.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <LiquidCrystal_I2C.h>
#include <NewPing.h>
#include "trash_can.h"

// Configuration constants
const char BLYNK_TEMPLATE_ID[] = "TMPL6h4tlercF";
const char BLYNK_TEMPLATE_NAME[] = "Smart Trash";
const char BLYNK_AUTH_TOKEN[] = "RgJ3QtoWBn1czaZcSMcctVcFHNMS_rYp";

// WiFi credentials (modify as needed)
const char WIFI_SSID[] = "Wokwi-GUEST";
const char WIFI_PASS[] = "";

// Pin definitions
const uint8_t TRIGGER_PIN1 = 5;
const uint8_t ECHO_PIN1 = 18;
const uint8_t TRIGGER_PIN2 = 4;
const uint8_t ECHO_PIN2 = 15;
const uint8_t SERVO_PIN = 13;

const unsigned int MAX_DISTANCE = 400;  // cm
const uint8_t MAX_CAPACITY = 100;       // percent

// Global objects
NewPing ultrasonicSensor1;
NewPing ultrasonicSensor2;
Servo servo;
LiquidCrystal_I2C lcd;
BlynkTimer timer;

// System state
bool manualControl = false;
bool trashFullNotified = false;
bool servoOpen = false;

/**
 * Initialize all hardware components
 */
void setup() {
    Serial.begin(115200);
    Serial.println("Starting Automatic Trash Can System...");
    
    // Initialize ultrasonic sensors
    ultrasonicSensor1 = NewPing(TRIGGER_PIN1, ECHO_PIN1, MAX_DISTANCE);
    ultrasonicSensor2 = NewPing(TRIGGER_PIN2, ECHO_PIN2, MAX_DISTANCE);
    
    // Initialize servo
    servo.attach(SERVO_PIN);
    servo.write(0);  // Start with closed lid
    
    // Initialize LCD
    lcd.begin(16, 2);
    lcd.init();
    lcd.backlight();
    
    // Display loading message
    displayLoadingMessage();
    
    // Connect to Blynk
    char auth[] = BLYNK_AUTH_TOKEN;
    Blynk.begin(auth, WIFI_SSID, WIFI_PASS);
    
    // Set up timer for distance measurement (every 2 seconds)
    timer.setInterval(2000L, measureDistance);
    
    Serial.println("System initialization complete!");
}

/**
 * Main program loop
 */
void loop() {
    Blynk.run();
    timer.run();
}

/**
 * Measure distance using ultrasonic sensors
 * This function is called every 2 seconds by the timer
 */
void measureDistance() {
    if (!manualControl) {
        unsigned int distance1 = ultrasonicSensor1.ping_cm();  // Object detection
        unsigned int distance2 = ultrasonicSensor2.ping_cm();  // Trash level
        
        // Print to Serial Monitor
        Serial.print("Object Distance: ");
        Serial.print(distance1);
        Serial.print(" cm, Trash Distance: ");
        Serial.print(distance2);
        Serial.println(" cm");
        
        // Calculate capacity percentage
        uint8_t capacity = calculateCapacity(distance2);
        
        // Send data to Blynk
        Blynk.virtualWrite(V0, distance1);
        Blynk.virtualWrite(V1, distance2);
        Blynk.virtualWrite(V2, capacity);
        
        // Check for object detection and control servo
        handleServoControl(distance1);
        
        // Update LCD and send status
        updateStatus(distance2, capacity);
    }
}

/**
 * Calculate trash capacity as a percentage
 * @param distance Current distance measurement from sensor
 * @return Capacity percentage (0-100)
 */
uint8_t calculateCapacity(unsigned int distance) {
    // Map distance to percentage (16cm = 0%, 2cm = 100%)
    return static_cast<uint8_t>(map(distance, 16, 2, 0, 100));
}

/**
 * Handle servo motor control based on object detection
 * @param distance Distance to the detected object
 */
void handleServoControl(unsigned int distance) {
    if (distance > 0 && distance <= 20) {  // Object within 20cm
        if (!servoOpen) {
            servo.write(110);  // Open lid
            servoOpen = true;
            Serial.println("Lid opened - Object detected");
            
            // Set timer to close lid after 5 seconds
            timer.setTimeout(5000L, []() {
                if (servoOpen) {
                    servo.write(0);  // Close lid
                    servoOpen = false;
                    Serial.println("Lid closed - Auto close timer");
                }
            });
        }
    }
}

/**
 * Update system status on LCD and Blynk
 * @param distance Trash distance measurement
 * @param capacity Current capacity percentage
 */
void updateStatus(unsigned int distance, uint8_t capacity) {
    String status;
    
    if (distance <= 2) {  // Trash is full
        status = "Tong Sampah Penuh";
        Blynk.virtualWrite(V3, status);
        
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Kapasitas: ");
        lcd.print("100%");
        lcd.setCursor(0, 1);
        lcd.print("Sampah Penuh");
        
        // Send notification if not already sent
        if (!trashFullNotified) {
            Blynk.logEvent("message", "Tempat Sampah Penuh! Harap Segera Di Angkut");
            trashFullNotified = true;
        }
    } else if (capacity >= 85) {  // Almost full
        status = "Tempat Sampah Hampir Penuh";
        Blynk.virtualWrite(V3, status);
        
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Kapasitas: ");
        lcd.print(capacity);
        lcd.print("%");
        lcd.setCursor(0, 1);
        lcd.print("Hampir Penuh");
        
        trashFullNotified = false;
    } else if (distance >= 16) {  // Empty
        status = "Tong Sampah Kosong";
        Blynk.virtualWrite(V3, status);
        
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Tempat Sampah");
        lcd.setCursor(0, 1);
        lcd.print("Kosong");
        
        trashFullNotified = false;
    } else {  // Normal operation
        status = "Tempat Sampah Belum Penuh";
        Blynk.virtualWrite(V3, status);
        
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Kapasitas: ");
        lcd.print(capacity);
        lcd.print("%");
        lcd.setCursor(0, 1);
        lcd.print("Smart Trash!!");
        
        trashFullNotified = false;
    }
}

/**
 * Display system loading animation on LCD
 */
void displayLoadingMessage() {
    lcd.setCursor(1, 0);
    lcd.print("System Loading");
    
    for (int i = 0; i <= 15; i++) {
        lcd.setCursor(i, 1);
        lcd.print(".");
        delay(200);
    }
    
    lcd.clear();
}

/**
 * Handle manual control from Blynk app
 * Called when V4 switch state changes
 */
BLYNK_WRITE(V4) {
    int switchValue = param.asInt();
    manualControl = (switchValue == 1);
    
    if (manualControl) {
        servo.write(110);  // Open lid
        servoOpen = true;
        Serial.println("Manual control: Lid opened");
    } else {
        servo.write(0);  // Close lid
        servoOpen = false;
        Serial.println("Manual control: Lid closed");
    }
}

