
#include "Adafruit_CCS811.h"
#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display
#include <String.h>
#include <DHT.h> 
SoftwareSerial pmsSerial(4, 5);
SoftwareSerial gprsSerial(2,3); // transmit pin to D2, Receive pin to D3

Adafruit_CCS811 ccs;
const int S_analog = 1023.0;

int co,nh3;
float no2;
 
void setup() {
  // our debugging output
  gprsSerial.begin(9600);              // the GPRS baud rate
  pmsSerial.begin(9600);   
  Serial.begin(9600); 
   lcd.init();                      // initialize the lcd 
  lcd.backlight();
 
 
  // sensor baud rate is 9600
  pmsSerial.begin(9600);
}
 
struct pms5003data {
  // dust sensor line of code
  uint16_t framelen;
  uint16_t pm10_standard, pm25_standard, pm100_standard;
  uint16_t pm10_env, pm25_env, pm100_env;
  uint16_t particles_03um, particles_05um, particles_10um, particles_25um, particles_50um, particles_100um;
  uint16_t unused;
  uint16_t checksum;
};
 
struct pms5003data data;
    
void gassensors() {

}
 // dust sensor line of code
boolean readPMSdata(Stream *s) {
  if (! s->available()) {
    return false;
  }
  
  // Read a byte at a time until we get to the special '0x42' start-byte
  if (s->peek() != 0x42) {
    s->read();
    return false;
  }
 
  // Now read all 32 bytes
  if (s->available() < 32) {
    return false;
  }
    
  uint8_t buffer[32];    
  uint16_t sum = 0;
  s->readBytes(buffer, 32);
 
  // get checksum ready
  for (uint8_t i=0; i<30; i++) {
    sum += buffer[i];
  }
 
  /* debugging
  for (uint8_t i=2; i<32; i++) {
    Serial.print("0x"); Serial.print(buffer[i], HEX); Serial.print(", ");
  }
  Serial.println();
  */
  
  // The data comes in endian'd, this solves it so it works on all platforms
  uint16_t buffer_u16[15];
  for (uint8_t i=0; i<15; i++) {
    buffer_u16[i] = buffer[2 + i*2 + 1];
    buffer_u16[i] += (buffer[2 + i*2] << 8);
  }
 
  // put it into a nice struct :)
  memcpy((void *)&data, (void *)buffer_u16, 30);
 
  if (sum != data.checksum) {
    return false;
  }
  // success!
  return true;
}

void loop()   //I added thingspeak code for GSM module but it is in comment for further confirmation
{   
  
   co = map (analogRead(A0), 0, S_analog, 1, 1000);
  nh3 = map (analogRead(A1), 0, S_analog, 1, 500);
  no2 = map (analogRead(A2), 0, S_analog, 1, 1000)/ 100.0;
  
   Serial.print("Dust sensor: ");
    if (readPMSdata(&pmsSerial)) {
   Serial.print(data.pm25_standard);
    }
 Serial.print(" nh3: ");
 Serial.print(nh3);
 Serial.print(" no2: ");
 Serial.print(no2);
 Serial.print(" co: ");
 Serial.println(co);       
   
  if (gprsSerial.available())
    Serial.write(gprsSerial.read());
  gprsSerial.println("AT");
  delay(1000);
 
  gprsSerial.println("AT+CPIN?");
  delay(1000);
 
  gprsSerial.println("AT+CREG?");
  delay(1000);
 
  gprsSerial.println("AT+CGATT?");
  delay(1000);
 
  gprsSerial.println("AT+CIPSHUT");
  delay(1000);
 
  gprsSerial.println("AT+CIPSTATUS");
  delay(2000);
 
  gprsSerial.println("AT+CIPMUX=0");
  delay(2000);
 
  ShowSerialData();
 
  gprsSerial.println("AT+CSTT=\"glogprs.com\"");//start task and setting the APN,
  delay(1000);
 
  ShowSerialData();
 
  gprsSerial.println("AT+CIICR");//bring up wireless connection
  delay(3000);
 
  ShowSerialData();
 
  gprsSerial.println("AT+CIFSR");//get local IP adress
  delay(2000);
 
  ShowSerialData();
 
  gprsSerial.println("AT+CIPSPRT=0");
  delay(3000);
 
  ShowSerialData();
  
  gprsSerial.println("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",\"80\"");//start up the connection
  delay(6000);
 
  ShowSerialData();
 
  gprsSerial.println("AT+CIPSEND");//begin send data to remote server
  delay(4000);
  ShowSerialData();
  
String str="GET https://api.thingspeak.com/update?api_key=0AXMXG9PJ0K1T5NO&field1=" + String(co) +"&field2="+String(nh3) +"&field3="+String(no2)+"&field4="+String(data.pm10_standard);  Serial.println(str);
  gprsSerial.println(str);//begin send data to remote server
  
  delay(4000);
  ShowSerialData();
 
  gprsSerial.println((char)26);//sending
  delay(5000);//waitting for reply, important! the time is base on the condition of internet 
  gprsSerial.println();
 
  ShowSerialData();
 
  gprsSerial.println("AT+CIPSHUT");//close the connection
  delay(100);
  ShowSerialData();
} 

void ShowSerialData()
{
  while(gprsSerial.available()!=0)
  Serial.write(gprsSerial.read());
  delay(5000); 
  
}
