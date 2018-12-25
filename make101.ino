// Test driving code

int IN1  =  5;
int IN2  =  6;
int IN3  =  9;
int IN4  =  10;

void setup() 
{
  pinMode(IN1,  OUTPUT);
  pinMode(IN2,  OUTPUT);  
  pinMode(IN3,  OUTPUT);
  pinMode(IN4,  OUTPUT);  
}

void loop() {

  digitalWrite(IN1,LOW);  // GO
  digitalWrite(IN2,HIGH);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH);
  delay(1000);
  digitalWrite(IN1,HIGH); // STOP
  digitalWrite(IN2,HIGH);
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,HIGH);
  delay(1000);
  digitalWrite(IN1,HIGH); // BACK
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);
  delay(1000);

}
