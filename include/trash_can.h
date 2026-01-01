/**
 * Automatic Trash Can - Smart Waste Management System
 * Header file for ESP32 smart trash can project
 */

#ifndef TRASH_CAN_H
#define TRASH_CAN_H

#include <ESP32Servo.h>
#include <NewPing.h>
#include <LiquidCrystal_I2C.h>
#include <BlynkSimpleEsp32.h>

// Virtual pins for Blynk
const int V0 = 0;  // Object distance
const int V1 = 1;  // Trash level distance
const int V2 = 2;  // Capacity percentage
const int V3 = 3;  // Status message
const int V4 = 4;  // Manual control switch

// System constants
const unsigned int MAX_DISTANCE_CM = 400;
const uint8_t MAX_CAPACITY_PERCENT = 100;
const uint8_t SERVO_OPEN_ANGLE = 110;
const uint8_t SERVO_CLOSE_ANGLE = 0;
const uint8_t DETECTION_THRESHOLD_CM = 20;
const unsigned long AUTO_CLOSE_DELAY_MS = 5000L;
const unsigned long MEASUREMENT_INTERVAL_MS = 2000L;

/**
 * TrashCan class - Main system controller
 */
class TrashCan {
private:
    NewPing* ultrasonicSensor1;
    NewPing* ultrasonicSensor2;
    Servo* servo;
    LiquidCrystal_I2C* lcd;
    
    bool manualControl;
    bool trashFullNotified;
    bool servoOpen;
    
public:
    /**
     * Constructor
     */
    TrashCan(NewPing* sensor1, NewPing* sensor2, Servo* servoMotor, LiquidCrystal_I2C* display);
    
    /**
     * Initialize the trash can system
     */
    void begin();
    
    /**
     * Main measurement loop - call this periodically
     */
    void measureAndUpdate();
    
    /**
     * Calculate capacity from distance measurement
     */
    uint8_t calculateCapacity(unsigned int distance);
    
    /**
     * Handle servo control based on object detection
     */
    void handleServoControl(unsigned int distance);
    
    /**
     * Update status display on LCD and Blynk
     */
    void updateStatus(unsigned int distance, uint8_t capacity);
    
    /**
     * Set manual control mode
     */
    void setManualControl(bool enabled);
    
    /**
     * Check if in manual control mode
     */
    bool isManualControlEnabled() const;
    
    /**
     * Open the trash can lid
     */
    void openLid();
    
    /**
     * Close the trash can lid
     */
    void closeLid();
};

/**
 * Global system instance
 */
extern TrashCan system;

#endif // TRASH_CAN_H

