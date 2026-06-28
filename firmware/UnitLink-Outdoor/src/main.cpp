#include <Arduino.h>
#include <UnitLinkWireless.h>

UnitLinkWireless wireless;

// Apollo MAC address
// Apollo local MAC confirmed: 0x8C, 0x94, 0xDF, 0x92, 0xE2, 0x1C
uint8_t apolloMac[] = {0x8C, 0x94, 0xDF, 0x92, 0xE2, 0x1C};

unsigned long lastSendMs = 0;

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("Atlas ESP-NOW test firmware booting...");
  Serial.println("Role: ATLAS");
  Serial.println("Firmware: 0.1.0");

  bool ok = wireless.begin(ROLE_ATLAS, apolloMac);

  if (!ok) {
    Serial.println("UnitLinkWireless failed to start");
    return;
  }

  Serial.println("Atlas ready");
}

void loop() {
  wireless.loop();

  if (millis() - lastSendMs > 5000) {
    lastSendMs = millis();

    Serial.println("Sending Atlas test message...");
    wireless.sendTestMessage("ATLAS_TEST_001");
  }
}