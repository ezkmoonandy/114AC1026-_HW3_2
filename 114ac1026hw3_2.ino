// 腳位定義
const int ledPins[] = {2, 3, 4}; // 3顆LED分別接在 D2, D3, D4
const int numLeds = 3;           // LED數量
const int buttonPin = 7;         // 按鈕接在 D7

// 狀態與按鈕控制變數
int mode = 0;                    // 當前模式 (0:衡亮, 1:慢閃, 2:中閃, 3:快閃)
int lastButtonState = HIGH;      // 上一次讀取到的按鈕狀態 (假設使用上拉電阻，預設為HIGH)
unsigned long lastDebounceTime = 0;  // 上一次按鈕防彈跳時間
const unsigned long debounceDelay = 50; // 防彈跳延遲 (毫秒)

// 閃爍計時變數 (使用 non-blocking millis()，確保按鈕回應敏捷)
unsigned long lastBlinkTime = 0;
bool ledState = HIGH;

void setup() {
  // 設定 LED 腳位為輸出
  for (int i = 0; i < numLeds; i++) {
    pinMode(ledPins[i], OUTPUT);
  }
  
  // 設定按鈕腳位為輸入（開啟內部上拉電阻 INPUT_PULLUP）
  // 若您的按鈕模組已經自帶下拉/上拉電阻，改為 pinMode(buttonPin, INPUT); 即可
  pinMode(buttonPin, INPUT_PULLUP);
}

void loop() {
  // 1. 按鈕檢測與防彈跳處理
  int reading = digitalRead(buttonPin);

  // 如果按鈕狀態改變（雜訊或真實按下）
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // 當確定按鈕狀態改變且穩定時
    // 這裡設為 LOW 代表按下按鈕（配合 INPUT_PULLUP）
    // 如果您的按鈕按下是 HIGH，請改為 (reading == HIGH)
    static int buttonState = HIGH;
    if (reading != buttonState) {
      buttonState = reading;
      if (buttonState == LOW) { // 按下按鈕瞬間觸發
        mode = (mode + 1) % 4;  // 切換模式：0 -> 1 -> 2 -> 3 -> 0
      }
    }
  }

  lastButtonState = reading;

  // 2. 根據 mode 設定執行對應的燈光效果
  switch (mode) {
    case 0:
      // 狀態 0：衡亮 (全亮)
      setLeds(HIGH);
      break;

    case 1:
      // 狀態 1：慢閃爍 (開關週期 1000ms)
      blinkLeds(500);
      break;

    case 2:
      // 狀態 2：不快不慢的閃爍 (開關週期 400ms)
      blinkLeds(200);
      break;

    case 3:
      // 狀態 3：快閃爍 (開關週期 150ms)
      blinkLeds(75);
      break;
  }
}

// 輔助函式：讓所有 LED 同步顯示亮/滅
void setLeds(bool state) {
  for (int i = 0; i < numLeds; i++) {
    digitalWrite(ledPins[i], state);
  }
}

// 輔助函式：非阻塞式閃爍控制
void blinkLeds(unsigned long interval) {
  unsigned long currentMillis = millis();
  if (currentMillis - lastBlinkTime >= interval) {
    lastBlinkTime = currentMillis;
    ledState = !ledState;
    setLeds(ledState);
  }
}