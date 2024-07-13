#include <Arduino.h>
#include "bluetooth_server.hpp"
#include "hexapod_leg.hpp"

#define SDA 21
#define SCL 22

hexapod::I2CLeg l1_leg;
hexapod::Leg l2_leg(hexapod::L2_J1_PIN, hexapod::L2_J2_PIN, hexapod::L2_J3_PIN);
hexapod::Leg l3_leg(hexapod::L3_J1_PIN, hexapod::L3_J2_PIN, hexapod::L3_J3_PIN);
hexapod::I2CLeg r1_leg;
hexapod::Leg r2_leg(hexapod::R2_J1_PIN, hexapod::R2_J2_PIN, hexapod::R2_J3_PIN);
hexapod::Leg r3_leg(hexapod::R3_J1_PIN, hexapod::R3_J2_PIN, hexapod::R3_J3_PIN);

void onBLEConnect()
{
  Serial.println("CONNECTED");
}
void onBLEDisconnect()
{
  Serial.println("DISCONNECT");
}

void initHexapod()
{
  Serial.println("Starting BLE work!");
  BLE::setupBLEServer("ESP32 Hexapod", &onBLEConnect, &onBLEDisconnect);
  Serial.println("Characteristic defined! Now you can read it in your phone!");

  hexapod::initI2C();
  hexapod::initEspPWM();
}

void setup()
{
  Serial.begin(115200);
  initHexapod();
}

// Print the joystick position from mobile app
// void loop()
// {
//   // put your main code here, to run repeatedly:
//   Serial.println(BLE::readCharacteristicValue(JOYSTICK_SERVICE_UUID, JOYSTICK_OFFSET_CHARACTERISTIC_UUID).c_str());
// }

void loop()
{

  l2_leg.setSameAngles(90);
  delay(1000);
  l3_leg.setSameAngles(90);
  r2_leg.setSameAngles(90);
  delay(1000);
  r3_leg.setSameAngles(90);

  delay(4000);
  l2_leg.setAngles(0, 20, 40);
  l3_leg.setAngles(60, 80, 100);
  r2_leg.setAngles(120, 140, 160);
  r3_leg.setAngles(0, 90, 180);
  delay(1000);
  l2_leg.setSameAngles(0);
  l3_leg.setSameAngles(0);
  r2_leg.setSameAngles(0);
  r3_leg.setSameAngles(0);
  delay(4000);
}