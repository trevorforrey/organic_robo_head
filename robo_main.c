#include <stdint.h>
#include <wiringPiI2C.h>
#include <wiringPi.h>
#include <stdlib.h>
#include <stdio.h>
#include "robo_main.h"
#include "cap1188.h"
#include "pca9685.h"

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

// Variables
int fd_pwm = -1;
int fd_cap = -1;
int pulse;
int cap_node;
int num_of_turns = 1;
uint8_t touched_caps;

// MAIN
int main() {

	/////// Set Up ////////
  setup();

	// Main Loop
	while (1) {
		delay(500);
    touched_caps = touched(fd_cap);
		if (touched_caps) {
      printf("Touch detected on - %d\n", touched_caps);
      cap_node = 0;
			for (cap_node; cap_node < 8; cap_node++) {
				if (touched_caps & (1 << cap_node)) {
          num_of_turns++;
          if (num_of_turns % 2 == 0) {
					  printf("%d touched, ", cap_node);
					  react(cap_node);
            printf("React Completed\n");
          }
				}
				printf("\n");
			}
		}
	}

}


///////// FUNCTIONS ///////////

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
