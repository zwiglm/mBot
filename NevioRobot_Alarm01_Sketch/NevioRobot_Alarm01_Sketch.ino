
#include <Arduino.h>
#include <Wire.h>
#include <Servo.h>
#include <SoftwareSerial.h>
#include <EEPROM.h>

#include "mBot.h"
#include "MePort.h"
MeBoard myBoard(mBot);
#include "MeIR.h"
#include "MeRGBLed.h"
#include "MeBuzzer.h"
#include "MeLEDMatrix.h"
#include "MeUltrasonic.h"
#include "MeDCMotor.h"

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
void Do(double wakeCounts);
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
    delay(1000*1);    
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

void Do(double wakeCounts)
{
    for(int i=0;i<wakeCounts;i++)
    {
        PlayYankeeSeconds(20);
        Walk(10,60);
        PlayYankeeSeconds(20);
        Walk(10,60);
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
    ir.begin();
    rgbled_7.setNumber(7==7?2:16);
    Do(2);    
}

void loop(){
    
    if(ir.keyPressed(69)){
        rgbled_7.setColor(1,255,0,0);
        rgbled_7.show();
    }else{
        rgbled_7.setColor(1,0,255,0);
        rgbled_7.show();
    }
    ShowTimeFnc(10,22);
    ShowDateFnc(1,1);
    
    ir.loop();    
    ir.loop();    
    ir.loop();    
    ir.loop();    
    ir.loop();    
    ir.loop();    
    ir.loop();    
    ir.loop();        
}

