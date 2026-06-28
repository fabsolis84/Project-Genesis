#include "UnitLinkPacket.h"

UnitLinkPacket::UnitLinkPacket() {
  doc.clear();
  doc["packetVersion"] = 1;
  doc["uptime"] = 0;
}

void UnitLinkPacket::setApiKey(const char* apiKey) {
  doc["api_key"] = apiKey;
}

void UnitLinkPacket::setDevice(const char* id, const char* role, const char* fw, const char* hw) {
  JsonObject device = doc["device"].is<JsonObject>()
    ? doc["device"].as<JsonObject>()
    : doc["device"].to<JsonObject>();

  device["id"] = id;
  device["role"] = role;
  device["fw"] = fw;
  device["hw"] = hw;
}

void UnitLinkPacket::setSignal(const char* name, int value) {
  JsonObject sig = doc["sig"].is<JsonObject>()
    ? doc["sig"].as<JsonObject>()
    : doc["sig"].to<JsonObject>();

  sig[name] = value;
}

void UnitLinkPacket::setTemp(float supply, float ret) {
  JsonObject temp = doc["temp"].is<JsonObject>()
    ? doc["temp"].as<JsonObject>()
    : doc["temp"].to<JsonObject>();

  temp["supply"] = supply;
  temp["return"] = ret;
}

void UnitLinkPacket::setAnalog(float voltage, float pressure) {
  JsonObject analog = doc["analog"].is<JsonObject>()
    ? doc["analog"].as<JsonObject>()
    : doc["analog"].to<JsonObject>();

  analog["voltage"] = voltage;
  analog["pressure"] = pressure;
}

void UnitLinkPacket::setAmps(float comp, float fan, float blower) {
  JsonObject amps = doc["amps"].is<JsonObject>()
    ? doc["amps"].as<JsonObject>()
    : doc["amps"].to<JsonObject>();

  amps["comp"] = comp;
  amps["fan"] = fan;
  amps["blower"] = blower;
}

void UnitLinkPacket::setSafety(int aux1, int aux2, int aux3) {
  JsonObject safety = doc["safety"].is<JsonObject>()
    ? doc["safety"].as<JsonObject>()
    : doc["safety"].to<JsonObject>();

  safety["aux1"] = aux1;
  safety["aux2"] = aux2;
  safety["aux3"] = aux3;
}

const char* UnitLinkPacket::toJson() {
  doc["uptime"] = millis();
  serializeJson(doc, buffer);
  return buffer;
}