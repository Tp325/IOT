#define LED_PIN  2
#define FREQ     1000        
#define RESOLUTION 10        
#define FADE_TIME 1000      
#define UPDATE_INTERVAL 10   

const uint32_t maxDuty = (1UL << RESOLUTION) - 1;


const float dutyStep = (float)maxDuty / (FADE_TIME / UPDATE_INTERVAL);

float currentDuty = 0;
unsigned long lastUpdate = 0;

void setup() {
  Serial.begin(115200);
  Serial.printf("Resolution: %d bit, maxDuty: %lu, step: %.2f\n",
                RESOLUTION, maxDuty, dutyStep);

  ledcAttach(LED_PIN, FREQ, RESOLUTION);
  ledcWrite(LED_PIN, 0);
}

void loop() {
  unsigned long now = millis();

  if (now - lastUpdate >= UPDATE_INTERVAL) {
    lastUpdate = now;

    currentDuty += dutyStep;

    if (currentDuty >= maxDuty) {
      currentDuty = 0;   // Lặp lại từ đầu
    }

    ledcWrite(LED_PIN, (uint32_t)currentDuty);
    Serial.printf("Duty: %lu / %lu\n", (uint32_t)currentDuty, maxDuty);
  }
}