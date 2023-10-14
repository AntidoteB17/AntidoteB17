/* Obstacle Avoiding Robot Using Ultrasonic Sensor and Arduino NANO

    Circuit Digest(www.circuitdigest.com)

*/

#include <Servo.h>          //Servo motor library. This is standard library

int trigPin = A1;      // trig pin of HC-SR04

int echoPin = A2;     // Echo pin of HC-SR04


int revleft4 = 4;       //REVerse motion of Left motor

int fwdleft5 = 5;       //ForWarD motion of Left motor

int revright6 = 2;      //REVerse motion of Right motor

int fwdright7 = 3;      //ForWarD motion of Right motor


long duration, distance;
Servo servo_motor; //our servo name

void setup() {
  delay(random(500, 2000));  // delay for random time

  Serial.begin(9600);

  pinMode(revleft4, OUTPUT);      // set Motor pins as output

  pinMode(fwdleft5, OUTPUT);

  pinMode(revright6, OUTPUT);

  pinMode(fwdright7, OUTPUT);

  pinMode(trigPin, OUTPUT);         // set trig pin as output

  pinMode(echoPin, INPUT);          //set echo pin as input to capture reflected waves
  servo_motor.attach(11); //our servo pin
  servo_motor.write(80);
  delay(500);
  servo_motor.write(20);
  delay(500);
  servo_motor.write(170);
  delay(500);
  servo_motor.write(80);
}


void loop() {
  delay(50);

  digitalWrite(trigPin, LOW);

  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);     // send waves for 10 us

  delayMicroseconds(10);

  duration = pulseIn(echoPin, HIGH); // receive reflected waves

  distance = duration / 58.2;   // convert to distance
  delay(10);
  Serial.println (distance);
  // If you dont get proper movements of your robot then alter the pin numbers

  if (distance > 20)

  {
    Serial.println("move forward");

    digitalWrite(fwdright7, HIGH);                    // move forward

    digitalWrite(revright6, LOW);

    digitalWrite(fwdleft5, HIGH);

    digitalWrite(revleft4, LOW);

  }


  if (distance <= 18)

  {
    Serial.println("move left");

    digitalWrite(fwdright7, LOW);  //Stop

    digitalWrite(revright6, LOW);

    digitalWrite(fwdleft5, LOW);

    digitalWrite(revleft4, LOW);

    delay(500);

    digitalWrite(fwdright7, LOW);      //movebackword

    digitalWrite(revright6, HIGH);

    digitalWrite(fwdleft5, LOW);

    digitalWrite(revleft4, HIGH);

    delay(500);

    digitalWrite(fwdright7, LOW);  //Stop

    digitalWrite(revright6, LOW);

    digitalWrite(fwdleft5, LOW);

    digitalWrite(revleft4, LOW);

    delay(100);

    digitalWrite(fwdright7, HIGH);   // move left

    digitalWrite(revright6, LOW);

    digitalWrite(revleft4, HIGH);

    digitalWrite(fwdleft5, LOW);

    delay(500);

  }
}
