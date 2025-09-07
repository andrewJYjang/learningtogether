#include <Servo.h>

Servo myServo;
int servoPin = 9;  // 서보 모터 연결 핀

void setup() {
  Serial.begin(9600);
  Serial.println("=== SG90 서보 모터 진단 테스트 ===");
  Serial.println("시리얼 모니터에서 명령을 입력하세요:");
  Serial.println("1: 기본 각도 테스트");
  Serial.println("2: 연속 회전 테스트");
  Serial.println("3: PWM 신호 테스트");
  Serial.println("4: 느린 움직임 테스트");
  Serial.println("5: 중립 위치 테스트");
  Serial.println("6: 극한 각도 테스트");
  Serial.println("7: 지터링 테스트");
  Serial.println("0: 서보 분리");
  Serial.println();
  
  myServo.attach(servoPin);
  delay(1000);
  
  // 초기 중립 위치로 이동
  myServo.write(90);
  delay(1000);
  Serial.println("서보 모터가 중립 위치(90도)로 초기화되었습니다.");
}

void loop() {
  if (Serial.available() > 0) {
    char command = Serial.read();
    
    switch (command) {
      case '1':
        basicAngleTest();
        break;
      case '2':
        continuousRotationTest();
        break;
      case '3':
        pwmSignalTest();
        break;
      case '4':
        slowMovementTest();
        break;
      case '5':
        neutralPositionTest();
        break;
      case '6':
        extremeAngleTest();
        break;
      case '7':
        jitterTest();
        break;
      case '0':
        detachServo();
        break;
      default:
        Serial.println("잘못된 명령입니다. 0-7을 입력하세요.");
        break;
    }
  }
}

// 1. 기본 각도 테스트
void basicAngleTest() {
  Serial.println("\n--- 기본 각도 테스트 ---");
  int angles[] = {0, 45, 90, 135, 180};
  
  for (int i = 0; i < 5; i++) {
    Serial.print("각도 ");
    Serial.print(angles[i]);
    Serial.println("도로 이동 중...");
    
    myServo.write(angles[i]);
    delay(1500);  // 충분한 시간 대기
    
    Serial.print("현재 위치: ");
    Serial.print(angles[i]);
    Serial.println("도 (예상)");
  }
  Serial.println("기본 각도 테스트 완료\n");
}

// 2. 연속 회전 테스트
void continuousRotationTest() {
  Serial.println("\n--- 연속 회전 테스트 ---");
  Serial.println("0도에서 180도까지 연속 회전...");
  
  for (int angle = 0; angle <= 180; angle += 10) {
    myServo.write(angle);
    Serial.print("각도: ");
    Serial.println(angle);
    delay(200);
  }
  
  Serial.println("180도에서 0도까지 역방향 회전...");
  for (int angle = 180; angle >= 0; angle -= 10) {
    myServo.write(angle);
    Serial.print("각도: ");
    Serial.println(angle);
    delay(200);
  }
  Serial.println("연속 회전 테스트 완료\n");
}

// 3. PWM 신호 테스트 (원시 신호)
void pwmSignalTest() {
  Serial.println("\n--- PWM 신호 테스트 ---");
  myServo.detach();  // Servo 라이브러리 제어 해제
  
  Serial.println("원시 PWM 신호로 제어 중...");
  
  // 0도 (1ms 펄스)
  Serial.println("0도 위치 (1ms 펄스)");
  for (int i = 0; i < 50; i++) {
    digitalWrite(servoPin, HIGH);
    delayMicroseconds(1000);  // 1ms
    digitalWrite(servoPin, LOW);
    delayMicroseconds(19000); // 19ms (총 20ms 주기)
  }
  
  delay(1000);
  
  // 90도 (1.5ms 펄스)
  Serial.println("90도 위치 (1.5ms 펄스)");
  for (int i = 0; i < 50; i++) {
    digitalWrite(servoPin, HIGH);
    delayMicroseconds(1500);  // 1.5ms
    digitalWrite(servoPin, LOW);
    delayMicroseconds(18500); // 18.5ms
  }
  
  delay(1000);
  
  // 180도 (2ms 펄스)
  Serial.println("180도 위치 (2ms 펄스)");
  for (int i = 0; i < 50; i++) {
    digitalWrite(servoPin, HIGH);
    delayMicroseconds(2000);  // 2ms
    digitalWrite(servoPin, LOW);
    delayMicroseconds(18000); // 18ms
  }
  
  myServo.attach(servoPin);  // Servo 라이브러리 재연결
  Serial.println("PWM 신호 테스트 완료\n");
}

// 4. 느린 움직임 테스트
void slowMovementTest() {
  Serial.println("\n--- 느린 움직임 테스트 ---");
  Serial.println("1도씩 천천히 이동...");
  
  int currentAngle = 90;
  myServo.write(currentAngle);
  delay(500);
  
  // 90도에서 180도까지
  for (int angle = 90; angle <= 180; angle++) {
    myServo.write(angle);
    if (angle % 10 == 0) {
      Serial.print("현재 각도: ");
      Serial.println(angle);
    }
    delay(50);  // 매우 느린 움직임
  }
  
  delay(1000);
  
  // 180도에서 0도까지
  for (int angle = 180; angle >= 0; angle--) {
    myServo.write(angle);
    if (angle % 10 == 0) {
      Serial.print("현재 각도: ");
      Serial.println(angle);
    }
    delay(50);
  }
  
  // 중립 위치로 복귀
  myServo.write(90);
  Serial.println("느린 움직임 테스트 완료\n");
}

// 5. 중립 위치 테스트
void neutralPositionTest() {
  Serial.println("\n--- 중립 위치 테스트 ---");
  
  for (int i = 0; i < 10; i++) {
    Serial.print("중립 위치 테스트 ");
    Serial.print(i + 1);
    Serial.println("/10");
    
    myServo.write(90);
    delay(1000);
    
    // 약간의 변화를 주어 반응 확인
    myServo.write(85);
    delay(500);
    myServo.write(95);
    delay(500);
    myServo.write(90);
    delay(1000);
  }
  Serial.println("중립 위치 테스트 완료\n");
}

// 6. 극한 각도 테스트
void extremeAngleTest() {
  Serial.println("\n--- 극한 각도 테스트 ---");
  Serial.println("주의: 일부 서보는 0-180도 범위를 벗어날 수 있습니다");
  
  int testAngles[] = {-10, 0, 30, 60, 90, 120, 150, 180, 190};
  int numAngles = sizeof(testAngles) / sizeof(testAngles[0]);
  
  for (int i = 0; i < numAngles; i++) {
    Serial.print("테스트 각도: ");
    Serial.println(testAngles[i]);
    
    myServo.write(testAngles[i]);
    delay(2000);  // 충분한 시간 대기
    
    if (testAngles[i] < 0 || testAngles[i] > 180) {
      Serial.println("  -> 범위 외 각도입니다");
    }
  }
  
  // 안전한 위치로 복귀
  myServo.write(90);
  Serial.println("극한 각도 테스트 완료\n");
}

// 7. 지터링(떨림) 테스트
void jitterTest() {
  Serial.println("\n--- 지터링 테스트 ---");
  Serial.println("미세한 각도 변화로 떨림 현상 확인...");
  
  int baseAngle = 90;
  
  for (int cycle = 0; cycle < 5; cycle++) {
    Serial.print("지터링 테스트 사이클 ");
    Serial.print(cycle + 1);
    Serial.println("/5");
    
    for (int i = 0; i < 20; i++) {
      myServo.write(baseAngle + 1);
      delay(100);
      myServo.write(baseAngle - 1);
      delay(100);
      myServo.write(baseAngle);
      delay(100);
    }
    delay(1000);
  }
  
  Serial.println("지터링 테스트 완료\n");
}

// 0. 서보 분리
void detachServo() {
  Serial.println("\n--- 서보 모터 분리 ---");
  myServo.detach();
  Serial.println("서보 모터가 분리되었습니다.");
  Serial.println("전원을 끄고 배선을 확인해보세요.");
  Serial.println("다시 테스트하려면 아두이노를 리셋하세요.\n");
}
