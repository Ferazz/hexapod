/*
  Complete Getting Started Guide: https://RandomNerdTutorials.com/esp32-bluetooth-low-energy-ble-arduino-ide/
  Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleServer.cpp
  Ported to Arduino ESP32 by Evandro Copercini
*/
#include <Arduino.h>
#include <bluetooth_server.h>

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

void onBLEConnect()
{
  Serial.println("CONNECTED");
}
void onBLEDisconnect()
{
  Serial.println("DISCONNECT");
}

void setup()
{
  Serial.begin(115200);
  Serial.println("Starting BLE work!");
  BLE::setupBLEServer("ESP32 Hexapod", &onBLEConnect, &onBLEDisconnect);

  Serial.println("Characteristic defined! Now you can read it in your phone!");
}

void loop()
{
  // put your main code here, to run repeatedly:
  delay(2000);
  Serial.println(BLE::readCharacteristicValue(JOYSTICK_SERVICE_UUID, JOYSTICK_OFFSET_CHARACTERISTIC_UUID).c_str());
}