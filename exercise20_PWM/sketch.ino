#define LED_PIN    2
#define SWITCH_PIN 0

#define FREQ       5000
#define RESOLUTION 7
const uint32_t maxDuty = (1UL << RESOLUTION) - 1;

#define SATURATE_MODE true
#define DEBOUNCE_TIME 200

uint32_t duty = 0;
bool lastSwitchState = HIGH;
unsigned long lastDebounceTime = 0;

void setup() {
  Serial.begin(115200);
  pinMode(SWITCH_PIN, INPUT_PULLUP);
  ledcAttach(LED_PIN, FREQ, RESOLUTION);
  ledcWrite(LED_PIN, duty);
  Serial.println("San sang! Nhan nut de tang duty.");
}

void loop() {
  bool currentSwitchState = digitalRead(SWITCH_PIN);

  if (currentSwitchState == LOW && lastSwitchState == HIGH) {
    if (millis() - lastDebounceTime >= DEBOUNCE_TIME) {
      lastDebounceTime = millis();
      duty++;

      if (SATURATE_MODE) {
        if (duty > maxDuty) duty = maxDuty;
      } else {
        if (duty > maxDuty) duty = 0;
      }

      ledcWrite(LED_PIN, duty);
      Serial.printf("Duty: %lu / %lu\n", duty, maxDuty);
    }
  }

  lastSwitchState = currentSwitchState;
}