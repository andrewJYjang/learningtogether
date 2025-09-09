#include "esp_camera.h"
#include "WiFi.h"

void setup() {
  Serial.begin(115200);
  delay(2000);
  Serial.println("=== Simple Test ===");
  
  for(int i = 0; i < 5; i++) {
    Serial.print("Count: ");
    Serial.println(i);
    delay(1000);
  }
  
  Serial.println("Setup complete");
}

void loop() {
  static int count = 0;
  Serial.print("Loop count: ");
  Serial.println(count++);
  delay(3000);
}