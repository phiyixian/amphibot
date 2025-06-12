#include "motor_control.h"
#include "communication.h"
#include "brightness.h"
#include "obstacle_detection.h"
#include "compass.h"
#include "motion.h"

void setup() {
  Serial.begin(9600);
  initMotors();
  initCommunication();
  initLightControl();
  initObstacleDetection();
  initCompass(); // Initialize compass
  initMotionSensor(); 
  
  Serial.println("System Initialized");
}

void sendSensorStatusToESP32() {
  // LDR brightness check
  int brightness = analogRead(LDR_PIN);
  String brightnessStatus = (brightness > brightnessThreshold) ? "Dark" : "Bright";

  // Ultrasonic distance
  int distance = calculateDistance();
  String obstacleStatus = (distance > 0 && distance < OBSTACLE_DISTANCE) ? "Obstacle" : "Clear";

  // Compass heading
  float heading = getHeadingDegrees();

  // Motion detection
  String motionStatus = isMotionDetected() ? "Motion" : "NoMotion";

  // Compose message
  String message = "LDR:" + brightnessStatus +
                   ",DIST:" + String(distance) +
                   ",OBS:" + obstacleStatus +
                   ",HEAD:" + String(heading, 1) +
                   ",MOTION:" + motionStatus;

  // Send to ESP32
  Serial.println(message);
}

void loop() {
  if (Serial.available()) {
    char cmd = Serial.read();
    switch (cmd) {
      case 'F': moveForward(); break;
      case 'B': moveBackward(); break;
      case 'L': turnLeft(); break;
      case 'R': turnRight(); break;
      case 'S': stopAll(); break;
    }
  }

  obstacleAvoidanceRoutine();
  checkVoiceAndAlert();
  updateLightBasedOnLDR();
  sendSensorStatusToESP32();  // Send LDR and distance info to ESP32
  delay(100);
}