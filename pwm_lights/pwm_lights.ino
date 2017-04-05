#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// default address 0x40 used
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

void setup() {
  
  Serial.begin(9600);
  Serial.println("Humanoid Robot Project!");

  pwm.begin();
  
  pwm.setPWMFreq(60);  // 60 Hz updates

  yield();
  
}

void loop() {

  delay(500);
  
  for (uint16_t pulselen = 0; pulselen < 1000; pulselen++) {
    pwm.setPWM(4, 0, pulselen);
  }

  delay(500);

  for (uint16_t pulselen = 1000; pulselen > 0; pulselen--) {
    pwm.setPWM(4, 0, pulselen);
  }
}
