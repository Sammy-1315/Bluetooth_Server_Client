import asyncio
from bleak import BleakScanner, BleakClient

# enables reading periodic notifications from microcontroller

ESP32_BLE_NAME = "XIAO_ESP32C6"
SERVICE_UUID = "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
CHARACTERISTIC_UUID = "beb5483e-36e1-4688-b7f5-ea07361b26a8"

async def notification_handler(sender, data):
    print(f"Notification from ESP32: {data.decode('utf-8')}")

async def connect_esp():
    print("Scanning for ESP32...")
    esp32 = None

    devices = await BleakScanner.discover()
    for device in devices:
        print(device.name)
        if device.name == ESP32_BLE_NAME:
            esp32 = device
            break

    if not esp32:
        print("ESP32 not found. Exiting...")
        return

    print(f"ESP32 found")

    async with BleakClient(esp32.address) as client:
        print("Connected to ESP32!")
        # Attempt to read characteristic
        try:
            val = await client.read_gatt_char(CHARACTERISTIC_UUID)
            print(f"Initial read value: {val.decode('utf-8')}")
        except Exception as e:
            print(f"Failed to read characteristic: {e}")
            return

        # Enable notifications
        try:
            await client.start_notify(CHARACTERISTIC_UUID, notification_handler)
            print("Notifications started successfully!")
        except Exception as e:
            print(f"Failed to start notifications: {e}")

        # Keep listening for notifications
        try:
            while True:
                await asyncio.sleep(1)
        except KeyboardInterrupt:
            print("\nDisconnecting...")
            await client.stop_notify(CHARACTERISTIC_UUID)

asyncio.run(connect_esp())
