#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

typedef struct HeadPosition {
    int neckPosition;
    int headPosition;
}HeadPosition;

// Directions based on head's perspective
// Pulse length counts
#define HEADUP   225
#define HEADDOWN   400
#define HEADSTRAIGHT 340

#define NECKRIGHT  150
#define NECKLEFT  550 

// mapping degrees to PWM value
// start at right, go towards left
#define NECKZONE1 map(170, 0, 180, 150, 550)
#define NECKZONE2 map(160, 0, 180, 150, 550)
#define NECKZONE3 map(150, 0, 180, 150, 550)
#define NECKZONE4 map(140, 0, 180, 150, 550)
#define NECKZONE5 map(130, 0, 180, 150, 550)
#define NECKZONE6 map(120, 0, 180, 150, 550)
#define NECKZONE7 map(110, 0, 180, 150, 550)
#define NECKZONE8 map(100, 0, 180, 150, 550)
#define NECKZONE9 map(90, 0, 180, 150, 550)
#define NECKZONE10 map(80, 0, 180, 150, 550)
#define NECKZONE11 map(70, 0, 180, 150, 550)
#define NECKZONE12 map(60, 0, 180, 150, 550)
#define NECKZONE13 map(50, 0, 180, 150, 550)
#define NECKZONE14 map(40, 0, 180, 150, 550)
#define NECKZONE15 map(30, 0, 180, 150, 550)
#define NECKZONE16 map(20, 0, 180, 150, 550)

#define ZONEZERO 0
#define ZONEEIGHT 8

#define LEDLOW 0
#define LEDHIGH 1000

// PWM positions on PCA 9685
#define NECK 0
#define HEAD 1
#define LED1 2
#define LED2 3
#define LED3 4
#define LED4 5
#define LED5 6
#define LED6 7
#define LED7 8
#define LED8 9
#define LED9 10

// Head position structs
HeadPosition RIGHT_DOWN = {NECKRIGHT,HEADDOWN};
HeadPosition LEFT_DOWN = {NECKLEFT,HEADDOWN};
HeadPosition RIGHT_STRAIGHT = {NECKRIGHT,HEADSTRAIGHT};
HeadPosition LEFT_STRAIGHT = {NECKLEFT,HEADSTRAIGHT};
HeadPosition ZONE1_DOWN = {NECKZONE1,HEADDOWN};
HeadPosition ZONE2_DOWN = {NECKZONE2,HEADDOWN};
HeadPosition ZONE3_DOWN = {NECKZONE3,HEADDOWN};
HeadPosition ZONE4_DOWN = {NECKZONE4,HEADDOWN};
HeadPosition ZONE5_DOWN = {NECKZONE5,HEADDOWN};
HeadPosition ZONE6_DOWN = {NECKZONE6,HEADDOWN};
HeadPosition ZONE7_DOWN = {NECKZONE7,HEADDOWN};
HeadPosition ZONE8_DOWN = {NECKZONE8,HEADDOWN};
HeadPosition ZONE9_DOWN = {NECKZONE9,HEADDOWN};
HeadPosition ZONE10_DOWN = {NECKZONE10,HEADDOWN};
HeadPosition ZONE11_DOWN = {NECKZONE11,HEADDOWN};
HeadPosition ZONE12_DOWN = {NECKZONE12,HEADDOWN};
HeadPosition ZONE13_DOWN = {NECKZONE13,HEADDOWN};
HeadPosition ZONE14_DOWN = {NECKZONE14,HEADDOWN};
HeadPosition ZONE15_DOWN = {NECKZONE15,HEADDOWN};
HeadPosition ZONE16_DOWN = {NECKZONE16,HEADDOWN};

// default address 0x40 used
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

void moveFace(HeadPosition newPosition) {
  pwm.setPWM(HEAD, 0, newPosition.headPosition);
  pwm.setPWM(NECK, 0, newPosition.neckPosition);  
}

HeadPosition getZoneHeadPosition(int zone) {
  HeadPosition newPosition = {0,0};
  switch (zone) {
    case 1: 
      newPosition = ZONE1_DOWN;
      break;
    case 2:
      newPosition = ZONE2_DOWN;
      break;
    case 3:
      newPosition = ZONE3_DOWN;
      break;
    case 4:
      newPosition = ZONE4_DOWN;
      break;
    case 5:
      newPosition = ZONE5_DOWN;
      break;
    case 6:
      newPosition = ZONE6_DOWN;
      break;
    case 7:
      newPosition = ZONE7_DOWN;
      break;
    case 8:
      newPosition = ZONE8_DOWN;
      break;
    case 9:
      newPosition = ZONE9_DOWN;
      break;
    case 10:
      newPosition = ZONE10_DOWN;
      break;
    case 11:
      newPosition = ZONE11_DOWN;
      break;
    case 12:
      newPosition = ZONE12_DOWN;
      break;
    case 13:
      newPosition = ZONE13_DOWN;
      break;
    case 14:
      newPosition = ZONE14_DOWN;
      break;
    case 15:
      newPosition = ZONE15_DOWN;
      break;
    case 16:
      newPosition = ZONE16_DOWN;
      break;
  }

  return newPosition;
}

int LEDReaction(int zone) {
  // Will do logic after more construction of project
  switch (zone) {
    case 1:
      for (uint16_t pulselen = 0; pulselen < 1000; pulselen++) {
        pwm.setPWM(LED1, 0, pulselen);
      }
      break;
    default:
      break;
  }  
  return 0;
}

void react() {
  // Get zone, calculate head position and led chain to activate
  int zoneActive = Serial.read();
  HeadPosition newPosition = getZoneHeadPosition(zoneActive);

  LEDReaction(zoneActive);
  delay(500); // delay could make it seem like the face reacted to the light ??
  moveFace(newPosition);
}

void setup() {
  
  Serial.begin(9600);
  Serial.println("Humanoid Robot Project!");

  pwm.begin();
  
  pwm.setPWMFreq(60);  // 60 Hz updates

  yield();
  
}

void loop() {

  if (Serial.available() > 0) {
    react();  
  }
  delay(800);
}
