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
boolean withSound = true;
//MePort lightsensor_8(8);
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
void DoWake(double wakeCounts);
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
        if ((0^(analogRead(A7)>10?0:1)))
          withSound = false;
        
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
    if (withSound) {
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

void DoWake(double wakeCounts)
{
    for(int i=0;i<wakeCounts;i++)
    {
        PlayYankeeSeconds(30);
        Walk(30,60);
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

    // Wake hours
    EEPROM.update(0, 7);
    EEPROM.update(1, 6);
    EEPROM.update(2, 6);
    EEPROM.update(3, 6);
    EEPROM.update(4, 6);
    EEPROM.update(5, 6);
    EEPROM.update(6, 7);

    // Wake minutes
    EEPROM.update(7, 15);
    EEPROM.update(8, 40);
    EEPROM.update(9, 40);
    EEPROM.update(10, 40);
    EEPROM.update(11, 40);
    EEPROM.update(12, 40);
    EEPROM.update(13, 15);

    // "FaceMode" can be only set for all days
    // Start
    EEPROM.update(15, 6);
    EEPROM.update(16, 10);
    // End
    EEPROM.update(17, 21);
    EEPROM.update(18, 59);
    //
    
    ir.begin();
    rgbled_7.setNumber(7==7?2:16);
}

void loop(){
    
    // Added after mBlock
    DateTime now = RTC.now();
    uint16_t years = now.year();
    uint8_t months = now.month();
    uint8_t days = now.day();

//    Serial.print(years, DEC);
//    Serial.print('-');
//    Serial.print(months, DEC);
//    Serial.print('-');
//    Serial.print(days, DEC);
//    Serial.println();

    uint8_t dow = now.dayOfWeek();
    uint8_t hrs = now.hour();
    uint8_t mins = now.minute();
    uint32_t uxNow = now.unixtime();

    int wHrs = EEPROM.read(dow);
    int wMins = EEPROM.read(dow + 7);

    if (hrs == wHrs && mins == wMins) {
        for (int wRep = 0; wRep < 5; wRep ++) {
          
            rgbled_7.setColor(1,255,204,0);
            rgbled_7.show();
            DoWake(1);
            //if (ir.keyPressed(69)) { }
            //ir.loop();    
        }
    } else {
        withSound = true;
        rgbled_7.clear();

        int qStartHrs = EEPROM.read(15);
        int qStartMins = EEPROM.read(16);
        int qEndHrs = EEPROM.read(17);
        int qEndMins = EEPROM.read(18);

//        Serial.print(qStartHrs, DEC);
//        Serial.print('-');
//        Serial.print(qStartMins, DEC);
//        Serial.print('-');
//        Serial.print(qEndHrs, DEC);
//        Serial.print('-');
//        Serial.print(qEndMins, DEC);
//        Serial.println();

        // starting face
        DateTime dtStartFace = DateTime(years, months, days, qStartHrs, qStartMins);
        uint32_t uxStartFace = dtStartFace.unixtime();
        // .... to ending face
        DateTime dtEndFace = DateTime(years, months, days, qEndHrs, qEndMins);
        uint32_t uxEndFace = dtEndFace.unixtime();

//        Serial.print(uxStartFace, DEC);
//        Serial.print('-');
//        Serial.print(uxNow, DEC);
//        Serial.print('-');
//        Serial.print(uxEndFace, DEC);
//        Serial.println();

        if ((uxStartFace < uxNow && uxNow <= uxEndFace)) {
            ShowTimeFnc(hrs,mins);
            ShowDateFnc(1,1);    
        } else {
            ledMtx_3.clearScreen();
        }
        
//        Light Sensor on our mBot seems NOT to work (no matter if dark or shine on with torch-light) 
//        double lightVal = lightsensor_8.aRead2();
//        Serial.println(lightVal);
    }
    //    
}

