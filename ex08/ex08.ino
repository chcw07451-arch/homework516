#include <WiFi.h>
#include <WebServer.h>

// 替换成你的OPPO热点账号密码
const char* ssid     = "OPPO Reno15 43F0";
const char* password = "85126838";

WebServer server(80);
const uint8_t ledPin = 2;
const uint8_t touchPin = 4; 
// 全局状态标记
bool isArm = false;    // false=撤防 true=布防
bool isAlarm = false;  // false=无报警 true=锁定报警

// 网页前端：布防/撤防按钮
String htmlPage = R"HTML(
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<title>安防报警系统</title>
<style>
body{text-align:center;margin-top:60px;font-size:22px;}
button{font-size:24px;padding:16px 45px;margin:20px 10px;border:none;border-radius:8px;}
#armBtn{background:#009933;color:white;}
#disarmBtn{background:#dd2222;color:white;}
</style>
</head>
<body>
<h2>ESP32安防主机</h2>
<button id="armBtn" onclick="fetch('/arm')">布防 Arm</button>
<button id="disarmBtn" onclick="fetch('/disarm')">撤防 Disarm</button>
</body>
</html>
)HTML";

// 主页路由
void handleRoot() {
  server.send(200, "text/html", htmlPage);
}

// 布防接口
void handleArm() {
  isArm = true;
  server.send(200, "text/plain", "系统已布防，触碰引脚将触发报警");
}

// 撤防接口：清除报警、关灯、重置状态
void handleDisarm() {
  isArm = false;
  isAlarm = false;
  digitalWrite(ledPin, LOW);
  server.send(200, "text/plain", "系统已撤防，报警复位");
}

// 注册Web路由
void initWebServer() {
  server.on("/", handleRoot);
  server.on("/arm", handleArm);
  server.on("/disarm", handleDisarm);
  server.begin();
}

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  // 连接WiFi热点
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi连接成功，设备IP：" + WiFi.localIP().toString());
  initWebServer();
}

void loop() {
  server.handleClient();
  uint16_t touchValue = touchRead(touchPin);

  // 仅布防状态下才检测触摸触发报警
  if (isArm && !isAlarm) {
    // 触摸阈值：数值小于30判定为触碰，可根据硬件微调
    if (touchValue < 400) {
      isAlarm = true;
    }
  }

  // 报警锁定：高频闪烁LED，松手也不会停止
  if (isAlarm) {
    digitalWrite(ledPin, HIGH);
    delay(80);
    digitalWrite(ledPin, LOW);
    delay(80);
  }
}