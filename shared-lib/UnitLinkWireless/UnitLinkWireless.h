#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>

#define UNITLINK_MAGIC 0x554C
#define UNITLINK_VERSION 1

enum UnitLinkRole : uint8_t {
  ROLE_APOLLO = 1,
  ROLE_ATLAS = 2
};

enum UnitLinkPacketType : uint8_t {
  PKT_HELLO = 1,
  PKT_HELLO_ACK = 2,
  PKT_PING = 3,
  PKT_ACK = 4
};

struct __attribute__((packed)) UnitLinkWirelessPacket {
  uint16_t magic;
  uint8_t version;
  uint8_t type;
  uint8_t fromRole;
  uint8_t toRole;
  uint32_t sequence;
  uint32_t uptimeMs;
  char message[32];
};

typedef void (*UnitLinkMessageCallback)(const UnitLinkWirelessPacket& packet);

class UnitLinkWireless {
public:
  bool begin(UnitLinkRole role, const uint8_t peerMac[6]);
  void loop();

  bool sendHello();
  bool sendTestMessage(const char* message);
  bool isLinked();

  void onMessage(UnitLinkMessageCallback callback);

private:
  UnitLinkRole _role;
  uint8_t _peerMac[6];
  bool _linked = false;
  uint32_t _sequence = 0;
  uint32_t _lastHelloMs = 0;

  UnitLinkMessageCallback _messageCallback = nullptr;

  bool addPeer();
  bool sendPacket(UnitLinkPacketType type, const char* message);

  static void onDataSent(const uint8_t* mac, esp_now_send_status_t status);

#if ESP_ARDUINO_VERSION_MAJOR >= 3
  static void onDataRecv(const esp_now_recv_info_t* info, const uint8_t* data, int len);
#else
  static void onDataRecv(const uint8_t* mac, const uint8_t* data, int len);
#endif

  static UnitLinkWireless* instance;
};