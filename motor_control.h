// motor_control.h

// Motor 1 (Front Left)
#define M1_IN1 7
#define M1_IN2 6

// Motor 2 (Front Right)
#define M2_IN1 3
#define M2_IN2 2

// Motor 3 (Rear Left)
#define M3_IN1 5
#define M3_IN2 4

// Motor 4 (Rear Right)
#define M4_IN1 13
#define M4_IN2 12

void initMotors() {
  pinMode(M1_IN1, OUTPUT); pinMode(M1_IN2, OUTPUT);
  pinMode(M2_IN1, OUTPUT); pinMode(M2_IN2, OUTPUT);
  pinMode(M3_IN1, OUTPUT); pinMode(M3_IN2, OUTPUT);
  pinMode(M4_IN1, OUTPUT); pinMode(M4_IN2, OUTPUT);
}

// Utility function: set motor direction
void setMotor(int in1, int in2, bool forward) {
  digitalWrite(in1, forward ? HIGH : LOW);
  digitalWrite(in2, forward ? LOW : HIGH);
}

void moveForward() {
  setMotor(M1_IN1, M1_IN2, true);
  setMotor(M2_IN1, M2_IN2, true);
  setMotor(M3_IN1, M3_IN2, true);
  setMotor(M4_IN1, M4_IN2, true);
}

void moveBackward() {
  setMotor(M1_IN1, M1_IN2, false);
  setMotor(M2_IN1, M2_IN2, false);
  setMotor(M3_IN1, M3_IN2, false);
  setMotor(M4_IN1, M4_IN2, false);
}

void turnLeft() {
  setMotor(M1_IN1, M1_IN2, false);
  setMotor(M3_IN1, M3_IN2, false);
  setMotor(M2_IN1, M2_IN2, true);
  setMotor(M4_IN1, M4_IN2, true);
}

void turnRight() {
  setMotor(M1_IN1, M1_IN2, true);
  setMotor(M3_IN1, M3_IN2, true);
  setMotor(M2_IN1, M2_IN2, false);
  setMotor(M4_IN1, M4_IN2, false);
}

void stopAll() {
  digitalWrite(M1_IN1, LOW); digitalWrite(M1_IN2, LOW);
  digitalWrite(M2_IN1, LOW); digitalWrite(M2_IN2, LOW);
  digitalWrite(M3_IN1, LOW); digitalWrite(M3_IN2, LOW);
  digitalWrite(M4_IN1, LOW); digitalWrite(M4_IN2, LOW);
}
