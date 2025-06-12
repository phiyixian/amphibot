// light_control.h

#define LDR_PIN A2       // Analog input from LDR sensor
#define LED_STRIP A1     // Output to MOSFET controlling LED strip

// Tune this based on your environment (e.g. indoor ~500–700, outdoor ~1000+)
const int brightnessThreshold = 1000;

void initLightControl() {
  pinMode(LED_STRIP, OUTPUT);
  digitalWrite(LED_STRIP, LOW); // Start with LEDs off
}

void updateLightBasedOnLDR() {
  int brightness = analogRead(LDR_PIN);
  Serial.print("LDR Value: ");
  Serial.println(brightness);

  if (brightness > brightnessThreshold) {
    digitalWrite(LED_STRIP, HIGH);  // Turn on LED strip (dark)
  } else {
    digitalWrite(LED_STRIP, LOW);   // Turn off LED strip (bright)
  }
}
