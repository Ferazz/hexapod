// class BluetoothUUID {

// }
Map<String, String> serviceUUIDs = {
  "BATTERY_SERVICE_UUID": "180F",
  "JOYSTICK_SERVICE_UUID": "8079d8c1-6a38-4c1e-8f81-4a52ef0dd349",
};

Map<String, String> characteristicUUIDs = {
  "BATTERY_LEVEL_CHARACTERISTIC_UUID": "2A19",
  "JOYSTICK_OFFSET_CHARACTERISTIC_UUID":
      "d027f14b-150f-4e30-a519-1cade81e0570", // Data as Move_x Move_y Spin_x Spin_y
  "JOYSTICK_SPEED_CHARACTERISTIC_UUID": "1027deaf-b113-48a9-9732-0f35cd3c2945",
};

Map<String, int> descriptorUUIDs = {
  "JOYSTICK_OFFSET_DESCRIPTOR_UUID": 2901,
  "JOYSTICK_SPEED_DESCRIPTOR_UUID": 2901,
};
