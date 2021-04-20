/*
  Clock.h - Library for DIY Clock Kit.
  Created by Kato Gray, April 17, 2021.
  Released into the public domain.
*/
#ifndef Clock_h
#define Clock_h

#include "Arduino.h"

class Clock
{
  public:
    Clock(boolean printYesOrNo);
    void start(int brightnessLevel);
    void setBrightness(int brightnessLevel);
    int getTime();
    int getDate();
    void set(int setNumber);
    void display();
    bool buttonPress(int buttonPressPin, int buttonPressAmount);
    bool buttonPressLength(int buttonPressLengthPin, int buttonPressLengthLength);
    void alarmSet(int setalarmNumber, int setalarmfinishButton, int hourbutton, int minutebutton, int onbutton, int offbutton);
    void alarmArm(int armalarmNumber, boolean sleepButton, int sleepButtonPin, boolean buzzer, boolean led, int Red, int Green, int Blue, int a, int b, int c, int d, int e, int f, int g, int h);
    void ledWrite(int rgbred, int rgbgreen, int rgbblue);
    void buzzerSequence(int buzzerSa, int buzzerSb, int buzzerSc, int buzzerSd, int buzzerSe, int buzzerSf, int buzzerSg, int buzzerSh);
    void ledSequence(int ledSRed, int ledSGreen, int ledSBlue, int ledSa, int ledSb, int ledSc, int ledSd, int ledSe, int ledSf, int ledSg, int ledSh);
    void buzzerledSequence(int buzledSRed, int buzledSGreen, int buzledSBlue, int buzledSa, int buzledSb, int buzledSc, int buzledSd, int buzledSe, int buzledSf, int buzledSg, int buzledSh);
    void led(int ledPin, boolean ledType, int ledBrightness);
    void buzzer(int buzPin, boolean buzzerStatus);
    float Map(float val, boolean mapType, boolean inv = false);
    float DHT11Temperature(boolean temperatureSet = true);
    float DHT11Humidity();
    float DHT11HeatIndex(boolean index = true);
  private:
    int _Now=0;
    int _count=0;
    int _time=0;
    int _pressLength=0;
    boolean _alarm1=false;
    boolean _alarm2=false;
    boolean _alarm3=false;
    boolean _alarm4=false;
    int _alarm1hour=0;
    int _alarm2hour=0;
    int _alarm3hour=0;
    int _alarm4hour=0;
    int _alarm1minute=0;
    int _alarm2minute=0;
    int _alarm3minute=0;
    int _alarm4minute=0;
    int _printYesOrNo;
    int buttonpresses;
};
#endif