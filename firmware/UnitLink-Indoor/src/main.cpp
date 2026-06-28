#include <Arduino.h>
#include <UnitLinkMQTT.h>
#include <UnitLinkPacket.h>
#include <UnitLinkWireless.h>

UnitLinkMQTT mqtt;
UnitLinkWireless wireless;

const char* WIFI_SSID = "SpectrumSetup-E5";
const char* WIFI_PASSWORD = "stocknumber528";

const char* MQTT_SERVER = "mqtt.myunitlink.com";
const uint16_t MQTT_PORT = 8883;
const char* MQTT_USERNAME = "7_1";
const char* MQTT_PASSWORD = "5e4d9c3a6b55604321d809679f025905d5ce83d7dd5a0dac64b37da783227e87";

const char* API_KEY = "5e4d9c3a6b55604321d809679f025905d5ce83d7dd5a0dac64b37da783227e87";
const char* MQTT_TOPIC = "hvac/7/1/telemetry";

// Atlas MAC Address
uint8_t atlasMac[] = {
  0x8C, 0x94, 0xDF, 0x92, 0xB7, 0x30
};

unsigned long lastPublish = 0;

bool atlasMessagePending = false;
char atlasMessage[32] = "";

void publishTelemetry(const char* sourceMessage) {

  UnitLinkPacket packet;

  packet.setApiKey(API_KEY);
  packet.setDevice("7_1", "apollo", "0.1.0", "dev");

  packet.setSignal("r", 1);
  packet.setSignal("y", 1);
  packet.setSignal("y2", 0);
  packet.setSignal("g", 1);
  packet.setSignal("w1", 0);
  packet.setSignal("w2", 0);
  packet.setSignal("o", 0);

  packet.setTemp(54.2, 72.1);
  packet.setAnalog(24.3, 0.45);
  packet.setAmps(12.4, 2.1, 3.8);
  packet.setSafety(0, 0, 0);

  const char* payload = packet.toJson();

  bool ok = mqtt.publish(MQTT_TOPIC, payload);

  Serial.print("MQTT publish: ");
  Serial.println(ok ? "success" : "failed");

  Serial.print("Source message: ");
  Serial.println(sourceMessage);

  Serial.println(payload);
}

void setup() {

  Serial.begin(115200);
  delay(1000);

  Serial.println("Apollo MQTT + ESP-NOW bridge booting...");

  mqtt.begin(
    WIFI_SSID,
    WIFI_PASSWORD,
    MQTT_SERVER,
    MQTT_PORT,
    MQTT_USERNAME,
    MQTT_PASSWORD
  );

Serial.print("Apollo Wi-Fi channel: ");
Serial.println(WiFi.channel());

  wireless.begin(ROLE_APOLLO, atlasMac);

  wireless.onMessage([](const UnitLinkWirelessPacket& packet) {

    Serial.println("=================================");
    Serial.println("Atlas packet received!");
    Serial.print("Message: ");
    Serial.println(packet.message);
    Serial.println("=================================");

    strncpy(atlasMessage, packet.message, sizeof(atlasMessage) - 1);
    atlasMessage[sizeof(atlasMessage) - 1] = '\0';

    atlasMessagePending = true;
  });

  Serial.println("Apollo bridge ready.");
}

void loop() {

  mqtt.loop();
  wireless.loop();

  if (atlasMessagePending) {

    atlasMessagePending = false;

    publishTelemetry(atlasMessage);
  }

  if (millis() - lastPublish >= 30000) {

    lastPublish = millis();

    publishTelemetry("APOLLO_HEARTBEAT");
  }
}