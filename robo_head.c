#include <stdint.h>
#include <wiringPiI2C.h>
#include <wiringSerial.h>
#include <wiringPi.h>
#include <stdlib.h>
#include <stdio.h>

// This is the main program for the robo_head project
// This will be running on a Raspberry pi

// CAP1188 I2c address
#define I2C_ADDRESS_1 0x28
#define I2C_ADDRESS_2 0x29

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

void cap1188info();
void setUpLEDTouch();
uint8_t touched();

int fileDescriptor1 = -1;
int fileDescriptor2 = -1;
volatile uint8_t touchedCaps;
uint8_t i;

int main() {
	
	// Setting up Wiring Pi, and I2c communication
	if (wiringPiSetup() == -1) {
		printf("Wiring pi set up failure\n");
		exit(1);
	}

	// Set up file descriptor 1
	fileDescriptor1 = wiringPiI2CSetup(I2C_ADDRESS_1);
	printf("fd - %d\n", fileDescriptor1);
	if (fileDescriptor1  == -1) {
		printf("Could not find cap1188 num 1\n");
		exit(1);
	} 

	// Set up file descriptor 2
	fileDescriptor2 = wiringPiI2CSetup(I2C_ADDRESS_2);
	printf("fd - %d\n", fileDescriptor2);
	if (fileDescriptor2  == -1) {
		printf("Could not find cap1188 num 2\n");
		exit(1);
	} 

	// Check I2c communication and set up LEDs on #1
	cap1188info(fileDescriptor1);
	setUpLEDTouch(fileDescriptor1);

	// Check I2c communication and set up LEDs on #2
	cap1188info(fileDescriptor2);
	setUpLEDTouch(fileDescriptor2);


	// Main loop
	while (1) {
		if (touched(fileDescriptor1)) {
			printf("Touch detected on #1 - %d\n", touchedCaps);
			i = 0;
			for (i; i < 8; i++) {
				if (touchedCaps & (1 << i)) {
					printf("%d touched, ", i);
					// Send i via serial library to arduino
				}
				printf("\n"); //flush write buffer so all touched buttons print to term
			}
		}
		else if (touched(fileDescriptor2)) {
			printf("Touch detected on #2 - %d\n", touchedCaps);
			i = 0;
			for (i; i < 8; i++) {
				if (touchedCaps & (1 << i)) {
					printf("%d touched, ", i);
					// Send i via serial library to arduino
				}
				printf("\n"); //flush write buffer so all touched buttons print to term
			}
		}
	}
}

void cap1188info(int fileDescriptor) {
	printf("Product Id Found - %d\n", wiringPiI2CReadReg8(fileDescriptor,CAP1188_PRODID));
	printf("Manufac Id Found - %d\n", wiringPiI2CReadReg8(fileDescriptor,CAP1188_MANUID));
	printf("Revision # Found - %d\n", wiringPiI2CReadReg8(fileDescriptor,CAP1188_REV));
}

void setUpLEDTouch(int fileDescriptor) {
	wiringPiI2CWriteReg8(fileDescriptor,CAP1188_MTBLK, 0); // Allow multiple touches
	wiringPiI2CWriteReg8(fileDescriptor,CAP1188_LEDLINK, 0xFF); // Have LEDs follow touch
	wiringPiI2CWriteReg8(fileDescriptor,CAP1188_STANDBYCFG, 0x30); // speed up a bit
}

uint8_t touched(int fileDescriptor) {
	touchedCaps = wiringPiI2CReadReg8(fileDescriptor,CAP1188_SENINPUTSTATUS);
	if (touched) {
		wiringPiI2CWriteReg8(fileDescriptor,CAP1188_MAIN, wiringPiI2CReadReg8(fileDescriptor,CAP1188_MAIN) & ~CAP1188_MAIN_INT);
	}
	return touchedCaps;
}









