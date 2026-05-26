#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

// Wi-Fi credentials
const char* WIFI_SSID = "YOUR_WIFI_SSID";
const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";

// NETPIE / MQTT credentials
const char* MQTT_BROKER = "broker.netpie.io";
const uint16_t MQTT_PORT = 1883;
const char* MQTT_CLIENT_ID = "YOUR_CLIENT_ID";
const char* MQTT_USERNAME = "YOUR_TOKEN";
const char* MQTT_PASSWORD = "YOUR_SECRET";
const char* MQTT_TOPIC = "@msg/temperature";

// DHT sensor configuration
constexpr uint8_t DHT_PIN = 4;
constexpr uint8_t DHT_TYPE = DHT22;
constexpr unsigned long PUBLISH_INTERVAL_MS = 2000;

DHT dht(DHT_PIN, DHT_TYPE);
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

unsigned long lastPublishTime = 0;

void connectWiFi() {
  if (WiFi.status() == WL_CONNECTED) {
    return;
  }

  Serial.print("Connecting to Wi-Fi");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("Wi-Fi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void connectMQTT() {
  mqttClient.setServer(MQTT_BROKER, MQTT_PORT);

  while (!mqttClient.connected()) {
    Serial.print("Connecting to MQTT...");

    if (mqttClient.connect(MQTT_CLIENT_ID, MQTT_USERNAME, MQTT_PASSWORD)) {
      Serial.println("connected");
      return;
    }

    Serial.print("failed, state=");
    Serial.print(mqttClient.state());
    Serial.println(" retry in 2 seconds");
    delay(2000);
  }
}

void publishSensorData() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor");
    return;
  }

  String payload = "{";
  payload += "\"temperature\":";
  payload += String(temperature, 2);
  payload += ",\"humidity\":";
  payload += String(humidity, 2);
  payload += "}";

  Serial.print("Publishing payload: ");
  Serial.println(payload);

  if (!mqttClient.publish(MQTT_TOPIC, payload.c_str())) {
    Serial.println("Publish failed");
  }
}

void setup() {
  Serial.begin(115200);
  dht.begin();

  connectWiFi();
  connectMQTT();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    connectWiFi();
  }

  if (!mqttClient.connected()) {
    connectMQTT();
  }

  mqttClient.loop();

  const unsigned long now = millis();
  if (now - lastPublishTime >= PUBLISH_INTERVAL_MS) {
    lastPublishTime = now;
    publishSensorData();
  }
}
