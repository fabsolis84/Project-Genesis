#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>

class UnitLinkPacket {
public:
    UnitLinkPacket();

    void setApiKey(const char* apiKey);
    void setDevice(const char* id,
                   const char* role,
                   const char* fw,
                   const char* hw = "dev");

    void setSignal(const char* name, int value);
    void setTemp(float supply, float ret);
    void setAnalog(float voltage, float pressure);
    void setAmps(float comp, float fan, float blower);
    void setSafety(int aux1, int aux2, int aux3);

    const char* toJson();

private:
    StaticJsonDocument<768> doc;
    char buffer[768];
};