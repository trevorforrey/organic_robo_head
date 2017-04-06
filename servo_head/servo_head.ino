#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

typedef struct HeadPosition {
    int neckPosition;
    int headPosition;
}HeadPosition;

// Directions based on head's perspective
// Pulse length counts
#define HEADUP   225
#define HEADDOWN   400
#define HEADSTRAIGHT 340

#define NECKRIGHT  150
#define NECKLEFT  550 

// mapping degrees to PWM value
// start at right, go towards left
#define NECKZONE1 map(170, 0, 180, 150, 550)
#define NECKZONE2 map(160, 0, 180, 150, 550)
#define NECKZONE3 map(150, 0, 180, 150, 550)
#define NECKZONE4 map(140, 0, 180, 150, 550)
#define NECKZONE5 map(130, 0, 180, 150, 550)
#define NECKZONE6 map(120, 0, 180, 150, 550)
#define NECKZONE7 map(110, 0, 180, 150, 550)
#define NECKZONE8 map(100, 0, 180, 150, 550)
#define NECKZONE9 map(90, 0, 180, 150, 550)
#define NECKZONE10 map(80, 0, 180, 150, 550)
#define NECKZONE11 map(70, 0, 180, 150, 550)
#define NECKZONE12 map(60, 0, 180, 150, 550)
#define NECKZONE13 map(50, 0, 180, 150, 550)
#define NECKZONE14 map(40, 0, 180, 150, 550)
#define NECKZONE15 map(30, 0, 180, 150, 550)
#define NECKZONE16 map(20, 0, 180, 150, 550)

#define ZONEZERO 0
#define ZONEEIGHT 8

// PWM positions on PCA 9685
#define NECK 0
#define HEAD 1
#define LED1 2
#define LED2 3
#define LED3 4
#define LED4 5
#define LED5 6
#define LED6 7
#define LED7 8
#define LED8 9
#define LED9 10

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
HeadPosition ZONE7_DOWN = {NECKZONE7,HEADDOWN};
HeadPosition ZONE8_DOWN = {NECKZONE8,HEADDOWN};
HeadPosition ZONE9_DOWN = {NECKZONE9,HEADDOWN};
HeadPosition ZONE10_DOWN = {NECKZONE10,HEADDOWN};
HeadPosition ZONE11_DOWN = {NECKZONE11,HEADDOWN};
HeadPosition ZONE12_DOWN = {NECKZONE12,HEADDOWN};
HeadPosition ZONE13_DOWN = {NECKZONE13,HEADDOWN};
HeadPosition ZONE14_DOWN = {NECKZONE14,HEADDOWN};
HeadPosition ZONE15_DOWN = {NECKZONE15,HEADDOWN};
HeadPosition ZONE16_DOWN = {NECKZONE16,HEADDOWN};

// default address 0x40 used
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

void moveFace(HeadPosition newPosition) {
  pwm.setPWM(HEAD, 0, newPosition.headPosition);
  pwm.setPWM(NECK, 0, newPosition.neckPosition);  
}

void setup() {
  
  Serial.begin(9600);
  Serial.println("Humanoid Robot Project!");

  pwm.begin();
  
  pwm.setPWMFreq(60);  // 60 Hz updates

  yield();
  
}

void loop() {

  moveFace(RIGHT_DOWN);
  delay(800);
  moveFace(LEFT_STRAIGHT);
  delay(800);
  
  //for (uint16_t pulselen = SERVOMIN; pulselen < SERVOMAX; pulselen++) {
    //pwm.setPWM(servonum, 1, pulselen);
  //}

  // Straight head and neck
  //pwm.setPWM(head, 0, HEADSTRAIGHT);
  //pwm.setPWM(neck, 0, 350);

  //delay(500);

  // Down to the left
  //pwm.setPWM(head, 0, HEADDOWN);
  //pwm.setPWM(neck, 0, NECKLEFT);

  //delay(500);

  // Straight head and neck
  //pwm.setPWM(head, 0, HEADSTRAIGHT);
  //pwm.setPWM(neck, 0, 350);

  //delay(500);

  // Down to the right
  //pwm.setPWM(head, 0, HEADDOWN);
  //pwm.setPWM(neck, 0, NECKRIGHT);
  
  
  //delay(500);
}
