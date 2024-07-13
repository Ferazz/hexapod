#include "hexapod_leg.hpp"

void hexapod::initEspPWM()
{
    // Allow allocation of all timers
    ESP32PWM::allocateTimer(0);
    ESP32PWM::allocateTimer(1);
    ESP32PWM::allocateTimer(2);
    ESP32PWM::allocateTimer(3);
}
void hexapod::initI2C()
{
    // For I2C
    Wire.begin();
}

hexapod::Leg::Leg(int coxa_pin, int femur_pin, int tibia_pin)
{
    coxa_servo.setPeriodHertz(50);
    femur_servo.setPeriodHertz(50);
    tibia_servo.setPeriodHertz(50);

    coxa_servo.attach(coxa_pin);
    femur_servo.attach(femur_pin);
    tibia_servo.attach(tibia_pin);
}

void hexapod::Leg::setAngles(int coxa_angle, int femur_angle, int tibia_angle)
{
    coxa_servo.write(coxa_angle + coxa_calibration_offset);
    femur_servo.write(femur_angle + femur_calibration_offset);
    tibia_servo.write(tibia_angle + tibia_calibration_offset);
}

void hexapod::Leg::setSameAngles(int angle)
{
    setAngles(angle, angle, angle);
}

void hexapod::Leg::setCalibrationOffsets(int coxa_offset, int femur_offset, int tibia_offset)
{
    coxa_calibration_offset = coxa_offset;
    femur_calibration_offset = femur_offset;
    tibia_calibration_offset = tibia_offset;
}

void hexapod::I2CLeg::initServos(int coxa_index, int femur_index, int tibia_index)
{
    // TODO
}

// UNTESTED
uint8_t hexapod::I2CLeg::setAngles(int coxa_angle, int femur_angle, int tibia_angle)
{
    Wire.beginTransmission(ATMEGA_I2C_ADDR);
    Wire.write(coxa_angle + coxa_calibration_offset);
    Wire.write(femur_angle + femur_calibration_offset);
    Wire.write(tibia_angle + tibia_calibration_offset);
    uint8_t error = Wire.endTransmission();
    return error;
}

void hexapod::I2CLeg::setCalibrationOffsets(int coxa_offset, int femur_offset, int tibia_offset)
{
    coxa_calibration_offset = coxa_offset;
    femur_calibration_offset = femur_offset;
    tibia_calibration_offset = tibia_offset;
}