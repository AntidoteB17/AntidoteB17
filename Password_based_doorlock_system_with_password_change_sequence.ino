#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include<SoftwareSerial.h>
#include<EEPROM.h>
#define BULB 6
#define BULBTEST 7
#define buzzer A4
LiquidCrystal_I2C lcd(0x27,16,2);
char password[4];
char pass[4],pass1[4];
int i=0;
char customKey=0;
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns

SoftwareSerial mySerial(2,3); // (Rx,Tx > Tx,Rx) char incomingByte; String inputString

char incomingByte; 
String inputString;

char hexaKeys[ROWS][COLS] = {
{'1','2','3','A'},
{'4','5','6','B'},
{'7','8','9','C'},
{'*','0','#','D'}
};
byte rowPins[ROWS] = {10,11,12,13}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {A0,A1, 8, 9}; //connect to the column pinouts of the keypad
//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);
void setup()
{
 lcd.init(); //initialize the lcd
    lcd.backlight(); //open the backlight
pinMode(BULB , OUTPUT);
pinMode(BULBTEST, OUTPUT);
Serial.begin(9600);
mySerial.begin(9600);
pinMode(buzzer, OUTPUT);
//  pinMode(m12, OUTPUT);
lcd.print("Password based");
lcd.setCursor(0,1);
lcd.print("Circuit breaker");
delay(2000);
lcd.clear();
lcd.print("Enter Your Passkey:");
lcd.setCursor(0,1);
    
for(int j=0;j<4;j++)
EEPROM.write(j, j+49);
for(int j=0;j<4;j++)
pass[j]=EEPROM.read(j);

 while(!mySerial.available()){ mySerial.println("AT"); 
    delay(1000); Serial.println("Connecting...");
     }
 Serial.println("Connected!"); mySerial.println("AT+CMGF=1"); //Set SMS to Text Mode delay(1000); mySerial.println("AT+CNMI=1,2,0,0,0"); //Procedure to handle newly arrived messages(command name in text: new message indications to TE) delay(1000); mySerial.println("AT+CMGL=\"REC UNREAD\""); // Read Unread Messages 
            
   
}

void loop()
{
customKey = customKeypad.getKey();
if(customKey=='#')
change();
if (customKey)
{
password[i++]=customKey;
lcd.print(customKey);
}
if(i==4)
{
delay(200);
for(int j=0;j<4;j++)
pass[j]=EEPROM.read(j);
if(!(strncmp(password, pass,4)))
{
digitalWrite(BULBTEST, HIGH);
delay (10000);
lcd.clear();
lcd.print("Passkey Accepted");
delay(2000);
lcd.setCursor(0,1);
lcd.print("#.Change Passkey");
delay(2000);
lcd.clear();
lcd.print("Enter Passkey:");
lcd.setCursor(0,1);
i=0;
digitalWrite(BULB, LOW);
digitalWrite(BULBTEST, LOW);
}
else
{
digitalWrite(buzzer, HIGH);
lcd.clear();
lcd.print("Access Denied...");
lcd.setCursor(0,1);
lcd.print("#.Change Passkey");
delay(2000);
lcd.clear();
lcd.print("Enter Passkey:");
lcd.setCursor(0,1);
i=0;
digitalWrite(buzzer, LOW);
}
}
}
void change()
{
int j=0;
lcd.clear();
lcd.print("UR Current Passk");
lcd.setCursor(0,1);
while(j<4)
{
char key=customKeypad.getKey();
if(key)
{
pass1[j++]=key;
lcd.print(key);
}
key=0;
}
delay(500);
if((strncmp(pass1, pass, 4)))
{
lcd.clear();
lcd.print("Wrong Passkey...");
lcd.setCursor(0,1);
lcd.print("Try Again");
delay(1000);
}
else
{
j=0;
lcd.clear();
lcd.print("Enter New Passk:");
lcd.setCursor(0,1);
while(j<4)
{
char key=customKeypad.getKey();
if(key)
{
pass[j]=key;
lcd.print(key);
EEPROM.write(j,key);
j++;
}
}
lcd.print(" Done......");
delay(1000);
}
lcd.clear();
lcd.print("Enter Ur Passk:");
lcd.setCursor(0,1);
customKey=0;


}
