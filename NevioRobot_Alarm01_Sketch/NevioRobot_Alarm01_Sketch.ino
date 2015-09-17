#include <Arduino.h>
#include <Wire.h>
#include <Servo.h>
#include <SoftwareSerial.h>

//Added after mBlock
#include <EEPROM.h>
#include <RTClib.h>
//

#include "mBot.h"
#include "MePort.h"
#include "MeIR.h"
#include "MeRGBLed.h"
#include "MeBuzzer.h"
#include "MeLEDMatrix.h"
#include "MeUltrasonic.h"
#include "MeDCMotor.h"

// Added after mBlock
RTC_DS1307 RTC;
//

MeBoard myBoard(mBot);
double angle_rad = PI/180.0;
double angle_deg = 180.0/PI;
MeIR ir;
MeRGBLed rgbled_7(7);
void PlayYankeeSeconds(double iPlayYankTime);
double iPlayYankStart;
double iPlayYankRunner;
double currentTime = 0;
double lastTime = 0;
double getLastTime(){
    	return currentTime = millis()/1000.0 - lastTime;
}
void PlayYankeeNr(double iPlayYankNr);
void Play();
MeBuzzer buzzer;
void ShowTimeFnc(double nowHour, double nowMinute);
MeLEDMatrix ledMtx_3(3);
void ShowDateFnc(double nowDay, double nowMonth);
unsigned char drawBuffer[16];
unsigned char *drawTemp;
void DoWake(double wakeCounts, boolean withSound);
void Walk(double iWalkTime, double walkSpeed);
double iWalkLineStart;
double iWalkLineRunner;
double iUltraDistance;
double iWalkLineTemp;
MeUltrasonic ultrasonic_1(1);
MePort linefollower_2(2);
MeDCMotor motor(0);


void PlayYankeeSeconds(double iPlayYankTime)
{
    iPlayYankStart = getLastTime();    
    iPlayYankRunner = iPlayYankStart;
    
    while(!(((iPlayYankRunner) - (iPlayYankStart)) > (iPlayYankTime)))
    {
        Play();
        iPlayYankRunner = getLastTime();
    }    
}

void PlayYankeeNr(double iPlayYankNr)
{
    for(int i=0;i<iPlayYankNr;i++)
    {
        Play();
    }    
}

void Play()
{
    buzzer.tone(1047);    
    delay(1000*0.125);    
    buzzer.tone(1175);    
    delay(1000*0.125);    
    buzzer.tone(1319);    
    delay(1000*0.25);    
    buzzer.tone(1568);    
    delay(1000*0.25);    
    buzzer.tone(1568);    
    delay(1000*0.25);    
    buzzer.tone(1760);    
    delay(1000*0.25);    
    buzzer.tone(1568);    
    delay(1000*0.25);    
    buzzer.tone(1319);    
    delay(1000*0.25);    
    buzzer.tone(1047);    
    delay(1000*0.5);    
    buzzer.tone(1175);    
    delay(1000*0.125);    
    buzzer.tone(1319);    
    delay(1000*0.25);    
    buzzer.tone(1319);    
    delay(1000*0.25);    
    buzzer.tone(1175);    
    delay(1000*0.25);    
    buzzer.tone(1047);    
    delay(1000*0.25);    
    buzzer.tone(1175);    
    delay(1000*1);    
    buzzer.noTone();    
    delay(1000*2);    
}

void ShowTimeFnc(double nowHour, double nowMinute)
{
    ledMtx_3.clearScreen();
    ledMtx_3.setColorIndex(1);
    ledMtx_3.setBrightness(6);
    ledMtx_3.showClock(nowHour,nowMinute,strcmp(":",":")==0);    
    delay(2000*1);    
}

void ShowDateFnc(double nowDay, double nowMonth)
{
    ledMtx_3.clearScreen();
    ledMtx_3.setColorIndex(1);
    ledMtx_3.setBrightness(6);
    drawTemp = new unsigned char[16]{0,0,64,72,68,66,2,2,2,2,66,68,72,64,0,0};
    memcpy(drawBuffer,drawTemp,16);
    free(drawTemp);
    ledMtx_3.drawBitmap(0,0,16,drawBuffer);
    
    delay(1000*1);
    
}

void DoWake(double wakeCounts, boolean withSound)
{
    for(int i=0;i<wakeCounts;i++)
    {
        PlayYankeeSeconds(17);
        if (withSound) {
          Walk(13,60);
        }
    }
    
}

void Walk(double iWalkTime, double walkSpeed)
{
    iPlayYankRunner = 100;    
    iWalkLineStart = getLastTime();    
    iWalkLineRunner = iWalkLineStart;
    
    while(!(((iWalkLineRunner) - (iWalkLineStart)) > (iWalkTime)))
    {
        iUltraDistance = ultrasonic_1.distanceCm();
        if((iUltraDistance) > (15)){
            iWalkLineTemp = linefollower_2.dRead1()*2+linefollower_2.dRead2();
            if(((iWalkLineTemp)==( 0 ))){
                motor.move(1,walkSpeed);
            }else{
                if(((iWalkLineTemp)==(1))){
                    motor.move(3,(walkSpeed) * (2));
                }else{
                    if(((iWalkLineTemp)==(2))){
                        motor.move(4,(walkSpeed) * (2));
                    }else{
                        motor.move(2,(walkSpeed) * (2));
                    }
                }
            }
        }else{
            motor.move(1,0);
        }
        iWalkLineRunner = getLastTime();
    }    
    
    motor.move(1,0);    
}


void setup(){
  
    // Added after mBlock
    Serial.begin(9600);
    Serial.println("Serial started");
    
    Wire.begin();
    RTC.begin();
    if (! RTC.isrunning()) {
      Serial.println("RTC is NOT running!");
      // following line sets the RTC to the date & time this sketch was compiled
      RTC.adjust(DateTime(__DATE__, __TIME__));
    }

    EEPROM.update(1, 6);
    EEPROM.update(2, 6);
    EEPROM.update(3, 6);
    EEPROM.update(4, 2);
    EEPROM.update(5, 6);
    EEPROM.update(6, 99);
    EEPROM.update(7, 99);
    
    EEPROM.update(8, 45);
    EEPROM.update(9, 45);
    EEPROM.update(10, 45);
    EEPROM.update(11, 01);
    EEPROM.update(12, 45);
    EEPROM.update(13, 99);
    EEPROM.update(14, 99);
    //
    
    ir.begin();
    rgbled_7.setNumber(7==7?2:16);
}

void loop(){
    
    // Added after mBlock
    DateTime now = RTC.now();
    uint8_t dow = now.dayOfWeek();
    uint8_t hrs = now.hour();
    uint8_t mins = now.minute();

    int wHrs = EEPROM.read(dow);
    int wMins = EEPROM.read(dow + 7);

    if (hrs == wHrs && mins == wMins) {
        boolean withSound = true;
        for (int wRep = 0; wRep < 4; wRep ++) {
          
            DoWake(1, withSound);
            //if (ir.keyPressed(69)) {
            if ((0^(analogRead(A7)>10?0:1))) {
                rgbled_7.setColor(1,255,0,0);
                rgbled_7.show();
                withSound = false;
            }
            //ir.loop();    
        }
    } else {
      rgbled_7.clear();
    }
    //
    
    ShowTimeFnc(hrs,mins);
    ShowDateFnc(1,1);    
}

