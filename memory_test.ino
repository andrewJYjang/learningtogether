#include "esp_camera.h"
#include "WiFi.h"

void setup() {
  Serial.begin(115200);
  delay(2000);
  
  Serial.println("=== ESP32 Memory Info ===");
  Serial.print("Free Heap: ");
  Serial.println(ESP.getFreeHeap());
  Serial.print("Total PSRAM: ");
  Serial.println(ESP.getPsramSize());
  Serial.print("Free PSRAM: ");
  Serial.println(ESP.getFreePsram());
}

void loop() {
  delay(5000);
}