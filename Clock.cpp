/*
  Clock.cpp - Library for DIY Clock Kit.
  Created by Kato Gray, April 17, 2021.
  Released into the public domain.
*/

#include "Arduino.h"
#include "Clock.h"
#include "RTClib.h"
#include "SevSeg.h"
#include "Wire.h"
#include "DHT.h"

//#ifdef Standard_Pinout
#define SW1               0
#define SW2               1
#define SW3               2
#define SW4               3
#define Button1           4
#define Button2           5
#define Button3           6
#define Button4           7
#define Serial_RX         8
#define Serial_TX         9
#define DHT11_Pin         10
#define RGBRed            11
#define RGBGreen          12
#define RGBBlue           13
#define S7SEG_PIN_DECIMAL 14
#define S7SEG_PIN_A       15
#define I2C_SCL           16
#define I2C_SDA           17 
#define S7SEG_PIN_B       18
#define S7SEG_PIN_C       19
#define S7SEG_PIN_D       20
#define S7SEG_PIN_E       21
#define S7SEG_PIN_F       22
#define S7SEG_PIN_G       23      
#define S7SEG_PIN_DIG1    A0
#define S7SEG_PIN_DIG2    A1
#define S7SEG_PIN_DIG3    A2
#define S7SEG_PIN_DIG4    A3    
#define LDR               A4
#define POT1              A6
#define LED4              A5
#define Buzzer            A7
//#endif

int button1 = digitalRead(Button1);
int button2 = digitalRead(Button2);
int button3 = digitalRead(Button3);
int button4 = digitalRead(Button4);
int sw1 = digitalRead(SW1);
int sw2 = digitalRead(SW2);
int sw3 = digitalRead(SW3);
int sw4 = digitalRead(SW4);
int ldr = analogRead(LDR);
int pot = analogRead(POT1);

RTC_DS1307 rtc;
SevSeg sevseg;
DHT dht(DHT11_Pin, DHT11);

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

byte numDigits = 4;
byte digitPins[] = {S7SEG_PIN_DIG1, S7SEG_PIN_DIG2, S7SEG_PIN_DIG3, S7SEG_PIN_DIG4};
byte segmentPins[] = {S7SEG_PIN_A, S7SEG_PIN_B, S7SEG_PIN_C, S7SEG_PIN_D, S7SEG_PIN_E,   S7SEG_PIN_F, S7SEG_PIN_G, S7SEG_PIN_DECIMAL};
bool resistorsOnSegments = true; // 'false' means resistors are on digit pins
byte hardwareConfig = COMMON_ANODE; // See README.md for options
bool updateWithDelays = false; // Default 'false' is Recommended
bool leadingZeros = false; // Use 'true' if you'd like to keep the leading zeros
bool disableDecPoint = false; // Use 'true' if your decimal point doesn't exist or isn't connected

Clock::Clock(boolean printYesOrNo){
  _printYesOrNo = printYesOrNo;
}

void Clock::start(int brightnessLevel){
  if (_printYesOrNo == true){
    Serial.begin(9600);
  }
  Serial.println("Clock library, April 17, 2021");
  Serial.println("Setting pinModes...");
  pinMode(Button1, INPUT);
  pinMode(Button2, INPUT);
  pinMode(Button3, INPUT);
  pinMode(Button4, INPUT);
  pinMode(SW1, INPUT);
  pinMode(SW2, INPUT);
  pinMode(SW3, INPUT);
  pinMode(SW4, INPUT);
  pinMode(LDR, INPUT);
  pinMode(POT1, INPUT);
  pinMode(LED4, OUTPUT);
  pinMode(RGBRed, OUTPUT);
  pinMode(RGBGreen, OUTPUT);
  pinMode(RGBBlue, OUTPUT);
  pinMode(Buzzer, OUTPUT);
  Serial.println("Starting sevseg display...");
  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments, updateWithDelays, leadingZeros, disableDecPoint);
  Serial.println("Setting sevseg brightness...");
  sevseg.setBrightness(brightnessLevel);
  Serial.println("Starting DHT11...");
  dht.begin();
  Serial.println("Setup is complete!");
}

void Clock::setBrightness(int brightnessLevel){
  Serial.print("Set sevseg brightness to ");
  Serial.println(brightnessLevel);
  sevseg.setBrightness(brightnessLevel);
}
    
int Clock::getTime(){
  DateTime now = rtc.now();
  return((now.hour()*100)+now.minute());
}

int Clock::getDate(){
  DateTime now = rtc.now();
  return((now.month()*100)+now.day());
}

void Clock::set(int setNumber){
  Serial.print("Set sevseg display to ");
  Serial.println(setNumber);
  sevseg.setNumber(setNumber);
}

void Clock::display(){
  sevseg.refreshDisplay();
}

bool Clock::buttonPress(int buttonPressPin, int buttonPressAmount){
  if (buttonPressPin == HIGH){
    _count++;
    while (buttonPressPin == HIGH);
    delay(20);
    _Now = millis();    
  }
  while (!buttonPressPin == HIGH){
    delay(20);
    _time = _time+20;
  }
  if((_Now+_time)>= _Now+10000){
    _count=0;
  }
  if(_count == buttonPressAmount){
    buttonpresses = true;
  }
  else {
    buttonpresses = false;
  }
  return(buttonpresses);
}

bool Clock::buttonPressLength(int buttonPressLengthPin, int buttonPressLengthLength){
  if (!buttonPressLengthPin == HIGH){
    _pressLength = 0;
  }
  while (buttonPressLengthPin == HIGH){
    delay(20);
    _pressLength = _pressLength+20;
  }
  if (_pressLength == buttonPressLengthLength){
    return(true);
  }
  else {
    return(false);
  }
}

void Clock::alarmSet(int setalarmNumber, int setalarmfinishButton, int hourbutton, int minutebutton, int onbutton, int offbutton){
  if (setalarmNumber == 1){
    alarmset1:
    if (onbutton == HIGH){
      _alarm1 = true;
      sevseg.setNumber(10);
      sevseg.refreshDisplay();
    }
    if (offbutton == HIGH){
      _alarm1 = false;
      sevseg.setNumber(11);
      sevseg.refreshDisplay();
    }
    if (hourbutton == HIGH){
      _alarm1hour++;
      if (_alarm1hour == 24){
        _alarm1hour=23;
      }
      while(hourbutton == HIGH);
      delay(100);
    }
    if (minutebutton == HIGH){
      _alarm1minute++;
      if (_alarm1minute == 60){
        _alarm1minute = 59;
      }
      while(minutebutton == HIGH);
      delay(100);
    }
    sevseg.setNumber((_alarm1hour*100)+_alarm1minute);
    sevseg.refreshDisplay();
    while (setalarmfinishButton != HIGH){
      goto alarmset1;
    }
    while (setalarmfinishButton == HIGH);
    delay(100);
  }
  if (setalarmNumber == 2){
    alarmset2:
    if (onbutton == HIGH){
      _alarm2 = true;
      sevseg.setNumber(20);
      sevseg.refreshDisplay();
    }
    if (offbutton == HIGH){
      _alarm2 = false;
      sevseg.setNumber(21);
      sevseg.refreshDisplay();
    }
    if (hourbutton == HIGH){
      _alarm2hour++;
      if (_alarm2hour == 24){
        _alarm2hour=23;
      }
      while(hourbutton == HIGH);
      delay(100);
    }
    if (minutebutton == HIGH){
      _alarm2minute++;
      if (_alarm2minute == 60){
        _alarm2minute = 59;
      }
      while(minutebutton == HIGH);
      delay(100);
    }
    sevseg.setNumber((_alarm2hour*100)+_alarm2minute);
    sevseg.refreshDisplay();
    while (setalarmfinishButton != HIGH){
      goto alarmset2;
    }
    while (setalarmfinishButton == HIGH);
    delay(100);
  }
  if (setalarmNumber == 3){
    alarmset3:
    if (onbutton == HIGH){
      _alarm3 = true;
      sevseg.setNumber(30);
      sevseg.refreshDisplay();
    }
    if (offbutton == HIGH){
      _alarm3 = false;
      sevseg.setNumber(31);
      sevseg.refreshDisplay();
    }
    if (hourbutton == HIGH){
      _alarm3hour++;
      if (_alarm3hour == 24){
        _alarm3hour=23;
      }
      while(hourbutton == HIGH);
      delay(100);
    }
    if (minutebutton == HIGH){
      _alarm3minute++;
      if (_alarm3minute == 60){
        _alarm3minute = 59;
      }
      while(minutebutton == HIGH);
      delay(100);
    }
    sevseg.setNumber((_alarm3hour*100)+_alarm3minute);
    sevseg.refreshDisplay();
    while (setalarmfinishButton != HIGH){
      goto alarmset3;
    }
    while (setalarmfinishButton == HIGH);
    delay(100);
  }
  if (setalarmNumber == 4){
    alarmset4:
    if (onbutton == HIGH){
      _alarm4 = true;
      sevseg.setNumber(40);
      sevseg.refreshDisplay();
    }
    if (offbutton == HIGH){
      _alarm4 = false;
      sevseg.setNumber(41);
      sevseg.refreshDisplay();
    }
    if (hourbutton == HIGH){
      _alarm4hour++;
      if (_alarm4hour == 24){
        _alarm4hour=23;
      }
      while(hourbutton == HIGH);
      delay(100);
    }
    if (minutebutton == HIGH){
      _alarm4minute++;
      if (_alarm4minute == 60){
        _alarm4minute = 59;
      }
      while(minutebutton == HIGH);
      delay(100);
    }
    sevseg.setNumber((_alarm4hour*100)+_alarm4minute);
    sevseg.refreshDisplay();
    while (setalarmfinishButton != HIGH){
      goto alarmset4;
    }
    while (setalarmfinishButton == HIGH);
    delay(100);
  }
}

void Clock::ledWrite(int rgbred, int rgbgreen, int rgbblue){
  analogWrite(RGBRed, rgbred);
  analogWrite(RGBGreen, rgbgreen);
  analogWrite(RGBBlue, rgbblue);
}

void Clock::buzzerSequence(int buzzerSa, int buzzerSb, int buzzerSc, int buzzerSd, int buzzerSe, int buzzerSf, int buzzerSg, int buzzerSh){
  digitalWrite(Buzzer, HIGH);
  delay(buzzerSa*1000);
  digitalWrite(Buzzer, LOW);
  delay(buzzerSb*1000);
  digitalWrite(Buzzer, HIGH);
  delay(buzzerSc*1000);
  digitalWrite(Buzzer, LOW);
  delay(buzzerSd*1000);
  digitalWrite(Buzzer, HIGH);
  delay(buzzerSe*1000);
  digitalWrite(Buzzer, LOW);
  delay(buzzerSf*1000);
  digitalWrite(Buzzer, HIGH);
  delay(buzzerSg*1000);
  digitalWrite(Buzzer, LOW);
  delay(buzzerSh*1000);
}

void Clock::ledSequence(int ledSRed, int ledSGreen, int ledSBlue, int ledSa, int ledSb, int ledSc, int ledSd, int ledSe, int ledSf, int ledSg, int ledSh){
  ledWrite(ledSRed,ledSGreen,ledSBlue);
  delay(ledSa*1000);
  ledWrite(0,0,0);
  delay(ledSb*1000);
  ledWrite(ledSRed,ledSGreen,ledSBlue);
  delay(ledSc*1000);
  ledWrite(0,0,0);
  delay(ledSd*1000);
  ledWrite(ledSRed,ledSGreen,ledSBlue);
  delay(ledSe*1000);
  ledWrite(0,0,0);
  delay(ledSf*1000);
  ledWrite(ledSRed,ledSGreen,ledSBlue);
  delay(ledSg*1000);
  ledWrite(0,0,0);
  delay(ledSh*1000);
}

void Clock::buzzerledSequence(int buzledSRed, int buzledSGreen, int buzledSBlue, int buzledSa, int buzledSb, int buzledSc, int buzledSd, int buzledSe, int buzledSf, int buzledSg, int buzledSh){
  ledWrite(buzledSRed,buzledSGreen,buzledSBlue);
  digitalWrite(Buzzer, HIGH);
  delay(buzledSa*1000);
  ledWrite(0,0,0);
  digitalWrite(Buzzer, LOW);
  delay(buzledSb*1000);
  ledWrite(buzledSRed,buzledSGreen,buzledSBlue);
  digitalWrite(Buzzer, HIGH);
  delay(buzledSc*1000);
  ledWrite(0,0,0);
  digitalWrite(Buzzer, LOW);
  delay(buzledSd*1000);
  ledWrite(buzledSRed,buzledSGreen,buzledSBlue);
  digitalWrite(Buzzer, HIGH);
  delay(buzledSe*1000);
  ledWrite(0,0,0);
  digitalWrite(Buzzer, LOW);
  delay(buzledSf*1000);
  ledWrite(buzledSRed,buzledSGreen,buzledSBlue);
  digitalWrite(Buzzer, HIGH);
  delay(buzledSg*1000);
  ledWrite(0,0,0);
  digitalWrite(Buzzer, LOW);
  delay(buzledSh*1000);
}

void Clock::alarmArm(int armalarmNumber, boolean sleepButton, int sleepButtonPin, boolean buzzer, boolean led, int Red, int Green, int Blue, int a, int b, int c, int d, int e, int f, int g, int h){
  if (armalarmNumber == 1){
    DateTime now = rtc.now();
    if (_alarm1=true){
      if (_alarm1hour == now.hour()){
        if (_alarm1minute == now.minute()){
          if (sleepButton == true){
            while (sleepButtonPin != HIGH){
              if (led == false && buzzer == true){
                buzzerSequence(a, b, c, d, e, f, g, h);
              }
              if (led == true && buzzer == false){
                ledSequence(Red, Green, Blue, a, b, c, d, e, f, g, h);
              }
              if (led == true && buzzer == true){
                buzzerledSequence(Red, Green, Blue, a, b, c, d, e, f, g, h);
              }
            }
          }
          else if (sleepButton == false){
            if (led == false && buzzer == true){
              buzzerSequence(a, b, c, d, e, f, g, h);
            }
            if (led == true && buzzer == false){
              ledSequence(Red, Green, Blue, a, b, c, d, e, f, g, h);
            }
            if (led == true && buzzer == true){
              buzzerledSequence(Red, Green, Blue, a, b, c, d, e, f, g, h);
            }
          }
        }
      }
    }
  }
  if (armalarmNumber == 2){
    DateTime now = rtc.now();
    if (_alarm2=true){
      if (_alarm2hour == now.hour()){
        if (_alarm2minute == now.minute()){
          if (sleepButton == true){
            while (sleepButtonPin != HIGH){
              if (led == false && buzzer == true){
                buzzerSequence(a, b, c, d, e, f, g, h);
              }
              if (led == true && buzzer == false){
                ledSequence(Red, Green, Blue, a, b, c, d, e, f, g, h);
              }
              if (led == true && buzzer == true){
                buzzerledSequence(Red, Green, Blue, a, b, c, d, e, f, g, h);
              }
            }
          }
          else if (sleepButton == false){
            if (led == false && buzzer == true){
              buzzerSequence(a, b, c, d, e, f, g, h);
            }
            if (led == true && buzzer == false){
              ledSequence(Red, Green, Blue, a, b, c, d, e, f, g, h);
            }
            if (led == true && buzzer == true){
              buzzerledSequence(Red, Green, Blue, a, b, c, d, e, f, g, h);
            }
          }
        }
      }
    }
  }
  if (armalarmNumber == 3){
    DateTime now = rtc.now();
    if (_alarm3=true){
      if (_alarm3hour == now.hour()){
        if (_alarm3minute == now.minute()){
          if (sleepButton == true){
            while (sleepButtonPin != HIGH){
              if (led == false && buzzer == true){
                buzzerSequence(a, b, c, d, e, f, g, h);
              }
              if (led == true && buzzer == false){
                ledSequence(Red, Green, Blue, a, b, c, d, e, f, g, h);
              }
              if (led == true && buzzer == true){
                buzzerledSequence(Red, Green, Blue, a, b, c, d, e, f, g, h);
              }
            }
          }
          else if (sleepButton == false){
            if (led == false && buzzer == true){
              buzzerSequence(a, b, c, d, e, f, g, h);
            }
            if (led == true && buzzer == false){
              ledSequence(Red, Green, Blue, a, b, c, d, e, f, g, h);
            }
            if (led == true && buzzer == true){
              buzzerledSequence(Red, Green, Blue, a, b, c, d, e, f, g, h);
            }
          }
        }
      }
    }
  }
  if (armalarmNumber == 4){
    DateTime now = rtc.now();
    if (_alarm4=true){
      if (_alarm4hour == now.hour()){
        if (_alarm4minute == now.minute()){
          if (sleepButton == true){
            while (sleepButtonPin != HIGH){
              if (led == false && buzzer == true){
                buzzerSequence(a, b, c, d, e, f, g, h);
              }
              if (led == true && buzzer == false){
                ledSequence(Red, Green, Blue, a, b, c, d, e, f, g, h);
              }
              if (led == true && buzzer == true){
                buzzerledSequence(Red, Green, Blue, a, b, c, d, e, f, g, h);
              }
            }
          }
          else if (sleepButton == false){
            if (led == false && buzzer == true){
              buzzerSequence(a, b, c, d, e, f, g, h);
            }
            if (led == true && buzzer == false){
              ledSequence(Red, Green, Blue, a, b, c, d, e, f, g, h);
            }
            if (led == true && buzzer == true){
              buzzerledSequence(Red, Green, Blue, a, b, c, d, e, f, g, h);
            }
          }
        }
      }
    }
  }
}

void Clock::led(int ledPin, boolean ledType, int ledBrightness){
  if (ledType == 1){ //digital
    digitalWrite(ledPin, ledBrightness);
  }
  else if (ledType == 0){ //analog
    analogWrite(ledPin, ledBrightness);
  }
}

void Clock::buzzer(int buzPin, boolean buzzerStatus){
  digitalWrite(buzPin, buzzerStatus);
}

float Map(float val, boolean mapType, boolean inv){
  if (inv == true){
    if (mapType == 1){//analog
      map(val,1023,0,0,1023);
    }
    else if (mapType == 0){//pwm
      map(val,1023,0,0,255);
    }
  }
  else if (inv == false){
    if (mapType == 1){
      map(val,0,1023,0,1023);
    }
    else if (mapType == 0){
      map(val,0,1023,0,255);
    }
  }
}

float DHT11Temperature(boolean temperatureSet = true){
  if (temperatureSet == true){
    return(dht.readTemperature(true));
  }
  else {
    return(dht.readTemperature());
  }
}

float DHT11Humidity(){
  return(dht.readHumidity());
}

float DHT11HeatIndex(boolean index = true){
  if (index == true){
    return(dht.computeHeatIndex(dht.readTemperature(true),dht.readHumidity()));
  }
  else {
    return(dht.computeHeatIndex(dht.readTemperature(),dht.readHumidity(),false));
  }
}