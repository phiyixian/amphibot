#ifndef COMPASS_H
#define COMPASS_H

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>

Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);

// Call this once in setup
void initCompass() {
  if (!mag.begin()) {
    Serial.println("Ooops, no HMC5883 detected ... Check your wiring!");
    while (1); // Halt
  }
  Serial.println("Compass initialized");
}

// Call this to get heading in degrees (0–360)
float getHeadingDegrees() {
  sensors_event_t event;
  mag.getEvent(&event);

  float heading = atan2(event.magnetic.y, event.magnetic.x);

  // Set your location's declination angle here (in radians)
  float declinationAngle = 0.22;  // e.g. ~13° west → +0.22 rad
  heading += declinationAngle;

  if (heading < 0) heading += 2 * PI;
  if (heading > 2 * PI) heading -= 2 * PI;

  return heading * 180 / PI;
}

#endif