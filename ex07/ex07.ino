#include <WiFi.h>
#include <WebServer.h>

// WiFi配置（你的热点信息无误，引号是英文双引号，没问题）
const char* ssid     = "OPPO Reno15 43F0";
const char* password = "85126838";

WebServer server(80);
const uint8_t ledPin = 2;
uint8_t ledBright = 0; // 亮度0-255

// HTML网页，包含range滑动条+JS fetch发送亮度
String htmlPage = R"HTML(
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<title>ESP32无极调光</title>
<style>
body{text-align:center;margin-top:50px;font-size:22px;}
#slider{width:80%;height:30px;margin:30px 0;}
</style>
</head>
<body>
<h2>LED亮度调节</h2>
<input type="range" id="slider" min="0" max="255" value="0">
<div>当前亮度值：<span id="val">0</span></div>
<script>
const slider = document.getElementById('slider');
// 修复：删除多余的右括号
const valSpan = document.getElementById('val');
// 滑动条变动触发请求
slider.addEventListener('input', function(){
    let v = this.value;
    valSpan.innerText = v;
    // GET请求发送亮度到ESP32
    fetch(`/set?bright=${v}`);
})
</script>
</body>
</html>
)HTML";

void handleRoot() {
  server.send(200, "text/html", htmlPage);
}

void handleSetBright() {
  if(server.hasArg("bright")){
    ledBright = server.arg("bright").toInt();
    ledBright = constrain(ledBright, 0, 255);
    analogWrite(ledPin, ledBright);
  }
  server.send(200, "text/plain", "OK");
}

void setupWeb() {
  server.on("/", handleRoot);
  server.on("/set", handleSetBright);
  server.begin();
}

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  analogWrite(ledPin, 0);

  // 连接WiFi
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  // 连接成功后串口打印设备IP地址
  Serial.println("\nWiFi已连接，IP：" + WiFi.localIP().toString());
  setupWeb();
}

void loop() {
  server.handleClient();
}