#define TOUCH_PIN 4
#define LED_PIN 2

// 全局状态
int speedGear = 1;       // 1慢、2中、3快
int pwmValue = 0;
int dir = 1;             // 1递增，-1递减
int lastTouchRaw;
const int TOUCH_THRESH = 40;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  lastTouchRaw = touchRead(TOUCH_PIN);
}

void loop() {
  // 1. 触摸边沿检测+软件防抖，切换档位
  int currTouchRaw = touchRead(TOUCH_PIN);
  // 检测：上一次未触摸，当前瞬间触摸（下降沿）
  if (lastTouchRaw > TOUCH_THRESH && currTouchRaw <= TOUCH_THRESH) {
    delay(25);  // 防抖
    if (touchRead(TOUCH_PIN) <= TOUCH_THRESH) {
      speedGear++;
      if (speedGear > 3) speedGear = 1;
    }
  }
  lastTouchRaw = currTouchRaw;

  // 2. 根据档位配置呼吸步长与延时，档位越高变化越快
  int step, delayTime;
  switch (speedGear) {
    case 1:
      step = 1;
      delayTime = 15;
      break;
    case 2:
      step = 3;
      delayTime = 8;
      break;
    case 3:
      step = 6;
      delayTime = 3;
      break;
    default:
      step = 1;
      delayTime = 15;
  }

  // 3. 标准呼吸灯逻辑：0→255渐亮，255→0渐暗
  pwmValue += dir * step;
  if (pwmValue >= 255) {
    pwmValue = 255;
    dir = -1;
  } else if (pwmValue <= 0) {
    pwmValue = 0;
    dir = 1;
  }

  analogWrite(LED_PIN, pwmValue);
  delay(delayTime);
} 
