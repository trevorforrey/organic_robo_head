#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// default address 0x40 used
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// Directions based on head's perspective
// Pulse length counts
#define NECKRIGHT  150
#define NECKLEFT  550 
#define HEADUP   225
#define HEADDOWN   400
#define HEADSTRAIGHT 340

#define ZONEZERO 0
#define ZONEEIGHT 8

// Servo positions on PCA 9685
uint8_t neck = 0;
uint8_t head = 1;


void setup() {
  
  Serial.begin(9600);
  Serial.println("Humanoid Robot Project!");

  pwm.begin();
  
  pwm.setPWMFreq(60);  // 60 Hz updates

  yield();
  
}

void loop() {
  
  //for (uint16_t pulselen = SERVOMIN; pulselen < SERVOMAX; pulselen++) {
    //pwm.setPWM(servonum, 1, pulselen);
  //}

  // Straight head and neck
  pwm.setPWM(head, 0, HEADSTRAIGHT);
  pwm.setPWM(neck, 0, 350);

  delay(500);

  // Down to the left
  pwm.setPWM(head, 0, HEADDOWN);
  pwm.setPWM(neck, 0, NECKLEFT);

  delay(500);

  // Straight head and neck
  pwm.setPWM(head, 0, HEADSTRAIGHT);
  pwm.setPWM(neck, 0, 350);

  delay(500);

  // Down to the right
  pwm.setPWM(head, 0, HEADDOWN);
  pwm.setPWM(neck, 0, NECKRIGHT);
  
  
  delay(500);
}
