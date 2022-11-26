#define bt 7 //button at digital pin 7
#define ld 5 //red led at digital pin 5
#define xd 8 //yellow led at digital pin 8
#define gr 3 //green led at digital pin 3

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F,20,4); //set I2C lcd 16x2 address 

String decode(String x){
   String morse[] = {".-","-...", "-.-.", "-..", ".","..-.","--.", "....","..", ".---", "-.-", ".-..", "--", "-.", "---", ".--.","--.-",".-.", "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--..","-----",".----","..---","...--","....-",".....","-....","--...","---..","----.",".-.-.-","--..--","..--..","-...-","----"};
   int i = 0;
   String ou{"ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.,?= "};
    
   while (i < sizeof(morse)){
       if (x == morse[i]){
          return String(ou[i]);
       }
       i++;
   }
   return "";
} //function to decode morse code.


void setup(){
    pinMode(bt,INPUT_PULLUP); //set pinMode button
    pinMode(ld,OUTPUT); //set red led pinMode
    pinMode(xd,OUTPUT);
    pinMode(gr,OUTPUT);
    Serial.begin(57600);
    lcd.init();
    lcd.backlight();
    lcd.setCursor(0,0);
}

long skip_now; //outside loop variable to determine interval
bool skip_on = false; //bool to check if button pressed or not
String code = ""; //outside loop variable to store clicked morse
int c1 = 0; //lcd coordinate 
int c2 = 0;

void loop(){
    String add_code = ""; //outside while loop to store 1 morse code
    long now = millis(); //variable to determine how long the button pressed
    while (!digitalRead(bt)){
        long end = millis();
        long z = end - now;
        
        if (z > 200){
           digitalWrite(xd,HIGH);
           digitalWrite(ld,LOW);
           add_code = "-";
        }
        else {
           digitalWrite(xd,LOW);
           digitalWrite(ld,HIGH);
           add_code = ".";
        }
        skip_on = true; //telling the program that the button is pressed
        skip_now = millis();
        
    } //loop to check if button is pressed
    code += add_code; //storing 1 morse code
    digitalWrite(xd,LOW); //turning off all the lcd
    digitalWrite(ld,LOW);
    
    if (skip_on){
       if (millis() - skip_now >= 500){
          skip_on = false; //resetting the last button pressed.
          digitalWrite(gr,HIGH);
          delay(100);
          digitalWrite(gr,LOW);
          String oup = decode(code); //decoding all the stored morse code
          Serial.print(oup); //writing to monitor
          lcd.print(oup); //printing to the I2C LCD.
          code = ""; //resetting the stored morse code.
          if (c1 < 16){
             c1++;
          }
          else{
              c2++;
              c1=0;
          } //incrementing the lcd coordinate.
       } //checking if the interval of last button pressed >= 500ms
    } //checking if the button has pressed
   
    
}
