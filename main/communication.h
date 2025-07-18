// communication.h

const int micPin = A0;
const int speakerPin = 10;

// Tuning parameters
const int sampleCount = 5;        // Number of samples for ambient level
const int suddenSoundGap = ;    // Minimum gap above ambient to consider as sudden sound
const unsigned long debounceTime = 1000; // Time before allowing next detection

unsigned long lastDetectionTime = 0;

void initCommunication() {
  pinMode(speakerPin, OUTPUT);
  Serial.begin(9600);
}

void beepLoud() {
  for (int i = 0; i < 3; i++) {
    tone(speakerPin, 1000);
    delay(300);
    noTone(speakerPin);
    delay(200);
  }
}

// Function to get average ambient sound level
int getAmbientLevel() {
  long sum = 0;
  for (int i = 0; i < sampleCount; i++) {
    sum += analogRead(micPin);
    delay(2);  // Small delay to stabilize readings
  }
  return sum / sampleCount;
}

void checkVoiceAndAlert() {
  int ambientLevel = getAmbientLevel();         // Get average background noise
  int currentLevel = analogRead(micPin);        // Read current mic value
  Serial.print("Ambient: "); Serial.print(ambientLevel);
  Serial.print(" | Current: "); Serial.println(currentLevel);

  if (currentLevel > ambientLevel + suddenSoundGap) {
    unsigned long currentTime = millis();
    if (currentTime - lastDetectionTime > debounceTime) {
      Serial.println("🔊 Sudden sound detected! Beeping alert!");
      beepLoud();
      lastDetectionTime = currentTime;
    }
  }

  delay(50); // Delay to reduce unnecessary looping
}
