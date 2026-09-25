/*
  Bài 16 - Điều khiển một LED bằng hai switch
  Mô tả: nút ON bật LED, nút OFF tắt LED.

  Sơ đồ nối dây:
    ESP32 GPIO2 ---- 220Ω ----|>|---- GND
    ESP32 GPIO4 ---- Nút ON ---- GND
    ESP32 GPIO5 ---- Nút OFF ---- GND
    (Dùng INPUT_PULLUP nên nút nhấn nối giữa chân GPIO và GND)
*/

const uint8_t LED_PIN = 2;
const uint8_t BUTTON_ON_PIN = 4;
const uint8_t BUTTON_OFF_PIN = 5;
const uint32_t DEBOUNCE_MS = 30;

bool ledState = LOW;
bool lastOnRawState = HIGH;
bool lastOffRawState = HIGH;
bool stableOnState = HIGH;
bool stableOffState = HIGH;
uint32_t lastOnDebounceTime = 0;
uint32_t lastOffDebounceTime = 0;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_ON_PIN, INPUT_PULLUP);
  pinMode(BUTTON_OFF_PIN, INPUT_PULLUP);

  digitalWrite(LED_PIN, ledState);

  Serial.begin(115200);
  Serial.println("Bai 16: LED dieu khien bang 2 switch");
}

void loop() {
  bool rawOnState = digitalRead(BUTTON_ON_PIN);
  bool rawOffState = digitalRead(BUTTON_OFF_PIN);

  // Xử lý nút ON.
  if (rawOnState != lastOnRawState) {
    lastOnDebounceTime = millis();
  }

  if ((millis() - lastOnDebounceTime) >= DEBOUNCE_MS) {
    if (stableOnState != rawOnState) {
      stableOnState = rawOnState;

      if (stableOnState == LOW) {
        ledState = HIGH;
        digitalWrite(LED_PIN, ledState);
        Serial.println("LED ON by button ON");
      }
    }
  }
  lastOnRawState = rawOnState;

  // Xử lý nút OFF.
  if (rawOffState != lastOffRawState) {
    lastOffDebounceTime = millis();
  }

  if ((millis() - lastOffDebounceTime) >= DEBOUNCE_MS) {
    if (stableOffState != rawOffState) {
      stableOffState = rawOffState;

      if (stableOffState == LOW) {
        ledState = LOW;
        digitalWrite(LED_PIN, ledState);
        Serial.println("LED OFF by button OFF");
      }
    }
  }
  lastOffRawState = rawOffState;
}
