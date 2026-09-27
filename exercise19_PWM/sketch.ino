

#define LED_PIN  2
#define FREQ     2000        
#define RESOLUTION 8         
#define FADE_TIME 500       
#define UPDATE_INTERVAL 10   

const uint32_t maxDuty = (1UL << RESOLUTION) - 1;

const float dutyStep = (float)maxDuty / (FADE_TIME / UPDATE_INTERVAL);


float currentDuty = 0;
bool fadingUp = true;       
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

    if (fadingUp) {
      currentDuty += dutyStep;
      if (currentDuty >= maxDuty) {
        currentDuty = maxDuty;
        fadingUp = false;  
      }
    } else {
      currentDuty -= dutyStep;
      if (currentDuty <= 0) {
        currentDuty = 0;
        fadingUp = true;   
      }
    }

    ledcWrite(LED_PIN, (uint32_t)currentDuty);
    Serial.printf("Duty: %lu / %lu | %s\n",
                  (uint32_t)currentDuty, maxDuty,
                  fadingUp ? "UP" : "DOWN");
  }
}