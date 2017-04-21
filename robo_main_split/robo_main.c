#include <stdint.h>
#include <wiringPiI2C.h>
#include <wiringPi.h>
#include <stdlib.h>
#include <stdio.h>
#include "cap1188.h"
#include "pca9685.h"

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

///////// PROTOTYPES ///////////

// MAIN PROTOTYPES
void setup();
void react(int zoneActivated);
void moveFace(HeadPosition newPosition);
void LEDReaction(int ledIndex);
void setColor(int red, int green, int blue);
HeadPosition getZoneHeadPosition(int zoneActived);
int getLEDindex(int zoneActived);
void LEDDarken(int ledIndex);
void graduallyLight(int ledIndex);



// Variables
int fd_pwm = -1;
int fd_cap = -1;
int pulse;
int i;
int numOfTurns = 1;
volatile uint8_t touchedCaps;

// MAIN
int main() {

	/////// Set Up ////////
  setup();

	// Main Loop
	while (1) {
		delay(500);
		if (touched(fd_cap)) {
      printf("Touch detected on - %d\n", touchedCaps);
      i = 0;
			for (i; i < 8; i++) {
				if (touchedCaps & (1 << i)) {
          numOfTurns++;
          if (numOfTurns % 2 == 0) {
					  printf("%d touched, ", i);
					  react(i);
            printf("React Completed\n");
          }
				}
				printf("\n"); //flush write buffer so all touched buttons print to term
			}
		}
	}

}


///////// TOP LEVEL FUNCTIONS ///////////

void react(int zoneActivated) {

  zoneActivated++;

  // Get head position and LED index of activated zone
  HeadPosition newPosition = getZoneHeadPosition(zoneActivated);
  int ledIndex = getLEDindex(zoneActivated);

  printf("Zone: %d\n", zoneActivated);
  printf("headPosition: %d\n", newPosition.headPosition);
  printf("neckPosition: %d\n", newPosition.neckPosition);
  printf("led index: %d\n", ledIndex);

  LEDReaction(ledIndex);
  printf("LED's lit\n");

  moveFace(newPosition);
  printf("Face has been moved\n");
  LEDDarken(ledIndex);
  printf("LED's darkened\n");
  delay(300);
  HeadPosition middlePosition = {NECKMIDDLE, HEADSTRAIGHT};
  moveFace(middlePosition);
  printf("Face to middle\n");

}


void moveFace(HeadPosition newPosition) {
  //printf("move face called\n");
  setPWM(HEAD, 0, newPosition.headPosition, fd_pwm);
  setPWM(NECK, 0, newPosition.neckPosition, fd_pwm);
}


void LEDReaction(int ledIndex) {
  graduallyLight(ledIndex);
  printf("got out of LEDReaction\n");
}


void graduallyLight(int ledIndex) {
	uint16_t pulselen = LEDLOW;
	for (pulselen; pulselen < LEDHIGH; pulselen += 2) {
    setPWM(ledIndex, 0, pulselen, fd_pwm);
  }
}

void LEDDarken(int ledIndex) {
	uint16_t pulselen = LEDHIGH;
	for (pulselen; pulselen > LEDLOW; pulselen -= 2) {
    setPWM(ledIndex, 0, pulselen, fd_pwm);
  }
  setPWM(ledIndex, 0, 0, fd_pwm);
  printf("got out from darken for loop\n");
}

HeadPosition getZoneHeadPosition(int zoneActived) {
	HeadPosition newPosition = {0,0};
	switch (zoneActived) {
	    case ZONE1:
	      newPosition = ZONE1_DOWN;
	      break;
	    case ZONE2:
	      newPosition = ZONE2_DOWN;
	      break;
	    case ZONE3:
	      newPosition = ZONE3_DOWN;
	      break;
	    case ZONE4:
	      newPosition = ZONE4_DOWN;
	      break;
	    case ZONE5:
	      newPosition = ZONE5_DOWN;
	      break;
	    case ZONE6:
	      newPosition = ZONE6_DOWN;
	      break;
	    default:
	      break;
	}
	return newPosition;
}

int getLEDindex(int zoneActived) {
  int ledIndex = 14; // set to unused index
	switch (zoneActived) {
    case ZONE1:
      ledIndex = LED_1;
      break;
    case ZONE2:
      ledIndex = LED_2;
      break;
    case ZONE3:
      ledIndex = LED_3;
      break;
    case ZONE4:
      ledIndex = LED_4;
      break;
    case ZONE5:
      ledIndex = LED_5;
      break;
    case ZONE6:
      ledIndex = LED_6;
      break;
    default:
      break;
	}
	return ledIndex;
}

void setup() {

  // Setting up Wiring Pi
	if (wiringPiSetup() == -1) {
		printf("Wiring pi set up failure\n");
		exit(1);
	}

	// Set up pwm file descriptor (i2c communication)
	fd_pwm = wiringPiI2CSetup(PWMI2CADDRESS);
	printf("fd - %d\n", fd_pwm);
	if (fd_pwm  == -1) {
		printf("Could not find pca9685\n");
		exit(1);
	}

	// Set up file descriptor 1
	fd_cap = wiringPiI2CSetup(CAP_I2C_ADDRESS);
	printf("fd - %d\n", fd_cap);
	if (fd_cap  == -1) {
		printf("Could not find cap1188\n");
		exit(1);
	}

	/////////// POSSIBLE JUNK CODE ////

	//i2c test
	uint8_t oldmode = wiringPiI2CReadReg8(fd_pwm, PCA9685_MODE1);
	printf("old mode - %d\n", oldmode);

	resetPCA9685(fd_pwm);

	//i2c test
	oldmode = wiringPiI2CReadReg8(fd_pwm, PCA9685_MODE1);
	printf("old mode - %d\n", oldmode);

	////////////////////////////////////

	setPWMFreq(60, fd_pwm);

	cap1188info(fd_cap);
	setUpLEDTouch(fd_cap);

  printf("Set up complete\n");
}

///////// RGB LED FUNCTIONS ///////////

void LEDtransition() {
	i = 255;
	for (i; i <= 255; i++) {
		setColor(255,i,0);
		delay(10);
	}
	i = 255;
	for (i; i >= 0; i--) {
		setColor(i,255,0);
		delay(10);
	}
	i = 0;
	for (i; i <= 255; i++) {
		setColor(0,255,i);
		delay(10);
	}
	i = 255;
	for (i; i >= 0; i--) {
		setColor(0,i,255);
		delay(10);
	}
	i = 0;
	for (i; i <= 255; i++) {
		setColor(i,0,255);
		delay(10);
	}
	i = 255;
	for (i; i >= 0; i--) {
		setColor(255,0,i);
		delay(10);
	}
}


void setColor(int red, int green, int blue) {
	setPWM(LED_R, 0, red);
	setPWM(LED_G, 0, green);
	setPWM(LED_B, 0, blue);
}
