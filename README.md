# ESP32 NETPIE Examples

This project contains ESP32 examples for connecting to `Wi-Fi` and `NETPIE` over `MQTT`.

The repository was reorganized from older `.txt` examples into cleaner source files that are easier to read, modify, and upload.

## Files

- `esp32_connect_netpie.ino` Basic ESP32 example for connecting to Wi-Fi and NETPIE and subscribing to an MQTT topic
- `send_dht_to_netpie.ino` ESP32 example that reads `DHT22` data and publishes temperature and humidity to NETPIE as JSON
- `esp32_connect_netpie.txt` Archived plain-text version of the basic connection example
- `send_dht_to_netpie.txt` Archived plain-text version of the DHT publishing example

## Requirements

- `ESP32` board
- Arduino IDE or PlatformIO
- Libraries:
  - `WiFi` (included with the ESP32 core)
  - `PubSubClient`
  - `DHT sensor library`

## NETPIE Settings

Update these values in the `.ino` files before uploading:

```cpp
const char* WIFI_SSID = "YOUR_WIFI_SSID";
const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";

const char* MQTT_CLIENT_ID = "YOUR_CLIENT_ID";
const char* MQTT_USERNAME = "YOUR_TOKEN";
const char* MQTT_PASSWORD = "YOUR_SECRET";
```

Also verify these settings:

- `MQTT_BROKER` is usually `broker.netpie.io`
- `MQTT_PORT` is usually `1883`
- `MQTT_TOPIC` or `MQTT_SUB_TOPIC` should match your use case

## Example Payload

`send_dht_to_netpie.ino` publishes data in this format:

```json
{
  "temperature": 28.50,
  "humidity": 64.20
}
```

## How To Use

1. Install the `ESP32` board package in Arduino IDE.
2. Install the `PubSubClient` library.
3. Install the `DHT sensor library`.
4. Open the example file you want to use.
5. Replace the Wi-Fi, Client ID, Token, Secret, and topic values.
6. Select the correct board and serial port.
7. Upload the sketch to your ESP32.
8. Open Serial Monitor at `115200 baud`.

## Improvements From The Original

- Converted plain text examples into ready-to-use source files
- Used clearer constant names and function structure
- Separated Wi-Fi and MQTT connection logic
- Added reconnect handling for lost Wi-Fi or MQTT connections
- Added `mqttClient.loop()` to keep MQTT communication alive
- Replaced simple `delay()`-based publishing with a `millis()` timer

## Note About File Extension

These examples use Arduino and ESP32 libraries and classes, which are C++-based rather than plain C.

Because of that, `.ino` or `.cpp` is a better fit than `.c`.  
