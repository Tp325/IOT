/*
  Bài 15 - Điều khiển một LED bằng một switch
  Mô tả: nhấn nút 1 lần sẽ đảo trạng thái LED ON/OFF.

  Sơ đồ nối dây:
    ESP32 GPIO2 ---- 220Ω ----|>|---- GND
    ESP32 GPIO4 ---- Nút nhấn ---- GND
    (Dùng INPUT_PULLUP nên nút nhấn được nối giữa GPIO4 và GND)
*/

const uint8_t LED_PIN = 2;
const uint8_t BUTTON_PIN = 4;
const uint32_t DEBOUNCE_MS = 30;     // thời gian chống dội nút nhấn

bool ledState = LOW;
bool lastRawButtonState = HIGH;       // trạng thái thô, INPUT_PULLUP => nhả nút = HIGH
bool stableButtonState = HIGH;
uint32_t lastDebounceTime = 0;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  digitalWrite(LED_PIN, ledState);

  Serial.begin(115200);
  Serial.println("Bai 15: LED dieu khien bang 1 switch");
}

void loop() {
  bool rawButtonState = digitalRead(BUTTON_PIN);

  // Phát hiện thay đổi trạng thái thô của nút để bắt đầu đếm thời gian debounce.
  if (rawButtonState != lastRawButtonState) {
    lastDebounceTime = millis();
  }

  // Sau khi đã qua khoảng thời gian debounce, xác nhận trạng thái ổn định.
  if ((millis() - lastDebounceTime) >= DEBOUNCE_MS) {
    if (stableButtonState != rawButtonState) {
      stableButtonState = rawButtonState;

      // Với INPUT_PULLUP, nút bấm xuống sẽ đọc LOW.
      if (stableButtonState == LOW) {
        ledState = !ledState;
        digitalWrite(LED_PIN, ledState);
        Serial.println(ledState ? "LED ON" : "LED OFF");
      }
    }
  }

  lastRawButtonState = rawButtonState;
}
