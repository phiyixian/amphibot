// communication.h

const int micPin = A0;
const int speakerPin = 10;
const int soundThreshold = 300;
const int durationThreshold = 100;

unsigned long voiceStartTime = 0;
bool voiceDetected = false;

void initCommunication() {
  pinMode(speakerPin, OUTPUT);
}

void beepLoud() {
  for (int i = 0; i < 3; i++) {
    tone(speakerPin, 1000);
    delay(300);
    noTone(speakerPin);
    delay(200);
  }
}

void checkVoiceAndAlert() {
  int micValue = analogRead(micPin);
  Serial.println(micValue);

  if (micValue > soundThreshold) {
    if (!voiceDetected) {
      voiceStartTime = millis();
      voiceDetected = true;
    }
  } else {
    if (voiceDetected && millis() - voiceStartTime >= durationThreshold) {
      Serial.println("🔊 Voice detected! Beeping alert!");
      beepLoud();
    }
    voiceDetected = false;
  }

  delay(20);
}


