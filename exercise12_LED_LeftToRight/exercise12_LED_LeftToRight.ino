/*
  Bài 12 - LED chạy trái sang phải
  Mô tả: 4 LED ở GPIO5, 18, 19, 21 sáng lần lượt từ trái sang phải, lặp lại.

  Sơ đồ nối dây:
    GPIO5  ---- 220Ω ----|>|---- GND
    GPIO18 ---- 220Ω ----|>|---- GND
    GPIO19 ---- 220Ω ----|>|---- GND
    GPIO21 ---- 220Ω ----|>|---- GND
*/

const uint8_t LED_PINS[] = {5, 18, 19, 21};
const uint8_t LED_COUNT = sizeof(LED_PINS) / sizeof(LED_PINS[0]);
const uint32_t RUN_INTERVAL = 200;

uint8_t currentIndex = 0;
uint32_t previousMillis = 0;

void setup() {
  for (uint8_t i = 0; i < LED_COUNT; i++) {
    pinMode(LED_PINS[i], OUTPUT);
    digitalWrite(LED_PINS[i], LOW);
  }

  Serial.begin(115200);
  Serial.println("Bai 12: LED chay trai sang phai");
}

void loop() {
  uint32_t currentMillis = millis();

  if (currentMillis - previousMillis >= RUN_INTERVAL) {
    previousMillis = currentMillis;

    // Tắt toàn bộ LED trước khi bật LED tiếp theo.
    for (uint8_t i = 0; i < LED_COUNT; i++) {
      digitalWrite(LED_PINS[i], LOW);
    }

    digitalWrite(LED_PINS[currentIndex], HIGH);
    Serial.print("LED active: ");
    Serial.println(currentIndex);

    currentIndex = (currentIndex + 1) % LED_COUNT;
  }
}
