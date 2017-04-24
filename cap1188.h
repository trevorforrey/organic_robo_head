#ifndef _CAP1188_
#define _CAP1188_

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

///////// CAP 1188 PROTOTYPES ///////////
void cap1188info(int fd_cap);
void setUpLEDTouch(int fd_cap);
uint8_t touched(int fd_cap);


#endif
