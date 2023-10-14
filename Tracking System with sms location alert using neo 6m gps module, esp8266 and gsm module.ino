#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>
 #include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
/*
   This sample sketch demonstrates the normal use of a TinyGPSPlus (TinyGPSPlus) object.
   It requires the use of SoftwareSerial, and assumes that you have a
   4800-baud serial GPS device hooked up on pins 4(rx) and 3(tx).
*/
#define rxGPS D3
#define txGPS D8
#define rxGSM D5
#define txGSM D6
SoftwareSerial sim800(rxGSM, txGSM);
#define buttonPin D7
#define buttonPin2 D4

#define ledpin D0

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


int buttonState = 0;
int buttonState2 = 0;
static const uint32_t GPSBaud = 9600;

// The TinyGPSPlus object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(rxGPS,txGPS);

const unsigned long eventInterval = 1800000;// millis best timer for 30 mins
unsigned long previousTime = 0;

void setup()
{
 pinMode(buttonPin, INPUT);
pinMode(buttonPin2, INPUT);
pinMode(ledpin,OUTPUT);
  Serial.begin(9600);
  ss.begin(GPSBaud);
  sim800.begin(9600);
   sim800.println("AT"); //Once the handshake test is successful, it will back to OK
  updateSerial();

  Serial.println(F("DeviceExample.ino"));
  Serial.println(F("A simple demonstration of TinyGPSPlus with an attached GPS module"));
  Serial.print(F("Testing TinyGPSPlus library v. ")); Serial.println(TinyGPSPlus::libraryVersion());
  Serial.println(F("by Mikal Hart"));
  Serial.println();
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }
  delay(2000);
  display.clearDisplay();
  display.setTextColor(WHITE);
}

void loop()
{
  int buttonState = digitalRead(buttonPin);
  int buttonState2 = digitalRead(buttonPin2);

  unsigned long currentTime = millis();

if (buttonState == HIGH & currentTime - previousTime >= eventInterval){
send_message();
digitalWrite(ledpin, HIGH);
previousTime = currentTime;
}
else if (buttonState == LOW){
  send_message();
digitalWrite(ledpin, LOW);  
}
if (buttonState2 == LOW){
send_message2();
digitalWrite(ledpin, HIGH);
}
  // This sketch displays information every time a new sentence is correctly encoded.
  while (ss.available() > 0)
    if (gps.encode(ss.read()))
      displayInfo();

  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    while(true);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Latitude: ");
  display.setTextSize(2);
  display.setCursor(0, 10);
  display.print(gps.location.lat(), 6);
  

  display.setTextSize(1);
  display.setCursor(0, 33);
  display.print("Longitude: ");
  display.setTextSize(2);
  display.setCursor(0, 45);
  display.print(gps.location.lng(), 6);

  display.display();
}

void displayInfo()
{
  Serial.print(F("Location ")); 
  if (gps.location.isValid())
  {
    Serial.print(F("Lat.: ")); 
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(",Long.:"));
    Serial.print(gps.location.lng(), 6);
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F("  Date/Time: "));
  if (gps.date.isValid())
  {
    Serial.print(gps.date.month());
    Serial.print(F("/"));
    Serial.print(gps.date.day());
    Serial.print(F("/"));
    Serial.print(gps.date.year());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F(" "));
  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10) Serial.print(F("0"));
    Serial.print(gps.time.hour());
    Serial.print(F(":"));
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(F(":"));
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());
    Serial.print(F("."));
    if (gps.time.centisecond() < 10) Serial.print(F("0"));
    Serial.print(gps.time.centisecond());
  }
  else
  {
    Serial.print(F("INVALID"));
  }
    
  Serial.println();
}

void send_message()
{ 
  sim800.println("AT"); //Once the handshake test is successful, it will back to OK
  updateSerial();
  sim800.println("AT+CMGF=1"); // Configuring TEXT mode
  updateSerial();
 sim800.println("AT+CMGS=\"+2348153486696\"");//change ZZ with country code and xxxxxxxxxxx with phone number to sms
  updateSerial();
  sim800.print("http://maps.google.com/maps?q=loc:");
    sim800.print(gps.location.lat(), 6);
    sim800.print(",");
    sim800.print(gps.location.lng(), 6);
    delay(1000);
     Serial.println("GPS Location SMS Sent Successfully.");
  updateSerial();
  sim800.write(26);
}

void send_message2()
{ 
  sim800.println("AT"); //Once the handshake test is successful, it will back to OK
  updateSerial();
  sim800.println("AT+CMGF=1"); // Configuring TEXT mode                                                                                                                                                                                                                                BB BBBBBBBBBBBBB BBBBBBBBBBBBBBBBB
  updateSerial();
 sim800.println("AT+CMGS=\"+2348132117038\"");//change ZZ with country code and xxxxxxxxxxx with phone number to sms
  updateSerial();
  sim800.print("http://maps.google.com/maps?q=loc:");
    sim800.print(gps.location.lat(), 6);
    sim800.print(",");
    sim800.print(gps.location.lng(), 6);
    delay(1000);
     Serial.println("GPS Location SMS Sent Successfully.");
  updateSerial();
  sim800.write(26);
}
void updateSerial()
{
  delay(500);
  while (Serial.available()) 
  {
    sim800.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while(sim800.available()) 
  {
    Serial.write(sim800.read());//Forward what Software Serial received to Serial Port
  }
}
