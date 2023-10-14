#include <IRremote.h>
#include <EEPROM.h>
#include <SoftwareSerial.h>
#include <DHT.h>;
//I2C LCD:
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
LiquidCrystal_I2C lcd(0x27,16,2); // set the LCD address to 0x27 for a 16 chars and 2 line display
SoftwareSerial mySerial(7,6); //SIM800L Tx & Rx is connected to Arduino #4 & #3

//Constants
#define DHTPIN 2     // what pin we're connected to
#define DHTTYPE DHT11   // DHT 11
#define Relay2 11 // Load2 Pin Out
#define Relay3 12 // Load3 Pin Out
#define touch2 A1 // Lamp Button Pin in 
#define touch3 A2 // Socket Button Pin in 
#define touch5 A3 // Power Button Pin in

int buzzer = 5; //buzzer pin
int load2, load3,power;
int flag2, flag3,flag5;
int timer=0;

DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino
int h;  //Stores humidity value
int t; //Stores temperature value
int f;
int GASA0 = A0;
int gasvalue;
int flame_sensor = 3;
int flame_pin = HIGH;
const int RECV_PIN = 4;
IRrecv irrecv(RECV_PIN);
decode_results results;
unsigned long key_value = 0;

void setup()
{    
  Serial.begin(9600);
    Serial.println("Sensor based Smart Home");
    delay(3000);
   mySerial.begin(9600);
   mySerial.println("AT"); //Once the handshake test is successful, it will back to OK
  updateSerial();
  irrecv.enableIRIn();
  irrecv.blink13(true);
    dht.begin();
    lcd.init(); //initialize the lcd
    lcd.backlight(); //open the backlight
    pinMode (flame_sensor, INPUT);
    pinMode(touch2, INPUT);
pinMode(touch3, INPUT);
pinMode(touch5, INPUT);
pinMode(buzzer, OUTPUT);

pinMode(Relay2, OUTPUT); digitalWrite(Relay2, 1); 
pinMode(Relay3, OUTPUT); digitalWrite(Relay3, 1); 

load2 = EEPROM.read(2);
load3 = EEPROM.read(3);
power = EEPROM.read(5);
delay(500); 
}
void loop()
{
    flame_pin = digitalRead(flame_sensor);
    int analogSensor = analogRead(GASA0);
    int gasvalue=(analogSensor-50)/35;       //gas module sensitivity
    h = dht.readHumidity();
    t = dht.readTemperature();   
    f = t * (9.0/5.0) + 32.0;
    //Print temp and humidity values to serial monitor
    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print(" %, Temp: ");
    Serial.print(t);
    Serial.print(f);
    Serial.println(" °Celsius");
    
    // print on LCD line of code

    lcd.setCursor(0, 1);
    lcd.print("Tem:");
    lcd.print(t);
    lcd.print("C");
    lcd.setCursor(8, 1);
    lcd.print("Hum.:");
    lcd.print(h);
    lcd.print("%");

    lcd.setCursor(0,0);
  lcd.print("Cook.Gas:");
  lcd.setCursor(9,0);
  lcd.print(gasvalue);
  lcd.setCursor(11,0);
  lcd.print("%");

  if (gasvalue >= 10)              //gas percentage alert
  {
    lcd.clear();    
    send_message_gas();
    lcd.setCursor(0,1); 
  lcd.print("GAS LEAKAGE!!!");
  digitalWrite(buzzer,HIGH);
  delay(5000);
  digitalWrite(buzzer,LOW);
   lcd.clear(); 

  }
  if(t>60){
    lcd.clear();
    send_message_temp();
    lcd.setCursor(0,1);  
  lcd.print("HIGH ROOM TEMP.!!!");
        digitalWrite(buzzer,HIGH);
    delay(5000);
       digitalWrite(buzzer,LOW); 
        lcd.clear();
      
  }
      if (flame_pin == LOW){
  Serial.println("FLAME FLAME FLAME");
  lcd.clear();   
  send_message_flame();
      lcd.setCursor(5, 0);
    lcd.print("FLAME!!!");
    digitalWrite(buzzer,HIGH);
    delay(5000);
       digitalWrite(buzzer,LOW);   
         lcd.clear();

 }
 else{
  Serial.println("no flame  ");
 }
 
if(digitalRead(touch2)==1){ 
if(flag2==0){flag2=1; 
load2 = !load2; 
EEPROM.write(2, load2); 
delay(100);
 }
}else{flag2=0;}

if(digitalRead(touch3)==1){ 
if(flag3==0){flag3=1; 
load3 = !load3; 
EEPROM.write(3, load3); 
delay(100);
 }
}else{flag3=0;}

if(digitalRead(touch5)==1){ 
if(flag5==0){flag5=1; 
power = !power; 
EEPROM.write(5, power); 
delay(100);
 }
}else{flag5=0;}
 
if(power==1){  
digitalWrite(Relay2, 1); 
digitalWrite(Relay3, 1); 
}
else{
digitalWrite(Relay2, load2); 
digitalWrite(Relay3, load3); 
}

if(irrecv.decode(&results)){
 Serial.println(results.value,HEX);
  
     if(results.value==0xFF30CF){ // For Load1 On  
Serial.print("Load 1 ON");  
load2 = 0; 
EEPROM.write(2, load2);
}
else if(results.value==0xFF18E7){ // For Load1 Off 
Serial.print("Load 1 OFF"); 
load2 = 1; 
EEPROM.write(2, load2); 
}

else if(results.value==0xFF10EF){ // For Load2 On 
Serial.print("Load 2 ON");  
load3 = 0; 
EEPROM.write(3, load3); 
}
else if(results.value==0xFF38C7){ // For Load2 Off 
Serial.print("Load 2 OFF"); 
load3 = 1; 
EEPROM.write(3, load3);   
}
else if(results.value==0xFF42BD){ // For Load3 On 
Serial.print("Load 3 ON");
power = 0; 
EEPROM.write(5, power);  
}
else if(results.value==0xFF4AB5){ // For Load3 Off 
Serial.print("Load 3 OFF");  
power = 1; 
EEPROM.write(5, power);
}   
 irrecv.resume(); // Receive the next value
 delay(100);
 }
}

// Function to send the message
void send_message_gas()
{ 
  mySerial.println("AT"); //Once the handshake test is successful, it will back to OK
  updateSerial();
  mySerial.println("AT+CMGF=1"); // Configuring TEXT mode
  updateSerial();
 mySerial.println("AT+CMGS=\"+2349151463779\"");//change ZZ with country code and xxxxxxxxxxx with phone number to sms
  updateSerial();
  mySerial.print("DANGER! Cooking Gas is Leaking"); //text content
  updateSerial();
  mySerial.write(26);
}

void send_message_flame()
{ 
  mySerial.println("AT"); //Once the handshake test is successful, it will back to OK
  updateSerial();
  mySerial.println("AT+CMGF=1"); // Configuring TEXT mode
  updateSerial();
 mySerial.println("AT+CMGS=\"+2349151463779\"");//change ZZ with country code and xxxxxxxxxxx with phone number to sms
  updateSerial();
  mySerial.print("OPEN FLAME DETECTED!!!"); //text content
  updateSerial();
  mySerial.write(26);
}

void send_message_temp()
{ 
  mySerial.println("AT"); //Once the handshake test is successful, it will back to OK
  updateSerial();
  mySerial.println("AT+CMGF=1"); // Configuring TEXT mode
  updateSerial();
 mySerial.println("AT+CMGS=\"+2349151463779\"");//change ZZ with country code and xxxxxxxxxxx with phone number to sms
  updateSerial();
  mySerial.print("Room temperature is HIGH!!!"); //text content
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
