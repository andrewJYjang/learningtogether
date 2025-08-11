#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DHT.h>
#include <ArduinoJson.h>

// WiFi 설정
const char* ssid = "Chichi_home";
const char* password = "eda7zx6820";

// DHT11 센서 설정
#define DHT_PIN D2
#define DHT_TYPE DHT11
DHT dht(DHT_PIN, DHT_TYPE);

// 조도센서 설정 (아날로그 핀)
#define LIGHT_SENSOR_PIN A0

// 웹서버 설정
ESP8266WebServer server(80);

// 센서 데이터 저장 변수
float temperature = 0;
float humidity = 0;
int lightLevel = 0;
unsigned long lastUpdate = 0;

void setup() {
  Serial.begin(115200);
  
  // DHT11 센서 초기화
  dht.begin();
  
  // WiFi 연결
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println();
  Serial.println("WiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  // 웹서버 라우트 설정
  server.on("/", handleRoot);
  server.on("/api/sensors", handleSensorData);
  server.on("/api/temperature", handleTemperature);
  server.on("/api/humidity", handleHumidity);
  server.on("/api/light", handleLight);
  
  // CORS 헤더 설정 (외부 접근 허용)
  server.onNotFound([]() {
    if (server.method() == HTTP_OPTIONS) {
      server.sendHeader("Access-Control-Allow-Origin", "*");
      server.sendHeader("Access-Control-Allow-Methods", "GET,POST,OPTIONS");
      server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
      server.send(200);
    } else {
      server.send(404, "text/plain", "Not Found");
    }
  });
  
  server.begin();
  Serial.println("HTTP server started");
  Serial.println("ESP8266 IP: " + WiFi.localIP().toString());
  Serial.println("To access from outside, run: ngrok http " + WiFi.localIP().toString() + ":80");
}

void loop() {
  // 센서 데이터 읽기 (15초마다)
  static unsigned long lastSensorRead = 0;
  if (millis() - lastSensorRead > 15000) {
    readSensors();
    lastSensorRead = millis();
  }
  
  // 웹서버 처리
  server.handleClient();
  
  // 시리얼 모니터에 데이터 출력 (10초마다)
  static unsigned long lastSerialPrint = 0;
  if (millis() - lastSerialPrint > 10000) {
    printSensorData();
    lastSerialPrint = millis();
  }
}

void readSensors() {
  // DHT11 데이터 읽기
  float newTemp = dht.readTemperature();
  float newHum = dht.readHumidity();
  
  if (!isnan(newTemp) && !isnan(newHum)) {
    temperature = newTemp;
    humidity = newHum;
    lastUpdate = millis();
  } else {
    Serial.println("DHT11 읽기 실패!");
  }
  
  // 조도센서 데이터 읽기 (0-1024를 0-100%로 변환)
  int rawLight = analogRead(LIGHT_SENSOR_PIN);
  lightLevel = map(rawLight, 0, 1024, 0, 100);
}

void printSensorData() {
  Serial.println("=== 센서 데이터 ===");
  Serial.println("온도: " + String(temperature) + "°C");
  Serial.println("습도: " + String(humidity) + "%");
  Serial.println("조도: " + String(lightLevel) + "%");
  Serial.println("업데이트: " + String(lastUpdate / 1000) + "초 전");
  Serial.println("==================");
}

// 메인 웹페이지 (실시간 모니터링 대시보드)
void handleRoot() {
  String html = "<!DOCTYPE html>";
  html += "<html><head>";
  html += "<meta charset='utf-8'>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
  html += "<title>ESP8266 센서 모니터링</title>";
  html += "<style>";
  html += "body { font-family: Arial; margin: 20px; background-color: #f0f0f0; }";
  html += ".container { max-width: 800px; margin: 0 auto; }";
  html += ".card { background: white; padding: 20px; margin: 10px 0; border-radius: 10px; box-shadow: 0 2px 5px rgba(0,0,0,0.1); }";
  html += ".sensor-value { font-size: 2em; font-weight: bold; color: #333; }";
  html += ".sensor-unit { color: #666; font-size: 0.8em; }";
  html += ".temp { color: #e74c3c; }";
  html += ".humidity { color: #3498db; }";
  html += ".light { color: #f39c12; }";
  html += ".status { text-align: center; padding: 10px; margin: 10px 0; border-radius: 5px; }";
  html += ".online { background-color: #d4edda; color: #155724; }";
  html += ".grid { display: grid; grid-template-columns: repeat(auto-fit, minmax(200px, 1fr)); gap: 15px; }";
  html += "</style>";
  html += "</head><body>";
  
  html += "<div class='container'>";
  html += "<h1 style='text-align: center; color: #2c3e50;'>ESP8266 센서 모니터링</h1>";
  
  html += "<div class='status online'>";
  html += "<strong>시스템 온라인</strong> - 마지막 업데이트: <span id='lastUpdate'></span>";
  html += "</div>";
  
  html += "<div class='grid'>";
  
  html += "<div class='card'>";
  html += "<h3>🌡️ 온도</h3>";
  html += "<div class='sensor-value temp'><span id='temperature'></span> <span class='sensor-unit'>°C</span></div>";
  html += "</div>";
  
  html += "<div class='card'>";
  html += "<h3>💧 습도</h3>";
  html += "<div class='sensor-value humidity'><span id='humidity'></span> <span class='sensor-unit'>%</span></div>";
  html += "</div>";
  
  html += "<div class='card'>";
  html += "<h3>☀️ 조도</h3>";
  html += "<div class='sensor-value light'><span id='light'></span> <span class='sensor-unit'>%</span></div>";
  html += "</div>";
  
  html += "</div>";
  
  // API 버튼들
  html += "<div class='card'>";
  html += "<h3>API 엔드포인트</h3>";
  html += "<p><a href='/api/sensors' target='_blank'>전체 센서 데이터 (JSON)</a></p>";
  html += "<p><a href='/api/temperature' target='_blank'>온도만</a></p>";
  html += "<p><a href='/api/humidity' target='_blank'>습도만</a></p>";
  html += "<p><a href='/api/light' target='_blank'>조도만</a></p>";
  html += "</div>";
  
  html += "</div>";
  
  // JavaScript로 실시간 업데이트
  html += "<script>";
  html += "function updateSensorData() {";
  html += "  fetch('/api/sensors')";
  html += "    .then(response => response.json())";
  html += "    .then(data => {";
  html += "      document.getElementById('temperature').textContent = data.temperature.toFixed(1);";
  html += "      document.getElementById('humidity').textContent = data.humidity.toFixed(1);";
  html += "      document.getElementById('light').textContent = data.light;";
  html += "      document.getElementById('lastUpdate').textContent = data.lastUpdate;";
  html += "    })";
  html += "    .catch(error => console.error('Error:', error));";
  html += "}";
  html += "updateSensorData();";
  html += "setInterval(updateSensorData, 3000);"; // 3초마다 업데이트
  html += "</script>";
  
  html += "</body></html>";
  
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/html", html);
}

// JSON API - 전체 센서 데이터
void handleSensorData() {
  DynamicJsonDocument doc(1024);
  
  doc["status"] = "online";
  doc["temperature"] = temperature;
  doc["humidity"] = humidity;
  doc["light"] = lightLevel;
  doc["lastUpdate"] = String((millis() - lastUpdate) / 1000) + "초 전";
  doc["uptime"] = millis() / 1000;
  doc["freeHeap"] = ESP.getFreeHeap();
  
  String json;
  serializeJson(doc, json);
  
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Content-Type", "application/json");
  server.send(200, "application/json", json);
}

// 온도 API
void handleTemperature() {
  DynamicJsonDocument doc(256);
  doc["temperature"] = temperature;
  doc["unit"] = "°C";
  doc["timestamp"] = millis();
  
  String json;
  serializeJson(doc, json);
  
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Content-Type", "application/json");
  server.send(200, "application/json", json);
}

// 습도 API
void handleHumidity() {
  DynamicJsonDocument doc(256);
  doc["humidity"] = humidity;
  doc["unit"] = "%";
  doc["timestamp"] = millis();
  
  String json;
  serializeJson(doc, json);
  
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Content-Type", "application/json");
  server.send(200, "application/json", json);
}

// 조도 API
void handleLight() {
  DynamicJsonDocument doc(256);
  doc["light"] = lightLevel;
  doc["unit"] = "%";
  doc["timestamp"] = millis();
  
  String json;
  serializeJson(doc, json);
  
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Content-Type", "application/json");
  server.send(200, "application/json", json);
}