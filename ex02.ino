#define LED_PIN 2
unsigned long now, lastFlip = 0;
const unsigned long INTERVAL = 500;

void setup(){
  pinMode(LED_PIN, OUTPUT);
}
void loop(){
  now = millis();
  // 间隔500ms翻转一次，1Hz闪烁
  if(now - lastFlip >= INTERVAL){
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    lastFlip = now;
  }
}