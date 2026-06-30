#include <WiFi.h>
#include <WebServer.h>

// 你的手机热点信息，和前面作业保持一致
const char* ssid     = "OPPO Reno15 43F0";
const char* password = "85126838";

WebServer server(80);
const uint8_t touchPin = 4;  
// 网页：AJAX定时拉取触摸传感器数值，实时刷新显示
String htmlPage = R"HTML(
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>触摸传感器仪表盘</title>
<style>
body{text-align:center;margin-top:80px;font-family:Arial;}
h2{color:#222;}
#sensorNum{font-size:90px;color:#0066ff;font-weight:bold;margin-top:40px;}
</style>
</head>
<body>
<h2>ESP32触摸实时数值监控仪表盘</h2>
<div id="sensorNum">0</div>

<script>
// 定时请求接口获取触摸值
function refreshData(){
    fetch("/getTouchData")
        .then(res => res.text())
        .then(val => {
            document.getElementById("sensorNum").innerText = val;
        })
}
// 每100毫秒刷新一次，数值实时跳动
setInterval(refreshData, 100);
</script>
</body>
</html>
)HTML";

// 主页，下发完整网页
void handleRoot() {
  server.send(200, "text/html", htmlPage);
}

// 数据接口：返回当前触摸传感器原始数值
void handleGetTouch() {
  uint16_t touchRaw = touchRead(touchPin);
  server.send(200, "text/plain", String(touchRaw));
}

// 初始化Web服务路由
void initWeb() {
  server.on("/", handleRoot);
  server.on("/getTouchData", handleGetTouch);
  server.begin();
}

void setup() {
  Serial.begin(115200);
  // 连接WiFi热点
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // 串口打印设备IP地址
  Serial.println("\nWiFi已连接，设备IP：" + WiFi.localIP().toString());
  initWeb();
}

void loop() {
  // 持续处理网页客户端请求
  server.handleClient();
}