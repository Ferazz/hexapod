#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <Arduino.h>

#define BATTERY_SERVICE_UUID "180F"
#define BATTERY_LEVEL_CHARACTERISTIC_UUID "2A19"
#define BATTERY_LEVEL_DESCRIPTOR_UUID 0x2901

#define JOYSTICK_SERVICE_UUID "8079d8c1-6a38-4c1e-8f81-4a52ef0dd349"
#define JOYSTICK_OFFSET_CHARACTERISTIC_UUID "d027f14b-150f-4e30-a519-1cade81e0570"
#define JOYSTICK_OFFSET_DESCRIPTOR_UUID 0x2901
#define JOYSTICK_SPEED_CHARACTERISTIC_UUID "1027deaf-b113-48a9-9732-0f35cd3c2945"
#define JOYSTICK_SPEED_DESCRIPTOR_UUID 0x2901

#pragma once
namespace BLE
{
    /**
     * Setup the ESP32 to act as a BLE server
     * @param serverName Name of the server which will be seen on from other devices when trying to pair over bluetooth.
     */
    void setupBLEServer(std::string serverName, void (*onConnectFun)(), void(onDisconnectFun)());

    /**
     * Add a characteristic and descriptor to a service
     * @param pService pointer to a service object which the characteristic will be added to.
     * @param pCharacteristic pointer to a characteristic object which will be added to the service.
     * @param pDescriptor pointer to a descriptor object which will be added to the characteristic.
     * @param descriptorValue String of text which will be the "name" of the descriptor. Currently used to describe what the characteristic does.
     */
    void setupBLEService(BLEService *pService, BLECharacteristic *pCharacteristic, BLEDescriptor *pDescriptor, std::string descriptorValue);

    /**
     * Update the battery level value which can be read over bluetooth.'
     * @param level battery level to be written over bluetooth.
     */
    void updateBatteryLevel(float level);

    /**
     * Read the latest joystick offset value sent by the mobile app.
     * @return X, Y offset of the joystick
     */
    std::pair<float, float> readJoystickPosition();

    std::string readCharacteristicValue(std::string serviceUUID, std::string characteristicUUID);

    /**
     * Setup callbacks onConnect and onDisconnect
     */
    class MyServerCallbacks : public BLEServerCallbacks
    {
    private:
        void (*onConnectFun)();
        void (*onDisconnectFun)();

    public:
        MyServerCallbacks(void (*onConnectFuntion)(), void (*onDisconnectFuntion)());
        void onConnect(BLEServer *pServer);
        void onDisconnect(BLEServer *pServer);
    };
}
