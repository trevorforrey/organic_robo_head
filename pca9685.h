#ifndef _PCA9685_
#define _PCA9685_

///////// PCA 9685 DEFINES ///////////

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

///////// PCA 9685 PROTOTYPES ///////////
void resetPCA9685(int fd_pwm);
void setPWM(uint8_t pwm_index, uint16_t on, uint16_t off, int fd_pwm);
void setPWMFreq(float freq, int fd_pwm);

#endif
