#include <LiquidCrystal.h>
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
int ADC_a = A0; //Analog Input
int ADC_Y = A1;
int ADC_B = A2;
float VIn_a = 0.0; //Voltage In after voltage divider
float VInY = 0.0;
float VInB = 0.0;
float Voltage_a = 0.0;
//Actual voltage after calculation
float VoltageY = 0.0;
float VoltageB= 0.0;
float CalVal = 11.08433; //Voltage divider calibration value
float AC_LOW_VOLT = 0.0;
float AC_HIGH_VOLT = 0.0;
float AC_LOW_VOLTY = 0.0;
float AC_HIGH_VOLTY = 0.0;
float AC_LOW_VOLTB = 0.0;
float AC_HIGH_VOLTB = 0.0;
//Timing
unsigned long previousMillis = 0;
const long interval = 200;
//Interval to read voltages
// input pin declearation
const int IBEDCIn = 13;
const int GENERATORIn = 10;
const int SOLARIn = 6;
// output pin declearation
const int IBEDCOut = 8;
const int GENERATOROut = 7;
const int SOLAROut = 9;
// variable for reading input phase status. This is keepin all input at 0 state
bool IBEDCState = false;
bool GENERATORState = false;
bool SOLARState = false;
bool allMAINSOn = false;
void setup() {
pinMode(IBEDCIn, INPUT);
pinMode(GENERATORIn, INPUT);
pinMode(SOLARIn, INPUT);
pinMode(IBEDCOut, OUTPUT);
pinMode(GENERATOROut, OUTPUT);
pinMode(SOLAROut, OUTPUT);
pinMode(A0,INPUT);
pinMode(A1,INPUT);
pinMode(A2,INPUT);
//Init serial monitor. This will help display what is happening on the serial monitor
Serial.begin(9600);
Serial.println("3MAINS AUTOMATIC CHANGE OVER SYSTEM");
delay (2000);
Serial.println("Processing available MAINS.....");
// initialize the LCD
lcd.begin(16, 2);
lcd.print("3MAINS POWER SYS!");
delay(2000);
lcd.setCursor(0,1);
lcd.print("Initializing sys");
delay(2000);
}
void loop() {
unsigned long currentMillis = millis();
//checking for the state of each input
int IBEDCState = digitalRead(IBEDCIn);
int GENERATORState = digitalRead(GENERATORIn);
int SOLARState = digitalRead(SOLARIn);
VIn_a = (ADC_a * 5.00) / 1024.00;
//Convert 10bit input to an actual voltage
Voltage_a = (VIn_a * CalVal)-0.10;
AC_LOW_VOLT = (Voltage_a / 1.414 );
AC_HIGH_VOLT = ( AC_LOW_VOLT * 18.333);
VInY = (ADC_Y * 5.00) / 1024.00;
//Convert 10bit input to an actual voltage
VoltageY = (VInY * CalVal)-0.10;
AC_LOW_VOLTY = (VoltageY / 1.414 );
AC_HIGH_VOLTY = ( AC_LOW_VOLTY * 18.333);
VInB = (ADC_B * 5.00) / 1024.00;
//Convert 10bit input to an actual voltage
VoltageB = (VInB * CalVal)-0.10;
AC_LOW_VOLTB = (VoltageB / 1.414 );
AC_HIGH_VOLTB = ( AC_LOW_VOLTB * 18.333);
if ( IBEDCState == HIGH && GENERATORState == HIGH && SOLARState == HIGH &&
currentMillis - previousMillis >= interval && allMAINSOn == false )
{
allMAINSOn = true;
previousMillis = currentMillis;
analogRead (A0);
Serial.println("All MAINS are On");
lcd.clear();
lcd.setCursor (0,0); // set to line 1, char 0
lcd.print("3MAINS CHANGEOVER SYS");
delay (2000);
lcd.setCursor (0,1); // set to line 1, char 0
lcd.print("All MAINS are ON!");
digitalWrite ( IBEDCOut,HIGH);
digitalWrite ( GENERATOROut,LOW);
digitalWrite ( SOLAROut,LOW);
delay (2000);
lcd.clear();
lcd.setCursor (0,1); // set to line 1, char 0
lcd.print("IBEDC IN USE");
lcd.setCursor (0,1); // set to line 1, char 0
lcd.print(AC_HIGH_VOLT);
lcd.setCursor(6,1);
lcd.print("V input");
}
else{
if ( IBEDCState == HIGH )
{
Serial.println("IBEDC IS AVAILABLE");
IBEDCState = true;
analogRead (A0);
lcd.clear();
lcd.setCursor (0,0); // set to line 1, char 0
lcd.print("MAINS SUPPLY 1");
delay (2000);
digitalWrite ( IBEDCOut,HIGH);
digitalWrite ( GENERATOROut,LOW);
digitalWrite ( SOLAROut,LOW);
delay (2000);
lcd.clear();
lcd.setCursor (0,1); // set to line 1, char 0
lcd.print("IBEDC IN USE");
lcd.setCursor (0,1); // set to line 1, char 0
lcd.print(AC_HIGH_VOLT);
lcd.setCursor(6,1);
lcd.print("V input");
}
else if ( GENERATORState == HIGH )
{
GENERATORState = true;
Serial.println("GENERATOR IS ON!");
analogRead (A1);
lcd.clear();
lcd.setCursor (0,0); // set to line 1, char 0
lcd.print("MAINS SUPPLY 2");
delay (2000);
lcd.setCursor (0,1); // set to line 1, char 0
lcd.print("GENERATOR IN USE");
digitalWrite ( IBEDCOut,LOW);
digitalWrite ( GENERATOROut,HIGH);
digitalWrite ( SOLAROut,LOW);
delay (2000);
lcd.clear();
lcd.setCursor (0,1); // set to line 1, char 0
lcd.print("GENERATOR IN USE");
lcd.setCursor (0,1); // set to line 1, char 0
lcd.print(AC_HIGH_VOLTY);
lcd.setCursor(6,1);
lcd.print("V input");
}
else if ( SOLARState == HIGH)
{
SOLARState = true;
Serial.println("SOLAR IS ON");
analogRead (A2);
lcd.clear();
lcd.setCursor (0,0); // set to line 1, char 0
lcd.print("MAINS SUPPLY 3");
delay (2000);
lcd.setCursor (0,1); // set to line 1, char 0
lcd.print("SOLAR IN USE");
digitalWrite ( IBEDCOut,LOW);
digitalWrite ( GENERATOROut,LOW);
digitalWrite ( SOLAROut,HIGH);
delay (2000);
lcd.clear();
lcd.setCursor (0,1); // set to line 1, char 0
lcd.print("SOLAR IN USE");
lcd.setCursor (0,1); // set to line 1, char 0
lcd.print(AC_HIGH_VOLTB);
lcd.setCursor(6,1);
lcd.print("V input");
}
else
{
allMAINSOn = false;
Serial.println("All MAINS are off");
lcd.clear();
lcd.setCursor (0,0); // set to line 1, char 0
lcd.print("3MAINS CHANGEOVER SYS");
delay (2000);
lcd.setCursor (0,1); // set to line 1, char 0
lcd.print("ALL MAINS OFF!");
digitalWrite ( IBEDCOut,LOW);
digitalWrite ( GENERATOROut,LOW);
digitalWrite ( SOLAROut,LOW);

        }
   }
}