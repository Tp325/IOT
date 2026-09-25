/*
  Bài 14 - Thực hiện lần lượt bài 11, 12, 13; mỗi mẫu lặp 5 lần
  Mô tả: chuỗi mẫu được chạy theo từng trạng thái và quay lại lặp lại.

  Sơ đồ nối dây:
    GPIO5  ---- 220Ω ----|>|---- GND
    GPIO18 ---- 220Ω ----|>|---- GND
    GPIO19 ---- 220Ω ----|>|---- GND
    GPIO21 ---- 220Ω ----|>|---- GND
*/

const uint8_t LED_PINS[] = {5, 18, 19, 21};
const uint8_t LED_COUNT = sizeof(LED_PINS) / sizeof(LED_PINS[0]);
const uint8_t TOTAL_REPEATS = 5;
const uint32_t BLINK_INTERVAL = 250;
const uint32_t RUN_INTERVAL = 200;

enum PatternMode {
  MODE_BLINK_ALL = 0,
  MODE_LEFT_TO_RIGHT = 1,
  MODE_RIGHT_TO_LEFT = 2
};

PatternMode currentMode = MODE_BLINK_ALL;
uint8_t repeatCount = 0;
uint8_t currentIndex = 0;
bool ledState = LOW;
uint32_t previousMillis = 0;

void clearLeds() {
  for (uint8_t i = 0; i < LED_COUNT; i++) {
    digitalWrite(LED_PINS[i], LOW);
  }
}

void advanceMode() {
  currentMode = static_cast<PatternMode>((currentMode + 1) % 3);
  repeatCount = 0;
  currentIndex = 0;
  ledState = LOW;
  clearLeds();

  Serial.print("Chuyen mode: ");
  Serial.println(currentMode);
}

void setup() {
  for (uint8_t i = 0; i < LED_COUNT; i++) {
    pinMode(LED_PINS[i], OUTPUT);
    digitalWrite(LED_PINS[i], LOW);
  }

  Serial.begin(115200);
  Serial.println("Bai 14: Chay lan luot 3 mau, moi mau 5 lan");
}

void loop() {
  uint32_t currentMillis = millis();

  if (currentMode == MODE_BLINK_ALL) {
    if (currentMillis - previousMillis >= BLINK_INTERVAL) {
      previousMillis = currentMillis;
      ledState = !ledState;

      for (uint8_t i = 0; i < LED_COUNT; i++) {
        digitalWrite(LED_PINS[i], ledState);
      }

      if (!ledState) {
        repeatCount++;
        Serial.print("Blink repeat: ");
        Serial.println(repeatCount);
      }

      if (repeatCount >= TOTAL_REPEATS) {
        advanceMode();
      }
    }
  }

  if (currentMode == MODE_LEFT_TO_RIGHT) {
    if (currentMillis - previousMillis >= RUN_INTERVAL) {
      previousMillis = currentMillis;
      clearLeds();
      digitalWrite(LED_PINS[currentIndex], HIGH);
      Serial.print("Left-to-right index: ");
      Serial.println(currentIndex);

      currentIndex++;
      if (currentIndex >= LED_COUNT) {
        currentIndex = 0;
        repeatCount++;
        Serial.print("Left-to-right repeat: ");
        Serial.println(repeatCount);
      }

      if (repeatCount >= TOTAL_REPEATS) {
        advanceMode();
      }
    }
  }

  if (currentMode == MODE_RIGHT_TO_LEFT) {
    if (currentMillis - previousMillis >= RUN_INTERVAL) {
      previousMillis = currentMillis;
      clearLeds();
      digitalWrite(LED_PINS[currentIndex], HIGH);
      Serial.print("Right-to-left index: ");
      Serial.println(currentIndex);

      if (currentIndex == 0) {
        currentIndex = LED_COUNT - 1;
        repeatCount++;
        Serial.print("Right-to-left repeat: ");
        Serial.println(repeatCount);
      } else {
        currentIndex--;
      }

      if (repeatCount >= TOTAL_REPEATS) {
        advanceMode();
      }
    }
  }
}
