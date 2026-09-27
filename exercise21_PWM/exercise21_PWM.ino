/*
 * BÀI 21 - ĐIỀU KHIỂN LED BẰNG PWM VÀ HAI SWITCH
 * Board: ESP32 DevKit V1
 * Core: Arduino-ESP32 Core 3.x
 */

// -----------------------------------------------------
// SƠ ĐỒ CHÂN ĐẤU NỐI (WIRING DIAGRAM)
// -----------------------------------------------------
// 
//        ESP32 DevKit V1
//      +-----------------+
//      |                 |
//      |          GPIO2  |----> [Điện trở 220Ω] ----> (+ LED -) ----> GND
//      |                 |
//      |          GPIO4  |----> [Switch UP] ------------------------> GND
//      |                 |
//      |          GPIO5  |----> [Switch DOWN] ----------------------> GND
//      |                 |
//      |          GND    |------------------------------------------> GND
//      +-----------------+
//
// * Ghi chú: 
//   - Hai switch sử dụng điện trở kéo lên nội bộ (INPUT_PULLUP) của ESP32.
//   - Do đó không cần dùng thêm điện trở ngoài, chỉ cần đấu 1 đầu của switch 
//     vào GPIO, đầu còn lại đấu thẳng xuống GND.
// -----------------------------------------------------

// -----------------------------------------------------
// PIN CONFIGURATION
// -----------------------------------------------------
const int LED_PIN = 2;        // LED tích hợp hoặc LED ngoài nối vào GPIO2
const int BUTTON_UP = 4;      // Switch tăng Duty (Kéo xuống GND)
const int BUTTON_DOWN = 5;    // Switch giảm Duty (Kéo xuống GND)

// -----------------------------------------------------
// PWM CONFIGURATION
// -----------------------------------------------------
const int PWM_FREQ = 10000;   // Tần số PWM: 10 kHz
const int PWM_RESOLUTION = 8; // Độ phân giải 8 bit (0 - 255)

// -----------------------------------------------------
// BUTTON CONFIGURATION
// -----------------------------------------------------
const unsigned long DEBOUNCE_TIME = 50; // Thời gian chống dội phím: 50ms

// -----------------------------------------------------
// GLOBAL VARIABLES
// -----------------------------------------------------
// Biến lưu duty cycle, dùng kiểu int (có dấu) để tránh lỗi underflow khi tính toán
int duty = 0; 

// Biến trạng thái và thời gian cho Switch UP
int upButtonState = HIGH;
int upLastButtonState = HIGH;
unsigned long upLastDebounceTime = 0;

// Biến trạng thái và thời gian cho Switch DOWN
int downButtonState = HIGH;
int downLastButtonState = HIGH;
unsigned long downLastDebounceTime = 0;

// -----------------------------------------------------
// FUNCTIONS
// -----------------------------------------------------

// Hàm cập nhật PWM và hiển thị thông tin ra Serial Monitor
void updatePWMAndSerial(const char* buttonName) {
  // Cập nhật tín hiệu PWM cho LED (API Core 3.x)
  ledcWrite(LED_PIN, duty);
  
  // Tính toán phần trăm
  float percent = (duty / 255.0) * 100.0;
  
  // Hiển thị ra Serial Monitor
  Serial.print(buttonName);
  Serial.print(" pressed | Duty = ");
  Serial.print(duty);
  Serial.print(" / 255 = ");
  Serial.print(percent);
  Serial.println("%");
}

// -----------------------------------------------------
// SETUP
// -----------------------------------------------------
void setup() {
  Serial.begin(115200);
  
  // Cấu hình hai switch dùng điện trở kéo lên nội bộ (nhấn = LOW, nhả = HIGH)
  pinMode(BUTTON_UP, INPUT_PULLUP);
  pinMode(BUTTON_DOWN, INPUT_PULLUP);
  
  // Cấu hình PWM cho LED GPIO2 (Sử dụng API mới của ESP32 Core 3.x)
  ledcAttach(LED_PIN, PWM_FREQ, PWM_RESOLUTION);
  
  // Đặt duty ban đầu bằng 0 (LED tắt)
  ledcWrite(LED_PIN, duty);
  
  Serial.println("=========================================");
  Serial.println("   ESP32 PWM LED CONTROL - CORE 3.X");
  Serial.println("   PWM: 10kHz | Resolution: 8-bit");
  Serial.println("   LED: GPIO2 | UP: GPIO4 | DOWN: GPIO5");
  Serial.println("=========================================");
}

// -----------------------------------------------------
// LOOP
// -----------------------------------------------------
void loop() {
  // 1. XỬ LÝ SWITCH UP
  int readingUp = digitalRead(BUTTON_UP);
  
  // Nếu trạng thái thay đổi (do nhấn hoặc nhiễu), reset lại bộ đếm thời gian
  if (readingUp != upLastButtonState) {
    upLastDebounceTime = millis();
  }
  
  // Nếu trạng thái đã ổn định qua khoảng thời gian DEBOUNCE_TIME
  if ((millis() - upLastDebounceTime) > DEBOUNCE_TIME) {
    // Nếu trạng thái thực sự thay đổi so với trạng thái lưu trữ
    if (readingUp != upButtonState) {
      upButtonState = readingUp;
      
      // Chỉ thực hiện tăng khi phát hiện sườn âm (vừa được nhấn xuống LOW)
      if (upButtonState == LOW) {
        duty += 2;
        // Giới hạn cận trên
        if (duty > 255) {
          duty = 255;
        }
        updatePWMAndSerial("UP");
      }
    }
  }
  upLastButtonState = readingUp; // Lưu trạng thái cho vòng lặp sau

  // 2. XỬ LÝ SWITCH DOWN
  int readingDown = digitalRead(BUTTON_DOWN);
  
  if (readingDown != downLastButtonState) {
    downLastDebounceTime = millis();
  }
  
  if ((millis() - downLastDebounceTime) > DEBOUNCE_TIME) {
    if (readingDown != downButtonState) {
      downButtonState = readingDown;
      
      // Chỉ thực hiện giảm khi phát hiện sườn âm (vừa được nhấn xuống LOW)
      if (downButtonState == LOW) {
        duty -= 2;
        // Giới hạn cận dưới
        if (duty < 0) {
          duty = 0;
        }
        updatePWMAndSerial("DOWN");
      }
    }
  }
  downLastButtonState = readingDown; // Lưu trạng thái cho vòng lặp sau
}