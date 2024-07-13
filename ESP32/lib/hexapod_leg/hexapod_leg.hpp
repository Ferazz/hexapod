#include <ESP32Servo.h>
#include <Wire.h>

#define ATMEGA_I2C_ADDR 4

/*
    Forward
     ____
L1 -/    \- R1
L2 -|    |- R2
L3 -\____/- R3

*/

#pragma once

namespace hexapod
{

    // J1 = Coxa
    // J2 = Femur
    // J3 = Tibia
    // ATMEGA ???
    const int L1_J1_PIN = 0;
    const int L1_J2_PIN = 0;
    const int L1_J3_PIN = 0;

    const int L2_J1_PIN = 33;
    const int L2_J2_PIN = 25;
    const int L2_J3_PIN = 26;

    const int L3_J1_PIN = 27;
    const int L3_J2_PIN = 14;
    const int L3_J3_PIN = 13;

    // ATMEGA ???
    const int R1_J1_PIN = 0;
    const int R1_J2_PIN = 0;
    const int R1_J3_PIN = 0;

    const int R2_J1_PIN = 19;
    const int R2_J2_PIN = 18;
    const int R2_J3_PIN = 5;

    const int R3_J1_PIN = 17;
    const int R3_J2_PIN = 16;
    const int R3_J3_PIN = 4;

    /**
     * Inititalize PWM timers.
     */
    void initEspPWM();

    /**
     * Initialize I2C communication.
     */
    void initI2C();

    /**
     * Initialize all the legs of the hexapod.
     */
    // void initLegs();

    /**
     * The coxa is the servo closest to the body (hip joint).
     * The femur is the second servo (knee joint).
     * The tibia is the servo for the foot (ankle joint).
     */
    class Leg
    {
    protected:
        int coxa_calibration_offset = 0;
        int femur_calibration_offset = 0;
        int tibia_calibration_offset = 0;

    private:
        Servo coxa_servo;
        Servo femur_servo;
        Servo tibia_servo;

    public:
        Leg(int coxa_pin, int femur_pin, int tibia_pin);

        void setAngles(int coxa_angle, int femur_angle, int tibia_angle);

        void setSameAngles(int angle);

        void setCalibrationOffsets(int coxa_offset, int femur_offset, int tibia_offset);

    private:
    };

    class I2CLeg
    {
    protected:
        int coxa_calibration_offset = 0;
        int femur_calibration_offset = 0;
        int tibia_calibration_offset = 0;

    public:
        void initServos(int coxa_index, int femur_index, int tibia_index); // TODO: Change to constructor.
        uint8_t setAngles(int coxa_angle, int femur_angle, int tibia_angle);
        void setCalibrationOffsets(int coxa_offset, int femur_offset, int tibia_offset);

    private:
    };
}
