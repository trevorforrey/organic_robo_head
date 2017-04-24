#ifndef _ROBOMAIN_
#define _ROBOMAIN_

// Head Position Type
typedef struct HeadPosition {
    int neckPosition;
    int headPosition;
}HeadPosition;

// Directions based on head's perspective
// Pulse length counts
#define HEADUP 225
#define HEADDOWN 400
#define HEADSTRAIGHT 325

#define NECKRIGHT 150
#define NECKLEFT 500
#define NECKMIDDLE 330

#define NECKZONE1 500
#define NECKZONE2 441
#define NECKZONE3 383
#define NECKZONE4 266
#define NECKZONE5 208
#define NECKZONE6 150


///////// LED DEFINES ////////////////
#define LEDLOW 0
#define LEDHIGH 1000

//////// ZONE DEFINES ////////////////
#define ZONE1 1
#define ZONE2 2
#define ZONE3 3
#define ZONE4 4
#define ZONE5 5
#define ZONE6 6

////// PWM index positions ///////////
#define NECK 0
#define HEAD 1
#define LED_1 2
#define LED_2 3
#define LED_3 4
#define LED_4 5
#define LED_5 6
#define LED_6 7
#define LED_R 8
#define LED_G 9
#define LED_B 10

// MAIN PROTOTYPES
void setup();
void react(int zoneActivated);
HeadPosition getZoneHeadPosition(int zoneActived);
int getLEDindex(int zoneActived);
void moveFace(HeadPosition newPosition);
void LEDReaction(int ledIndex);
void LEDDarken(int ledIndex);
void graduallyLight(int ledIndex);




#endif
