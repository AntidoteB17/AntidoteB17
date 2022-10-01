/* THIS PROGRAM IS CREATED BY ENGINEER THIS ELECTRONICS YOUTUBE CHANNEL */
#include <SoftwareSerial.h>
#include <Servo.h>
#define trigpin1 12
#define echopin1 13
#define led1 5
#define led2 6
#define led3 7
#define led4 8
SoftwareSerial SIM900A(2,3);
Servo Servo1;
int pirPin = 4;
int pirState = 0;
int calibrationTime = 30;
int sms_count=0;
const unsigned long eventTime_1 = 1000;
const unsigned long eventTime_2 = 5000;

unsigned long previousTime_1 = 0;
unsigned long previousTime_2 = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  SIM900A.begin(9600);
  Servo1.attach(9);
  delay(1000);
  Servo1.write(0);
  delay(1000);
  Servo1.detach();
  pinMode(trigpin1, OUTPUT);
  pinMode(echopin1, INPUT);
  pinMode(pirPin, INPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);
  digitalWrite(led4, LOW);
  delay(calibrationTime * 1000);   //30 Second Delay
  Serial.println("SMART DUSTBIN IS Ready TO WORK");
  Servo1.attach(9);
}

bool flag = false;
bool flag1 = false;
long Distance1, Duration1;

void loop() {
  // put your main code here, to run repeatedly:
  unsigned long currentTime = millis();

  if ( currentTime - previousTime_1 >= eventTime_1 )
  {
    Serial.println("PIR");
    pirState = digitalRead(pirPin);
    if (pirState == HIGH)
    {
      Servo1.attach(9);
      Serial.println("OPEN LID");
      Servo1.write(100);
      flag1 = true; 
    }
    else if (pirState == LOW) {
      Serial.println("Close LID");
      if(flag1 == false)
      {
        Servo1.detach();
        delay(15);
      }
      else if(flag1 == true)
      {
      Servo1.write(0);
      delay(15);
      flag1 = false;
      }
    }
    previousTime_1 = currentTime;
  }

  if ( currentTime - previousTime_2 >= eventTime_2 )
  {

    Serial.println("Ultrasonic");
    digitalWrite(trigpin1, LOW);
    delayMicroseconds(2);
    digitalWrite(trigpin1, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigpin1, LOW);
    Duration1 = pulseIn(echopin1, HIGH);
    Distance1 = (Duration1 / 2) / 29.1;
    Serial.print("Distance:");
    Serial.println(Distance1);

    if ((Distance1 >= 28) && (Distance1 <= 35 ))
    {
      Serial.println("level 1");
      digitalWrite(led1, HIGH);
      digitalWrite(led2, LOW);
      digitalWrite(led3, LOW);
      digitalWrite(led4, LOW);
      flag = false;
    }
    else if ((Distance1 >= 23) && (Distance1 <= 27))
    {
      Serial.println("level 2");
      digitalWrite(led1, HIGH);
      digitalWrite(led2, HIGH);
      digitalWrite(led3, LOW);
      digitalWrite(led4, LOW);
    }
    else if ((Distance1 >= 18) && (Distance1 <= 22) )
    {
      Serial.println("level 3");
      digitalWrite(led1, HIGH);
      digitalWrite(led2, HIGH);
      digitalWrite(led3, HIGH);
      digitalWrite(led4, LOW);
    }
    else if ((Distance1 >= 9) && (Distance1 <= 16) )
    {
      Serial.println("level 4");
      digitalWrite(led1, HIGH);
      digitalWrite(led2, HIGH);
      digitalWrite(led3, HIGH);
      digitalWrite(led4, HIGH);
      if ( flag == false )
      {
        while(sms_count<3)
        send_msg();
        flag = true;
      }
    }
    else
    {
      digitalWrite(led1, LOW);
      digitalWrite(led2, LOW);
      digitalWrite(led3, LOW);
      digitalWrite(led4, LOW);
    }

    previousTime_2 = currentTime;
  }

}


void send_msg()
{
  Servo1.detach();
  delay(100);
  SIM900A.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second

SIM900A.println("AT+CMGS=\"+2348167962359\"\r"); // Replace x with mobile number
  delay(1000);
  SIM900A.println("DUST BIN IS FULL");// The SMS text you want to send
  delay(100);
  SIM900A.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
  Servo1.attach(9);
  sms_count++;
}
