/*
  Bài 11 - Chớp tắt 4 LED tại GPIO5, 18, 19, 21
  Mô tả: tất cả 4 LED sáng/tắt đồng thời theo chu kỳ 500ms.

  Sơ đồ nối dây:
    GPIO5  ---- 220Ω ----|>|---- GND
    GPIO18 ---- 220Ω ----|>|---- GND
    GPIO19 ---- 220Ω ----|>|---- GND
    GPIO21 ---- 220Ω ----|>|---- GND
*/

const uint8_t LED_PINS[] = {5, 18, 19, 21};
const uint8_t LED_COUNT = sizeof(LED_PINS) / sizeof(LED_PINS[0]);
const uint32_t BLINK_INTERVAL = 500;

bool allLedsState = LOW;
uint32_t previousMillis = 0;

void setup() {
  for (uint8_t i = 0; i < LED_COUNT; i++) {
    pinMode(LED_PINS[i], OUTPUT);
    digitalWrite(LED_PINS[i], LOW);
  }

  Serial.begin(115200);
  Serial.println("Bai 11: Chop tat 4 LED");
}

void loop() {
  uint32_t currentMillis = millis();

  if (currentMillis - previousMillis >= BLINK_INTERVAL) {
    previousMillis = currentMillis;
    allLedsState = !allLedsState;

    for (uint8_t i = 0; i < LED_COUNT; i++) {
      digitalWrite(LED_PINS[i], allLedsState);
    }

    Serial.println(allLedsState ? "4 LED ON" : "4 LED OFF");
  }
}
