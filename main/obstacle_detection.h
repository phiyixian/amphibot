#ifndef OBSTACLE_DETECTION_H
#define OBSTACLE_DETECTION_H

#include <Servo.h>

const int trigPin = 9;//blue
const int echoPin = 8;//green
const int servoPin = 11;
const int OBSTACLE_DISTANCE = 10;

Servo myServo;

void initObstacleDetection() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  myServo.attach(servoPin);
}

int calculateDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH, 30000);
  if (duration == 0) return -1;
  return duration * 0.034 / 2;
}

void obstacleAvoidanceRoutine() {
  bool obstacleLeft = false, obstacleCenter = false, obstacleRight = false;

  for (int angle = 15; angle <= 165; angle += 15) {
    myServo.write(angle);
    delay(50);
    int distance = calculateDistance();
    if (distance > 0 && distance < OBSTACLE_DISTANCE) {
      if (angle >= 15 && angle <= 60) obstacleRight = true;
      else if (angle >= 75 && angle <= 105) obstacleCenter = true;
      else if (angle >= 120 && angle <= 165) obstacleLeft = true;
    }
  }

  if (obstacleLeft && obstacleRight && obstacleCenter) {
    stopAll();
    delay(300);
    moveBackward();
    delay(1000);
    stopAll();
    turnRight();
    delay(800);
    stopAll();
  } else if (obstacleRight) {
    stopAll();
    delay(300);
    turnLeft();
    delay(800);
    stopAll();
  } else if (obstacleLeft) {
    stopAll();
    delay(300);
    turnRight();
    delay(800);
    stopAll();
  } else if (obstacleCenter) {
    stopAll();
    delay(300);
    moveBackward();
    delay(1000);
    stopAll();
  }

  delay(300);
}

#endif