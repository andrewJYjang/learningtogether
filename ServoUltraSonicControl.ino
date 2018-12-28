#include <Servo.h>

Servo myservo; // create servo object to control a servo

const int trigPin = 2;

const int echoPin = 4;

void setup() {

// initialize serial communication:

Serial.begin(9600);

myservo.attach(9); // attaches the servo on pin 9 to the servo object

}

void loop() {

// and the distance result in centimeters:

long duration, cm;

pinMode(trigPin, OUTPUT);

digitalWrite(trigPin, LOW);

delayMicroseconds(2);

digitalWrite(trigPin, HIGH);

delayMicroseconds(20);

digitalWrite(trigPin, LOW);

pinMode(echoPin, INPUT);

duration = pulseIn(echoPin, HIGH);

// convert the time into a distance

cm = microsecondsToCentimeters(duration);

// the condition for the distance

if ( cm > 7 && cm < 14)

{

myservo.write(140); // sets the servo position according to the scaled value

delay(4000);

}

else if ( cm < 8)

{

myservo.write(40); // sets the servo position according to the scaled value

delay(100);

}

else

{

myservo.write(40); // sets the servo position according to the scaled value

delay(100);

}

Serial.print(cm);

Serial.print("cm");

Serial.println();

delay(100);

}

long microsecondsToCentimeters(long microseconds) {

// The speed of sound is 340 m/s or 29 microseconds per centimeter.

// The ping travels out and back, so to find the distance of the

// object we take half of the distance travelled.

return microseconds / 29 / 2;

}
