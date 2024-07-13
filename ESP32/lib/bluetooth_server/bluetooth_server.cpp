/*
  Complete Getting Started Guide: https://RandomNerdTutorials.com/esp32-bluetooth-low-energy-ble-arduino-ide/
  Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleServer.cpp
  Ported to Arduino ESP32 by Evandro Copercini
*/
#include <bluetooth_server.hpp>

BLECharacteristic batteryCharacteristic(BATTERY_LEVEL_CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_READ);
BLEDescriptor batteryDescriptor(BLEUUID(uint16_t(BATTERY_LEVEL_DESCRIPTOR_UUID)));

BLECharacteristic joystickOffsetCharacteristic(JOYSTICK_OFFSET_CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_WRITE);
BLEDescriptor joystickOffsetDescriptor(BLEUUID(uint16_t(JOYSTICK_OFFSET_DESCRIPTOR_UUID)));

BLECharacteristic joystickSpeedCharacteristic(JOYSTICK_SPEED_CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_WRITE);
BLEDescriptor joystickSpeedDescriptor(BLEUUID(uint16_t(JOYSTICK_SPEED_DESCRIPTOR_UUID)));

BLEServer *pServer;
BLEService *pJoystickService;

bool deviceConnected = false;

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

using namespace std;

void BLE::setupBLEServer(string serverName, void (*onConnectFun)(), void(onDisconnectFun)())
{
    BLEDevice::init(serverName);
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new BLE::MyServerCallbacks(onConnectFun, onDisconnectFun));

    BLEService *pBatteryService = pServer->createService(BATTERY_SERVICE_UUID);
    BLEService *pJoystickService = pServer->createService(JOYSTICK_SERVICE_UUID);

    // Configure all services
    setupBLEService(pBatteryService, &batteryCharacteristic, &batteryDescriptor, "Battery Level");
    setupBLEService(pJoystickService, &joystickOffsetCharacteristic, &joystickOffsetDescriptor, "Joystick Offset");
    setupBLEService(pJoystickService, &joystickSpeedCharacteristic, &joystickSpeedDescriptor, "Robot Speed, between 0 and 1");

    // Start all services
    pBatteryService->start();
    pJoystickService->start();

    // BLEAdvertising *pAdvertising = pServer->getAdvertising();  // this still is working for backward compatibility
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(BATTERY_SERVICE_UUID);
    pAdvertising->setScanResponse(true);
    pAdvertising->setMinPreferred(0x06); // functions that help with iPhone connections issue
    pAdvertising->setMinPreferred(0x12);
    BLEDevice::startAdvertising();
}

BLE::MyServerCallbacks::MyServerCallbacks(void (*onConnectFuntion)(), void (*onDisconnectFuntion)())
{
    onConnectFun = onConnectFuntion;
    onDisconnectFun = onDisconnectFuntion;
}
void BLE::MyServerCallbacks::onConnect(BLEServer *pServer)
{
    deviceConnected = true;

    onConnectFun();
};
void BLE::MyServerCallbacks::onDisconnect(BLEServer *pServer)
{
    deviceConnected = false;

    onDisconnectFun();
}

void BLE::setupBLEService(BLEService *pService, BLECharacteristic *pCharacteristic, BLEDescriptor *pDescriptor, string descriptorValue)
{
    pService->addCharacteristic(pCharacteristic);
    pDescriptor->setValue(descriptorValue);
    pCharacteristic->addDescriptor(pDescriptor);
}

void BLE::updateBatteryLevel(float level)
{
    batteryCharacteristic.setValue(level);
}

// TODO: Datatype for this?
pair<float, float> BLE::readJoystickPosition()
{
    string offset = joystickOffsetCharacteristic.getValue();
}

string BLE::readCharacteristicValue(string serviceUUID, string characteristicUUID)
{
    return pServer->getServiceByUUID(serviceUUID)->getCharacteristic(characteristicUUID)->getValue();
}