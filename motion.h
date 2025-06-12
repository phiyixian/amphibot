#ifndef MOTION_SENSOR_H
#define MOTION_SENSOR_H

#include <Arduino.h>

#define PIR_PIN A5  // Adjust this to your actual wiring

// Initialize the motion sensor pin
void initMotionSensor() {
  pinMode(PIR_PIN, INPUT);
}

// Check if motion is detected
bool isMotionDetected() {
  return digitalRead(PIR_PIN) == HIGH;
}

#endif