/*
  Bài 17 - Đếm số lần nhấn phím và in số đếm lên Serial Monitor
  Mô tả: mỗi lần nút được nhấn xuống và xác nhận ổn định, số đếm tăng lên 1.

  Sơ đồ nối dây:
    ESP32 GPIO2 ---- 220Ω ----|>|---- GND
    ESP32 GPIO4 ---- Nút nhấn ---- GND
    (Dùng INPUT_PULLUP nên nút nhấn nối giữa GPIO4 và GND)
*/

const uint8_t LED_PIN = 2;
const uint8_t BUTTON_PIN = 4;
const uint32_t DEBOUNCE_MS = 30;

bool ledState = LOW;
bool lastRawButtonState = HIGH;
bool stableButtonState = HIGH;
uint32_t lastDebounceTime = 0;
uint32_t pressCount = 0;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  digitalWrite(LED_PIN, ledState);

  Serial.begin(115200);
  Serial.println("Bai 17: Dem so lan nhan phim");
  Serial.println("Press count: 0");
}

void loop() {
  bool rawButtonState = digitalRead(BUTTON_PIN);

  if (rawButtonState != lastRawButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) >= DEBOUNCE_MS) {
    if (stableButtonState != rawButtonState) {
      stableButtonState = rawButtonState;

      // INPUT_PULLUP => trạng thái nhấn = LOW
      if (stableButtonState == LOW) {
        pressCount++;
        ledState = !ledState;
        digitalWrite(LED_PIN, ledState);

        Serial.print("Press count: ");
        Serial.println(pressCount);
      }
    }
  }

  lastRawButtonState = rawButtonState;
}
