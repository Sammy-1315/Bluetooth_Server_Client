#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

// Enables read/write/notifications within a define BLE characteristic

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

BLECharacteristic *pCharacteristic;
bool deviceConnected = false;
// Create a class to handle characteristic callbacks
class MyCharacteristicCallbacks: public BLECharacteristicCallbacks {
  String value = "default value";
    void onRead(BLECharacteristic *pCharacteristic) {
      Serial.println("Characteristic read!");
      
    }
    
    void onWrite(BLECharacteristic *pCharacteristic) {
      value = "";

      uint8_t* dataPtr = pCharacteristic->getData();
      if (dataPtr != nullptr) {
        // Convert to Arduino String
        for (int i = 0; i < pCharacteristic->getLength(); i++) {
          value += (char)dataPtr[i]; 
        }
        Serial.print("New value: ");
        Serial.println(value);
        pCharacteristic->setValue(value);
        if (deviceConnected) {
          Serial.println("Sending notification...");
          pCharacteristic->notify();
        }
      }
    }
};

// Create a class for server callbacks
class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      Serial.println("Client connected!");
      // Refresh the value when a client connects
      deviceConnected = true;
    }

    void onDisconnect(BLEServer* pServer) {
      Serial.println("Client disconnected!");
      // Restart advertising to allow new connections
      deviceConnected = false;
      BLEDevice::startAdvertising();
    }
};

void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE work!");

  BLEDevice::init("XIAO_ESP32C6");
  BLEDevice::setPower(ESP_PWR_LVL_P9);
  // Create the server
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());
  
  // Create the service
  BLEService *pService = pServer->createService(SERVICE_UUID);
  
  // Create the characteristic
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_READ |
                      BLECharacteristic::PROPERTY_WRITE |
                      BLECharacteristic::PROPERTY_NOTIFY
                    );

  pCharacteristic->setValue("Hello World");
  pCharacteristic->setCallbacks(new MyCharacteristicCallbacks());
  
  // Start the service
  pService->start();
  
  // Start advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  
  Serial.println("Characteristic defined! Now you can read it in your phone!");
}

void loop() {
  if (deviceConnected) {
    // arbitrary sensor data - can be replaced with something more significant
    String newData = "Sensor Data: " + String(random(0, 100)); 
    pCharacteristic->setValue(newData.c_str());
    pCharacteristic->notify();
    Serial.println("Sent notification: " + newData);
  }
  delay(2000);
}