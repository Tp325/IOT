/*
  Bài 10 - Chớp tắt LED tại GPIO2
  Mô tả: LED sáng tắt theo chu kỳ đều bằng kỹ thuật millis(), không dùng delay().

  Sơ đồ nối dây:
    ESP32 GPIO2 ---- 220Ω ----|>|---- GND
    (LED cathode nối GND, anode nối qua điện trở 220Ω tới GPIO2)
*/

const uint8_t LED_PIN = 2;
const uint32_t BLINK_INTERVAL = 500;  // 500ms

bool ledState = LOW;
uint32_t previousMillis = 0;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, ledState);

  Serial.begin(115200);
  Serial.println("Bai 10: Chop tat LED GPIO2");
}

void loop() {
  uint32_t currentMillis = millis();

  // Chỉ thay đổi trạng thái LED khi đã đủ thời gian theo millis().
  if (currentMillis - previousMillis >= BLINK_INTERVAL) {
    previousMillis = currentMillis;
    ledState = !ledState;
    digitalWrite(LED_PIN, ledState);

    Serial.println(ledState ? "LED ON" : "LED OFF");
  }
}
