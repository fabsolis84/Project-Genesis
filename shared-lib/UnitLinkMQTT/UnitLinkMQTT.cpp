#include "UnitLinkMQTT.h"

UnitLinkMQTT::UnitLinkMQTT()
: mqttClient(secureClient)
{
}

bool UnitLinkMQTT::begin(
    const char* wifiSsid,
    const char* wifiPassword,
    const char* mqttServer,
    uint16_t mqttPort,
    const char* mqttUsername,
    const char* mqttPassword)
{
    _mqttServer = mqttServer;
    _mqttPort = mqttPort;
    _mqttUsername = mqttUsername;
    _mqttPassword = mqttPassword;

    connectWiFi(wifiSsid, wifiPassword);

    secureClient.setInsecure();

    mqttClient.setServer(_mqttServer, _mqttPort);
    mqttClient.setBufferSize(1024);

    return connectMQTT();
}

void UnitLinkMQTT::connectWiFi(const char* ssid, const char* password)
{
    if (WiFi.status() == WL_CONNECTED)
        return;

    Serial.print("Connecting to WiFi");

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println();
    Serial.println("WiFi connected");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
}

bool UnitLinkMQTT::connectMQTT()
{
    if (mqttClient.connected())
        return true;

    Serial.printf("Connecting to MQTT %s:%d ... ",
                  _mqttServer,
                  _mqttPort);

    String clientId = "Apollo-";
    clientId += String((uint32_t)ESP.getEfuseMac(), HEX);

    bool ok = mqttClient.connect(
        clientId.c_str(),
        _mqttUsername,
        _mqttPassword);

    if (ok)
    {
        Serial.println("connected");
    }
    else
    {
        Serial.printf("failed, state=%d\n", mqttClient.state());
    }

    return ok;
}

void UnitLinkMQTT::loop()
{
    if (WiFi.status() != WL_CONNECTED)
        return;

    if (!mqttClient.connected())
    {
        if (millis() - _lastReconnectAttempt > 5000)
        {
            _lastReconnectAttempt = millis();
            connectMQTT();
        }
    }

    mqttClient.loop();
}

bool UnitLinkMQTT::publish(const char* topic, const char* payload)
{
    if (!mqttClient.connected())
    {
        Serial.print("MQTT not connected. State: ");
        Serial.println(mqttClient.state());
        return false;
    }

    Serial.print("Publishing to topic: ");
    Serial.println(topic);

    return mqttClient.publish(topic, payload);
}