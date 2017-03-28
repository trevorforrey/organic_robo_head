#include <stdint.h>
#include <wiringPiI2C.h>
#include <wiringPi.h>
#include <stdlib.h>
#include <stdio.h>

//PWM Driver Address
#define PWMI2CADDRESS 0x40

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

// Prototypes
void reset();
void setPWM(uint8_t num, uint16_t on, uint16_t off);
void setPWMFreq(float freq);

// Variables
int fileDescriptor = -1;

// MAIN
int main() {

	// Setting up Wiring Pi
	if (wiringPiSetup() == -1) {
		printf("Wiring pi set up failure\n");
		exit(1);
	}

	// Set up file descriptor (i2c communication)
	fileDescriptor = wiringPiI2CSetup(PWMI2CADDRESS);
	printf("fd - %d\n", fileDescriptor);
	if (fileDescriptor  == -1) {
		printf("Could not find pwm driver\n");
		exit(1);
	} 
}


void setPWMFreq(float freq) {

	freq *= 0.9;
	float prescaleval = 25000000;
 	prescaleval /= 4096;
 	prescaleval /= freq;
 	prescaleval -= 1;

 	uint8_t prescale = (prescaleval + 0.5);

 	uint8_t oldmode = wiringPiI2CReadReg8(fileDescriptor, PCA9685_MODE1);
 	uint8_t newmode = (oldmode&0x7F) | 0x10; // sleep
 	wiringPiI2CWriteReg8(fileDescriptor, PCA9685_MODE1, newmode); // go to sleep
 	wiringPiI2CWriteReg8(fileDescriptor, PCA9685_PRESCALE, prescale); // set the prescaler
 	wiringPiI2CWriteReg8(fileDescriptor, PCA9685_MODE1, oldmode);
 	delay(5);
 	wiringPiI2CWriteReg8(fileDescriptor, PCA9685_MODE1, oldmode | 0xa1);  //  This sets the MODE1 register to turn on auto increment.
}


void setPWM(uint8_t num, uint16_t on, uint16_t off) {
  // WIRE.write(LED0_ON_L+4*num);
  // WIRE.write(on);
  // WIRE.write(on>>8);
  // WIRE.write(off);
  // WIRE.write(off>>8);
}


void reset(void) {
	wiringPiI2CWriteReg8(fileDescriptor, PCA9685_MODE1, 0x0);
}






