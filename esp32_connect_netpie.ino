#include <WiFi.h>
#include <PubSubClient.h>

// Wi-Fi credentials
const char* WIFI_SSID = "YOUR_WIFI_SSID";
const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";

// NETPIE / MQTT credentials
const char* MQTT_BROKER = "broker.netpie.io";
const uint16_t MQTT_PORT = 1883;
const char* MQTT_CLIENT_ID = "YOUR_CLIENT_ID";
const char* MQTT_USERNAME = "YOUR_TOKEN";
const char* MQTT_PASSWORD = "YOUR_SECRET";
const char* MQTT_SUB_TOPIC = "@msg/demo";

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

void onMessage(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("]: ");

  for (unsigned int i = 0; i < length; ++i) {
    Serial.print(static_cast<char>(payload[i]));
  }

  Serial.println();
}

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
  mqttClient.setCallback(onMessage);

  while (!mqttClient.connected()) {
    Serial.print("Connecting to MQTT...");

    if (mqttClient.connect(MQTT_CLIENT_ID, MQTT_USERNAME, MQTT_PASSWORD)) {
      Serial.println("connected");
      mqttClient.subscribe(MQTT_SUB_TOPIC);
      return;
    }

    Serial.print("failed, state=");
    Serial.print(mqttClient.state());
    Serial.println(" retry in 2 seconds");
    delay(2000);
  }
}

void setup() {
  Serial.begin(115200);

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
}
