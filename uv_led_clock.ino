#include <Wire.h>
#include "RTClib.h"

RTC_DS1307 rtc;
char daysOfTheWeek[7][12] = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};

char font8x8_basic[11][8] = {
    { 0x3E, 0x63, 0x73, 0x7B, 0x6F, 0x67, 0x3E, 0x00},   // U+0030 (0)
    { 0x0C, 0x0E, 0x0C, 0x0C, 0x0C, 0x0C, 0x3F, 0x00},   // U+0031 (1)
    { 0x1E, 0x33, 0x30, 0x1C, 0x06, 0x33, 0x3F, 0x00},   // U+0032 (2)
    { 0x1E, 0x33, 0x30, 0x1C, 0x30, 0x33, 0x1E, 0x00},   // U+0033 (3)
    { 0x38, 0x3C, 0x36, 0x33, 0x7F, 0x30, 0x78, 0x00},   // U+0034 (4)
    { 0x3F, 0x03, 0x1F, 0x30, 0x30, 0x33, 0x1E, 0x00},   // U+0035 (5)
    { 0x1C, 0x06, 0x03, 0x1F, 0x33, 0x33, 0x1E, 0x00},   // U+0036 (6)
    { 0x3F, 0x33, 0x30, 0x18, 0x0C, 0x0C, 0x0C, 0x00},   // U+0037 (7)
    { 0x1E, 0x33, 0x33, 0x1E, 0x33, 0x33, 0x1E, 0x00},   // U+0038 (8)
    { 0x1E, 0x33, 0x33, 0x3E, 0x30, 0x18, 0x0E, 0x00},   // U+0039 (9)
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}    // U+003A ( ) (space)
};


#define IN1  A0 //BLUE
#define IN2  A2 //YELLOW
#define IN3  A1 //ORANGE
#define IN4  A3 //PINK
/*
#define IN1  A3 //ORANGE
#define IN2  A1 //PINK
#define IN3  A2 //BLUE
#define IN4  A0 //YELLOW
*/


//RTC (i2c)
//SCL A5
//SDA A4


#define LED0  2
#define LED1  3
#define LED2  4
#define LED3  5
#define LED4  6
#define LED5  7
#define LED6  8
#define LED7  9
#define LED8  10
#define LED9  11
#define LEDGND  12


int led_count = 0;
int step_count = 0;


int write_time = false;

void setup() {
  delay(1000);
  while (!Serial);
  Serial.begin(9600);
  
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    //rtc.adjust(DateTime(2018, 1, 29, 21, 03, 0)); //rtc.adjust(DateTime(2017, 5, 14, 23, 13, 0));
  }
  
  pinMode(IN1, OUTPUT); 
  pinMode(IN2, OUTPUT); 
  pinMode(IN3, OUTPUT); 
  pinMode(IN4, OUTPUT); 

  pinMode(LED0, OUTPUT); 
  pinMode(LED1, OUTPUT); 
  pinMode(LED2, OUTPUT); 
  pinMode(LED3, OUTPUT); 
  pinMode(LED4, OUTPUT); 
  pinMode(LED5, OUTPUT); 
  pinMode(LED6, OUTPUT); 
  pinMode(LED7, OUTPUT); 
  pinMode(LED8, OUTPUT); 
  pinMode(LED9, OUTPUT); 
  pinMode(LEDGND, OUTPUT); 

  digitalWrite(LEDGND, LOW);

}
int this_hour = 0;
int this_minute = 0;

int char_pos = 0;
int line_pos = 0;
int this_char_pos = 0;
int this_line_pos = 0;

void loop() {
  
  DateTime now = rtc.now();
  this_hour = now.hour();
  this_minute = now.minute();

  if(this_hour>12){this_hour=this_hour-12;}
  Serial.print("TIME: ");
  Serial.print(this_hour, DEC);
  Serial.print(':');
  Serial.print(this_minute, DEC);
  Serial.println();

  
    
  for(int a=0; a<4608; a++){
    step_count++;
    if(step_count>8){ step_count=0; }
    steppper(step_count);

    switch(a){
      case 80: char_pos=0; write_time = true; break;
      case 1000: char_pos=1; write_time = true; break;
      case 2500: char_pos=2; write_time = true; break;
      case 3500: char_pos=3; write_time = true; break;
    }
    if(write_time && a%100==0){
      output_line();
    }
    delay(5);
  }
  delay(10000);
}


void output_line(){
  if(write_time && line_pos>7){
    line_pos = 0;
    write_time = false;
    led_off();
  }else{
    
    int hour_tens = this_hour/10;
    int hour_ones = this_hour-hour_tens*10;
  
    int minute_tens = this_minute/10;
    int minute_ones = this_minute-minute_tens*10;
    bool reverse_it = false;
    
    switch(char_pos){
      case 2: 
        if(this_hour<10){
          this_char_pos = 10;
        }else{
          this_char_pos = hour_tens;
        }
        this_line_pos = line_pos;
        break;
      case 3: 
        this_char_pos = hour_ones;
        this_line_pos = line_pos;
        break;
      case 1: 
        if(this_minute<10){
          this_char_pos = 0;
        }else{
          this_char_pos = minute_tens;
        }
        this_line_pos = 7-line_pos;
        reverse_it=true;
        break;
      case 0: 
        this_char_pos = minute_ones;
        this_line_pos = 7-line_pos;
        reverse_it=true;
        break;
      
    }
    
    //int this_char_pos = (longtime.substring(char_pos,1)).toInt();
    /*Serial.print(this_char_pos);
    Serial.print(":");
    Serial.println(line_pos);*/
    
    for(int a=0; a<8; a++){
      switch(a){
        case 0: digitalWrite(LED0,bitRead( font8x8_basic[this_char_pos][this_line_pos], reverse_it?7-a:a)); break;
        case 1: digitalWrite(LED1,bitRead( font8x8_basic[this_char_pos][this_line_pos], reverse_it?7-a:a)); break;
        case 2: digitalWrite(LED2,bitRead( font8x8_basic[this_char_pos][this_line_pos], reverse_it?7-a:a)); break;
        case 3: digitalWrite(LED3,bitRead( font8x8_basic[this_char_pos][this_line_pos], reverse_it?7-a:a)); break;
        case 4: digitalWrite(LED4,bitRead( font8x8_basic[this_char_pos][this_line_pos], reverse_it?7-a:a)); break;
        case 5: digitalWrite(LED5,bitRead( font8x8_basic[this_char_pos][this_line_pos], reverse_it?7-a:a)); break;
        case 6: digitalWrite(LED6,bitRead( font8x8_basic[this_char_pos][this_line_pos], reverse_it?7-a:a)); break;
        case 7: digitalWrite(LED7,bitRead( font8x8_basic[this_char_pos][this_line_pos], reverse_it?7-a:a)); break;
      }
    }
    line_pos++;
  }
}


void led_off(){
  digitalWrite(LED0, LOW);
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
  digitalWrite(LED4, LOW);
  digitalWrite(LED5, LOW);
  digitalWrite(LED6, LOW);
  digitalWrite(LED7, LOW);
  digitalWrite(LED8, LOW);
  digitalWrite(LED9, LOW);
}
void led_on(){
  digitalWrite(LED0, HIGH);
  digitalWrite(LED1, HIGH);
  digitalWrite(LED2, HIGH);
  digitalWrite(LED3, HIGH);
  digitalWrite(LED4, HIGH);
  digitalWrite(LED5, HIGH);
  digitalWrite(LED6, HIGH);
  digitalWrite(LED7, HIGH);
  digitalWrite(LED8, HIGH);
  digitalWrite(LED9, HIGH);
}
void led(int num){
  digitalWrite(LED0, LOW);
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
  digitalWrite(LED4, LOW);
  digitalWrite(LED5, LOW);
  digitalWrite(LED6, LOW);
  digitalWrite(LED7, LOW);
  digitalWrite(LED8, LOW);
  digitalWrite(LED9, LOW);
  

  switch(num){
   case 0: digitalWrite(LED0, HIGH); break;
   case 1: digitalWrite(LED1, HIGH); break;
   case 2: digitalWrite(LED2, HIGH); break;
   case 3: digitalWrite(LED3, HIGH); break;
   case 4: digitalWrite(LED4, HIGH); break;
   case 5: digitalWrite(LED5, HIGH); break;
   case 6: digitalWrite(LED6, HIGH); break;
   case 7: digitalWrite(LED7, HIGH); break;
   case 8: digitalWrite(LED8, HIGH); break;
   case 9: digitalWrite(LED9, HIGH); break;
  }
}

void steppper(int Steps){
  switch(Steps){
   case 0:
     analogWrite(IN1, 0); 
     analogWrite(IN2, 0);
     analogWrite(IN3, 0);
     analogWrite(IN4, 255);
   break; 
   case 1:
     analogWrite(IN1, 0); 
     analogWrite(IN2, 0);
     analogWrite(IN3, 255);
     analogWrite(IN4, 255);
   break; 
   case 2:
     analogWrite(IN1, 0); 
     analogWrite(IN2, 0);
     analogWrite(IN3, 255);
     analogWrite(IN4, 0);
   break; 
   case 3:
     analogWrite(IN1, 0); 
     analogWrite(IN2, 255);
     analogWrite(IN3, 255);
     analogWrite(IN4, 0);
   break; 
   case 4:
     analogWrite(IN1, 0); 
     analogWrite(IN2, 255);
     analogWrite(IN3, 0);
     analogWrite(IN4, 0);
   break; 
   case 5:
     analogWrite(IN1, 255); 
     analogWrite(IN2, 255);
     analogWrite(IN3, 0);
     analogWrite(IN4, 0);
   break; 
     case 6:
     analogWrite(IN1, 255); 
     analogWrite(IN2, 0);
     analogWrite(IN3, 0);
     analogWrite(IN4, 0);
   break; 
   case 7:
     analogWrite(IN1, 255); 
     analogWrite(IN2, 0);
     analogWrite(IN3, 0);
     analogWrite(IN4, 255);
   break; 
}
}

