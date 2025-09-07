int ENA = 11; // 정수형 변수 선언 후 디지털 핀 번호 11번으로 초기화
int IN1 = 10; 
int IN2 = 9;
int ENB = 6;
int IN3 = 5;
int IN4 = 4;

void setup() 
{ 
    pinMode(ENA, OUTPUT); // 디지털 11번 핀을 출력(OUTPUT) 모드로 설정
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(ENB, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
} 

void loop() 
{ 
  // DC모터 A 회전  
  digitalWrite(IN1, HIGH); // 디지털 10번 핀에 디지털 신호 HIGH 출력
  digitalWrite(IN2, LOW); // 디지털 9번 핀에 디지털 신호 LOW 출력
  analogWrite(ENA, 255); // 디지털 11번 핀에 PWM 아날로그 신호 5V 출력
  delay(1000); // 1000ms 대기  
  analogWrite(ENA, 0); // 디지털 11번 핀에 PWM 아날로그 신호 0V 출력
  delay(1000);  
  // DC모터 A 역회전
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, 255);
  delay(1000);
  analogWrite(ENA, 0);
  delay(1000);



  // DC모터 B 회전  
  digitalWrite(IN3, HIGH); // 디지털 5번 핀에 디지털 신호 HIGH 출력
  digitalWrite(IN4, LOW); // 디지털 4번 핀에 디지털 신호 HIGH 출력
  analogWrite(ENB, 255); // 디지털 6번 핀에 PWM 아날로그 신호 5V 출력
  delay(1000);  
  analogWrite(ENB, 0); // 디지털 6번 핀에 PWM 아날로그 신호 0V 출력
  delay(1000);  
  // DC모터 B 역회전
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENB, 255);
  delay(1000);
  analogWrite(ENB, 0);
  delay(1000);
  
  
  
  // DC모터 A 회전 가속
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  for (int i = 1; i <= 255; i++) // 10 ms 간격으로 점점 빠르게 회전
  {
    analogWrite(ENA, i);
    delay(10); // 10 ms 대기
  }
  delay(1000);
  // DC모터 A 회전 감속
  for (int i = 254; i >= 0; i--) // 10 ms 간격으로 점점 느리게 회전
  {
    analogWrite(ENA, i);
    delay(10);
  }
  delay(1000);
  // DC모터 A 역회전 가속  
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  for (int i = 1; i <= 255; i++)
  {
    analogWrite(ENA, i);
    delay(10);
  }
  delay(1000);
  // DC모터 A 역회전 감속 
  for (int i = 254; i >= 0; i--)
  {
    analogWrite(ENA, i);
    delay(10);
  }
  delay(1000);
  
  
  
  // DC모터 B 회전 가속 
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  for (int i = 1; i <= 255; i++)
  {
    analogWrite(ENB, i);
    delay(10);
  }
  delay(1000);
  // DC모터 B 회전 감속
  for (int i = 254; i >= 0; i--)
  {
    analogWrite(ENB, i);
    delay(10);
  }
  delay(1000);
  // DC모터 B 역회전 가속 
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  for (int i = 1; i <= 255; i++)
  {
    analogWrite(ENB, i);
    delay(10);
  }
  delay(1000);
  // DC모터 B 역회전 감속 
  for (int i = 254; i >= 0; i--)
  {
    analogWrite(ENB, i);
    delay(10);
  }
  delay(1000);
}
