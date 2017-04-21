#include "cap1188.h"

///////// CAP 1188 FUNCTIONS ///////////

void cap1188info(int fd_cap) {
	printf("Product Id - %d\n", wiringPiI2CReadReg8(fd_cap,CAP1188_PRODID));
	printf("Manufac Id - %d\n", wiringPiI2CReadReg8(fd_cap,CAP1188_MANUID));
	printf("Revision # - %d\n", wiringPiI2CReadReg8(fd_cap,CAP1188_REV));
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
