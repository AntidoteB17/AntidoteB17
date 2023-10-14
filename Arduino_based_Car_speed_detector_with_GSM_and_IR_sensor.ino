
#include <SoftwareSerial.h>

//Create software serial object to communicate with SIM800L
SoftwareSerial mySerial(4, 3); //SIM800L Tx & Rx is connected to Arduino #4 & #3

float REV = 0;
int RPM_VALUE;
int PREVIOUS = 0;
int TIME;
int Speed;
int diameter;


void INTERRUPT()
{
  REV++;
}

void setup()
{
 //Begin serial communication with Arduino and Arduino IDE (Serial Monitor)
  Serial.begin(9600);
  
  //Begin serial communication with Arduino and SIM800L
  mySerial.begin(9600);
   mySerial.println("AT"); //Once the handshake test is successful, it will back to OK
  updateSerial();
  
  attachInterrupt(0, INTERRUPT, RISING);//output of IR sensor connected to pin D2 of the arduino
  
}

void loop()
{
  delay(1000);
  detachInterrupt(0);                   
  TIME = millis() - PREVIOUS;          
  RPM_VALUE = (REV/TIME) * 60000; 
  Speed = RPM_VALUE * 0.001885 * 40;
  PREVIOUS = millis();                  
  REV = 0;
  Serial.print(RPM_VALUE);
  Serial.println(" rpm");
  Serial.print(Speed);
  Serial.println(" Km/h");
  attachInterrupt(0, INTERRUPT, RISING);

  if(Speed>=100){
    Serial.println("Caution!!! Car is Over speeding.");
    send_message();
    delay(200);
  }
 else {
  Serial.println("Normal speed");
  delay(200);
  }
    }


// Function to send the message
void send_message()
{ 
  mySerial.println("AT"); //Once the handshake test is successful, it will back to OK
  updateSerial();
  mySerial.println("AT+CMGF=1"); // Configuring TEXT mode
  updateSerial();
 mySerial.println("AT+CMGS=\"+2349056479662\"");//change ZZ with country code and xxxxxxxxxxx with phone number to sms
  updateSerial();
  mySerial.print("Overspeeding!"); //text content
  updateSerial();
  mySerial.write(26);
}

void updateSerial()
{
  delay(500);
  while (Serial.available()) 
  {
    mySerial.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while(mySerial.available()) 
  {
    Serial.write(mySerial.read());//Forward what Software Serial received to Serial Port
  }
}
