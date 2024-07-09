#include <Arduino.h>
#include <Wire.h>
#include <Servo.h>

#define I2C_ADDR 4

#define NUM_SERVOS 6
#define L_J1 3  // Right header
#define L_J2 5  // Middle header
#define L_J3 6  // Left header
#define R_J1 11 // Right header
#define R_J2 10 // Middle header
#define R_J3 9  // Left header

Servo servos[NUM_SERVOS]; // L_J1, L_J2, L_J3, R_J1, R_J2, R_J3

int currentServoIndex = 0;

void setupServos()
{
  servos[0].attach(L_J1);
  servos[1].attach(L_J2);
  servos[2].attach(L_J3);
  servos[3].attach(R_J1);
  servos[4].attach(R_J2);
  servos[5].attach(R_J3);
}

void updateServoPositions(int howMany)
{

  while (0 < Wire.available())
  {
    int angle = Wire.read();
    servos[currentServoIndex].write(angle);
    currentServoIndex++;
    if (currentServoIndex >= NUM_SERVOS)
    {
      currentServoIndex = 0;
      break; // This should always be the last iteration of the loop but if data remains something went wrong.
    }
  }
}

void initI2C()
{
  Wire.begin(I2C_ADDR);
  Wire.onReceive(updateServoPositions);
}

void setup()
{
  setupServos();
  initI2C();
}

void loop()
{
  while (1)
  {
  }
}
