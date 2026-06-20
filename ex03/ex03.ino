#define LED_PIN 2

// 时序参数
const unsigned long SHORT_ON  = 200;   // 短亮
const unsigned long LONG_ON   = 1000;   // 长亮
const unsigned long GAP      = 300;   // 闪烁之间间隔
const unsigned long END_PAUSE= 2000;  // 一整轮SOS结束后的长停顿

unsigned long currentTime;
unsigned long lastTime = 0;
int stage = 0; // 0~5：3短；6~11：3长；12~17：3短；18：结束停顿

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
}

void loop() {
  currentTime = millis();
  unsigned long delta = currentTime - lastTime;

  if (stage >= 0 && stage <= 5) {
    // 第一组：3短闪，每个闪烁分【亮、间隔】两步
    if (stage % 2 == 0) {
      // 亮灯阶段
      if (delta >= SHORT_ON) {
        digitalWrite(LED_PIN, LOW);
        lastTime = currentTime;
        stage++;
      } else {
        digitalWrite(LED_PIN, HIGH);
      }
    } else {
      // 熄灭间隔
      if (delta >= GAP) {
        lastTime = currentTime;
        stage++;
      } else {
        digitalWrite(LED_PIN, LOW);
      }
    }
  } else if (stage >= 6 && stage <= 11) {
    // 第二组：3长闪
    if (stage % 2 == 0) {
      if (delta >= LONG_ON) {
        digitalWrite(LED_PIN, LOW);
        lastTime = currentTime;
        stage++;
      } else {
        digitalWrite(LED_PIN, HIGH);
      }
    } else {
      if (delta >= GAP) {
        lastTime = currentTime;
        stage++;
      } else {
        digitalWrite(LED_PIN, LOW);
      }
    }
  } else if (stage >= 12 && stage <= 17) {
    // 第三组：3短闪
    if (stage % 2 == 0) {
      if (delta >= SHORT_ON) {
        digitalWrite(LED_PIN, LOW);
        lastTime = currentTime;
        stage++;
      } else {
        digitalWrite(LED_PIN, HIGH);
      }
    } else {
      if (delta >= GAP) {
        lastTime = currentTime;
        stage++;
      } else {
        digitalWrite(LED_PIN, LOW);
      }
    }
  } else {
    // 完整一轮结束，长停顿，然后重置从头再来
    digitalWrite(LED_PIN, LOW);
    if (delta >= END_PAUSE) {
      stage = 0;
      lastTime = currentTime;
    }
  }
}