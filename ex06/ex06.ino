// 定义两路LED引脚
#define LED_CH1 2
#define LED_CH2 4

// PWM亮度变量、变化步长
uint8_t brightness = 0;
int step = 2;

void setup() {
  pinMode(LED_CH1, OUTPUT);
  pinMode(LED_CH2, OUTPUT);
}

void loop() {
  // 更新亮度，触达边界反转增减方向
  brightness += step;
  if (brightness >= 255 || brightness <= 0) {
    step = -step;
  }

  // 核心：两路PWM完全反相
  analogWrite(LED_CH1, brightness);
  analogWrite(LED_CH2, 255 - brightness);

  // 控制整体渐变平滑速度，数值越小过渡越快
  delay(8);
} 
