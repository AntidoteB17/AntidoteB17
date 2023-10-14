// water temperature sensor A1
// Turbidity sensor A3
// Surrounding temperature A5
// pH sensor A0
//Ultrasonic sensor (trigger D2, echo D3)

#include <SoftwareSerial.h>
SoftwareSerial gprsSerial(4, 5); // transmit pin to D2, Receive pin to D3
//
//
#include <LiquidCrystal_I2C.h>
#include <String.h>
#include <DHT.h>
#include <EEPROM.h>
#include <DallasTemperature.h>
#include <SoftwareSerial.h>


#include <Wire.h>
#define ONE_WIRE_BUS A1
#define DHTPIN 25
OneWire oneWire(ONE_WIRE_BUS);
DHT dht(DHTPIN, DHT11);
LiquidCrystal_I2C lcd(0x27, 20, 4);
DallasTemperature sensors(&oneWire);

float ph_act, turbidity;
long duration, inches;
int set_val, percentage;
bool state, pump;
int phval = 0; // the pH meter Analog output is connected with the Arduino’s A0
unsigned long int avgval; //Store the average value of the sensor feedback
int buffer_arr[10], temp;
float calibration_value = 21.85;


#define sensor_pin A3
int read_ADC;
int ntu;

int trigpin = 2;
int echopin = 3;
#include <String.h>
#include <DHT.h>




void setup()
{
  gprsSerial.begin(9600);               // the GPRS baud rate
  Serial.begin(9600);    // the GPRS baud rate
 
  dht.begin();
  Serial.print("     Nano GSM data to cloud............");
  pinMode(trigpin, OUTPUT);
  pinMode(echopin, INPUT);
  pinMode(sensor_pin, INPUT);



  set_val = EEPROM.read(0);
  if (set_val > 100)set_val = 100 ;
  delay(1000);
}

void loop()
{
  long duration,distance;
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  delay(100);

  digitalWrite(trigpin, HIGH);
   delayMicroseconds(10);
   digitalWrite(trigpin, LOW);
   duration = pulseIn(echopin, HIGH);
 distance= (duration/2) / 29.1;
 percentage = (34-distance)* 100/29.1;
   read_ADC = analogRead(sensor_pin);
   
   if(percentage<0)percentage=0;
   
    if (percentage < 0)percentage = 0;
  Serial.println(percentage);

  
  // surrending temperature sensor line of code
  int reading = analogRead(A5);

  // converting that reading to voltage,
  float voltage = reading * 5.0;
  voltage /= 1024.0;
  float temperatureC = (voltage - 0.5) * 100 ;

//Turbidity sensor line of code 
  if(read_ADC>208)read_ADC=208;

  ntu = map(read_ADC, 0, 208, 300, 0);

  //pH sensor line of code
  for (int i = 0; i < 10; i++) //Get 10 sample value from the sensor for smooth the value
  {
    buffer_arr[i] = analogRead(A0);
    delay(30);
  }
  for (int i = 0; i < 9; i++) //sort the analog from small to large
  {
    for (int j = i + 1; j < 10; j++)
    {
      if (buffer_arr[i] > buffer_arr[j])
      {
        temp = buffer_arr[i];
        buffer_arr[i] = buffer_arr[j];
        buffer_arr[j] = temp;
      }
    }
  }
  avgval = 0;
  for (int i = 2; i < 8; i++) //take the average value of 6 center sample
    avgval += buffer_arr[i];
  float volt = (float)avgval * 5.0 / 1024 / 6; //convert the analog into millivolt
    float ph_act = -5.7 * volt + calibration_value; //convert the millivolt into pH value
 //convert the millivolt into pH value

  if(read_ADC>208)read_ADC=208;

  ntu = map(read_ADC, 0, 208, 300, 0);

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

  String str = "GET https://api.thingspeak.com/update?api_key=H4YSN9F6O672HICE&field1=" + String(sensors.getTempCByIndex(0)) + "&field2=" + String(ntu) + "&field3=" + String(ph_act) + "&field4=" + String(percentage);

  Serial.println(str);
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
  while (gprsSerial.available() != 0)
    Serial.write(gprsSerial.read());
  delay(5000);

}

void watersensor() {
  long duration,distance;
  sensors.requestTemperatures();
  read_ADC = analogRead(sensor_pin);
  // Analog input for ultraasonic sensor water level
  digitalWrite(trigpin, HIGH);
   delayMicroseconds(10);
   digitalWrite(trigpin, LOW);
   duration = pulseIn(echopin, HIGH);
 distance= (duration/2) / 29.1;
 percentage = (34-distance)* 100/29.1;
   if(percentage<0)percentage=0;
   Serial.println(percentage);
delay(2000);





  // surrounding temperature sensor line of code
  int reading = analogRead(A5);
  float voltage = reading * 5.0;
  voltage /= 1024.0;


  // now print out the surrounding temperature
  float temperatureC = (voltage - 0.5) * 100 ;//converting from 10 mv per degree wit 500 mV offset
  //to degrees ((voltage - 500mV) times 100)

// turbidity sensor line of code
  if(read_ADC>208)read_ADC=208;

  ntu = map(read_ADC, 0, 208, 300, 0);


  //pH sensor line of code
  for (int i = 0; i < 10; i++) //Get 10 sample value from the sensor for smooth the value
  {
    buffer_arr[i] = analogRead(A0);
    delay(30);
  }
  for (int i = 0; i < 9; i++) //sort the analog from small to large
  {
    for (int j = i + 1; j < 10; j++)
    {
      if (buffer_arr[i] > buffer_arr[j])
      {
        temp = buffer_arr[i];
        buffer_arr[i] = buffer_arr[j];
        buffer_arr[j] = temp;
      }
    }
  }
  avgval = 0;
  for (int i = 2; i < 8; i++) //take the average value of 6 center sample
    avgval += buffer_arr[i];
  float volt = (float)avgval * 5.0 / 1024 / 6; //convert the analog into millivolt
    float ph_act = -5.7 * volt + calibration_value; //convert the millivolt into pH value
//convert the millivolt into pH value



}
long microsecondsToInches(long microseconds) {
  return microseconds / 29 / 2;

}
