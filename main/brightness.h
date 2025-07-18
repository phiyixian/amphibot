// brightness.h

#define LDR_PIN A2  // Analog input from LDR sensor
const int brightnessThreshold = 900;

void initLightControl() {
  // No pin setup needed since we're only reading LDR
  Serial.begin(9600);  // Ensure serial communication is started
}

void printLDRValue() {
  int brightness = analogRead(LDR_PIN);
  Serial.print("LDR Value: ");
  Serial.println(brightness);
}