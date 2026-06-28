#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>

class UnitLinkMQTT {
public:
  UnitLinkMQTT();

  bool begin(
    const char* wifiSsid,
    const char* wifiPassword,
    const char* mqttServer,
    uint16_t mqttPort,
    const char* mqttUsername,
    const char* mqttPassword
  );

  void loop();
  bool publish(const char* topic, const char* payload);

private:
  WiFiClientSecure secureClient;
  PubSubClient mqttClient;

  const char* _mqttServer;
  uint16_t _mqttPort;
  const char* _mqttUsername;
  const char* _mqttPassword;

  unsigned long _lastReconnectAttempt = 0;

  void connectWiFi(const char* ssid, const char* password);
  bool connectMQTT();
};