#include <stdint.h>
#include <wiringPiI2C.h>
#include <wiringPi.h>
#include <stdlib.h>
#include <stdio.h>
#include "pca9685.h"

///////// PCA 9685 FUNCTIONS ///////////

void setPWMFreq(float freq, int fd_pwm) {

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


void setPWM(uint8_t index, uint16_t on, uint16_t off, int fd_pwm) {
	wiringPiI2CWriteReg8(fd_pwm, LED0_ON_L + 4 * index, on & 0xFF);
	wiringPiI2CWriteReg8(fd_pwm, LED0_ON_H + 4 * index, on >> 8);
	wiringPiI2CWriteReg8(fd_pwm, LED0_OFF_L + 4 * index, off & 0xFF);
	wiringPiI2CWriteReg8(fd_pwm, LED0_OFF_H + 4 * index, off >> 8);
}


void resetPCA9685(int fd_pwm) {
	wiringPiI2CWriteReg8(fd_pwm, PCA9685_MODE1, 0x0);
}
