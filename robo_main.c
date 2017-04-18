#include <stdint.h>
#include <wiringPiI2C.h>
#include <wiringPi.h>
#include <stdlib.h>
#include <stdio.h>
//#include "cap1188.h"
//#include "pca9685.h"

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


////// PWM index positions ///////////
#define NECK 0
#define HEAD 1
#define LED1 2
#define LED2 3
#define LED3 4
#define LED4 5
#define LED5 6
#define LED6 7
#define LED_R 8
#define LED_G 9
#define LED_B 10

///////// PCA 9685 DEFINES ///////////

//PWM Driver Address
#define PWMI2CADDRESS 0x40

//#define SERVOMIN 150
//#define SERVOMAX 500

#define PCA9685_SUBADR1 0x2
#define PCA9685_SUBADR2 0x3
#define PCA9685_SUBADR3 0x4

#define PCA9685_MODE1 0x0
#define PCA9685_PRESCALE 0xFE

#define LED0_ON_L 0x6
#define LED0_ON_H 0x7
#define LED0_OFF_L 0x8
#define LED0_OFF_H 0x9

#define ALLLED_ON_L 0xFA
#define ALLLED_ON_H 0xFB
#define ALLLED_OFF_L 0xFC
#define ALLLED_OFF_H 0xFD


///////// CAP 1188 DEFINES ///////////

// CAP1188 I2c address
#define CAP_I2C_ADDRESS 0x28

// CAP 1188 Registers
#define CAP1188_SENINPUTSTATUS 0x3
#define CAP1188_MTBLK 0x2A
#define CAP1188_LEDLINK 0x72
#define CAP1188_PRODID 0xFD
#define CAP1188_MANUID 0xFE
#define CAP1188_STANDBYCFG 0x41
#define CAP1188_REV 0xFF
#define CAP1188_MAIN 0x00
#define CAP1188_MAIN_INT 0x01
#define CAP1188_LEDPOL 0x73


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

// PCA 9685
void reset();
void setPWM(uint8_t pwm_index, uint16_t on, uint16_t off);
void setPWMFreq(float freq);

// CAP 1188
void cap1188info(int fd_cap);
void setUpLEDTouch(int fd_cap);
uint8_t touched(int fd_cap);

// MAIN PROTOTYPES
void react(int zoneActivated);
void moveFace(HeadPosition newPosition);
void LEDReaction(int zone);
void setColor(int red, int green, int blue);
HeadPosition getZoneHeadPosition(int zoneActived);
void LEDDarken(int ledIndex);



// Variables
int fd_pwm = -1;
int fd_cap = -1;
int pulse;
int i;
volatile uint8_t touchedCaps;

// MAIN
int main() {

	/////// Set Up ////////

	// Setting up Wiring Pi
	if (wiringPiSetup() == -1) {
		printf("Wiring pi set up failure\n");
		exit(1);
	}

	// Set up pwm file descriptor (i2c communication)
	fd_pwm = wiringPiI2CSetup(PWMI2CADDRESS);
	printf("fd - %d\n", fd_pwm);
	if (fd_pwm  == -1) {
		printf("Could not find pwm driver\n");
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

	reset();

	//i2c test
	oldmode = wiringPiI2CReadReg8(fd_pwm, PCA9685_MODE1);
	printf("old mode - %d\n", oldmode);

	////////////////////////////////////

	setPWMFreq(60);

	cap1188info(fd_cap);
	setUpLEDTouch(fd_cap);

        printf("Set up complete\n");

	// Main Loop
	while (1) {
		
		delay(500);
		if (touched(fd_cap)) {
			printf("Touch detected on - %d\n", touchedCaps);
			i = 0;
			for (i; i < 8; i++) {
				if (touchedCaps & (1 << i)) {
					printf("%d touched, ", i);
					react(i);
				}
				printf("\n"); //flush write buffer so all touched buttons print to term
			}
		}
	}

}


///////// TOP LEVEL FUNCTIONS ///////////

void react(int zoneActivated) {

  zoneActivated++;

  // Get zone, calculate head position and led chain to activate
  HeadPosition newPosition = getZoneHeadPosition(zoneActivated);

  printf("Zone: %d\n", zoneActivated);
  printf("headPosition: %d\n", newPosition.headPosition);
  printf("neckPosition: %d\n", newPosition.neckPosition);

  //LEDReaction(zoneActivated);
  delay(500); // delay could make it seem like the face reacted to the light ??
  moveFace(newPosition);
  delay(1000);
  //delay(600);
  //LEDDarken(zoneActivated);
  HeadPosition middlePosition = {NECKMIDDLE, HEADSTRAIGHT};
  moveFace(middlePosition);
  
}


void moveFace(HeadPosition newPosition) {
  printf("move face called\n");
  setPWM(HEAD, 0, newPosition.headPosition);
  setPWM(NECK, 0, newPosition.neckPosition);  
}


void LEDReaction(int zone) {
  // Will do logic after more construction of project
  switch (zone) {
    case 1:
      graduallyLight(LED1);
      break;
    case 2:
      graduallyLight(LED2);
      break;
    case 3:
      graduallyLight(LED3);
      break;
    case 4:
      graduallyLight(LED4);
      break;
    case 5:
      graduallyLight(LED5);
      break;
    case 6:
      graduallyLight(LED6);
      break;
    default:
      break;
  }  
}


void graduallyLight(int ledIndex) {
	uint16_t pulselen = 0;
	for (pulselen; pulselen < 1000; pulselen++) {
        setPWM(ledIndex, 0, pulselen);
    }
}

void LEDDarken(int ledIndex) {
	uint16_t pulselen = 1000;
	for (pulselen; pulselen >= 0; pulselen--) {
        setPWM(ledIndex, 0, pulselen);
    }
}

HeadPosition getZoneHeadPosition(int zoneActived) {
	HeadPosition newPosition = {0,0};
	switch (zoneActived) {
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
	    default:
	      break;
	}
	return newPosition;
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


///////// PCA 9685 FUNCTIONS ///////////

void setPWMFreq(float freq) {

	freq *= 0.9;
	float prescaleval = 25000000;
 	prescaleval /= 4096;
 	prescaleval /= freq;
 	prescaleval -= 1;

 	uint8_t prescale = (prescaleval + 0.5);

 	uint8_t oldmode = wiringPiI2CReadReg8(fd_pwm, PCA9685_MODE1);
 	uint8_t newmode = (oldmode&0x7F) | 0x10; // sleep
 	wiringPiI2CWriteReg8(fd_pwm, PCA9685_MODE1, newmode); // go to sleep
 	wiringPiI2CWriteReg8(fd_pwm, PCA9685_PRESCALE, prescale); // set the prescaler
 	wiringPiI2CWriteReg8(fd_pwm, PCA9685_MODE1, oldmode);
 	delay(5);
 	wiringPiI2CWriteReg8(fd_pwm, PCA9685_MODE1, oldmode | 0xa1);  //  This sets the MODE1 register to turn on auto increment.
}


void setPWM(uint8_t index, uint16_t on, uint16_t off) {
	wiringPiI2CWriteReg8(fd_pwm, LED0_ON_L + 4 * index, on & 0xFF);
	wiringPiI2CWriteReg8(fd_pwm, LED0_ON_H + 4 * index, on >> 8);
	wiringPiI2CWriteReg8(fd_pwm, LED0_OFF_L + 4 * index, off & 0xFF);
	wiringPiI2CWriteReg8(fd_pwm, LED0_OFF_H + 4 * index, off >> 8);
        
}


void reset(void) {
	wiringPiI2CWriteReg8(fd_pwm, PCA9685_MODE1, 0x0);
}

///////// CAP 1188 FUNCTIONS ///////////

void cap1188info(int fd_cap) {
	printf("Product Id Found - %d\n", wiringPiI2CReadReg8(fd_cap,CAP1188_PRODID));
	printf("Manufac Id Found - %d\n", wiringPiI2CReadReg8(fd_cap,CAP1188_MANUID));
	printf("Revision # Found - %d\n", wiringPiI2CReadReg8(fd_cap,CAP1188_REV));
}


void setUpLEDTouch(int fd_cap) {
	wiringPiI2CWriteReg8(fd_cap,CAP1188_MTBLK, 0); // Allow multiple touches
	wiringPiI2CWriteReg8(fd_cap,CAP1188_LEDLINK, 0xFF); // Have LEDs follow touch
	wiringPiI2CWriteReg8(fd_cap,CAP1188_STANDBYCFG, 0x30); // speed up a bit
}


uint8_t touched(int fd_cap) {
	touchedCaps = wiringPiI2CReadReg8(fd_cap,CAP1188_SENINPUTSTATUS);
	if (touched) {
		wiringPiI2CWriteReg8(fd_cap,CAP1188_MAIN, wiringPiI2CReadReg8(fd_cap,CAP1188_MAIN) & ~CAP1188_MAIN_INT);
	}
	return touchedCaps;
}






