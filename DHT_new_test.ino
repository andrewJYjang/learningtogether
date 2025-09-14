#include <DHT.h>  // 라이브러리에서 DHT 추가

#define DHTPIN 2    // DHT 센서 데이터선을 디지털핀에 연결
#define DHTTYPE DHT11 // DHT 센서 정의

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);         
  dht.begin();  // DHT 센서 연결
}

void loop() {
  int h = dht.readHumidity(); // DHT 센서에서 습도데이터 읽기
  int t = dht.readTemperature();  // DHT 센서에서 온도데이터 읽기

  // int 로 값을 정의했을 때 오류가 나면 2147483647 이 출력되고
  // float으로 값을 정의하면 오류났을 때 nan 으로 출력된다.

  // 데이터가 없을 때 처리하는 방식 
  // 습도 혹은 온도 데이터가 nan 일때 출력되는 메시지
  if (isnan(h) || isnan(t)) {              
    Serial.println(F("센서와 연결되지 않았습니다"));
    return;
  }

  Serial.print("humidity:"); 
  Serial.println(h);                 // 습도출력

  Serial.print("temperature:");
  Serial.println(t);                 // 온도출력

  delay(1000);
}