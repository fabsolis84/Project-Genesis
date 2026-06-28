#include "UnitLinkWireless.h"

UnitLinkWireless* UnitLinkWireless::instance = nullptr;

bool UnitLinkWireless::begin(UnitLinkRole role, const uint8_t peerMac[6]) {
  instance = this;
  _role = role;
  memcpy(_peerMac, peerMac, 6);

  Serial.println("Starting UnitLinkWireless v0.2...");

  WiFi.mode(WIFI_STA);

  // Atlas is ESP-NOW only, so it can disconnect Wi-Fi.
  // Apollo uses Wi-Fi for MQTT, so never disconnect it here.
  if (_role == ROLE_ATLAS) {
    WiFi.disconnect();
    Serial.println("Wireless role: ATLAS");
    Serial.println("Wi-Fi disconnected for ESP-NOW only mode");
  } else {
    Serial.println("Wireless role: APOLLO");
    Serial.println("Wi-Fi preserved for MQTT + ESP-NOW bridge mode");
  }

  Serial.print("Local MAC: ");
  Serial.println(WiFi.macAddress());

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init failed");
    return false;
  }

  esp_now_register_send_cb(onDataSent);
  esp_now_register_recv_cb(onDataRecv);

  if (!addPeer()) {
    Serial.println("Failed to add ESP-NOW peer");
    return false;
  }

  Serial.println("UnitLinkWireless ready");
  return true;
}

bool UnitLinkWireless::addPeer() {
  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, _peerMac, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_is_peer_exist(_peerMac)) {
    Serial.println("ESP-NOW peer already exists");
    return true;
  }

  esp_err_t result = esp_now_add_peer(&peerInfo);

  if (result == ESP_OK) {
    Serial.println("ESP-NOW peer added");
    return true;
  }

  Serial.print("ESP-NOW peer add failed. Error: ");
  Serial.println(result);
  return false;
}

void UnitLinkWireless::loop() {
  // Atlas initiates.
  // Apollo only listens and replies.
  if (_role != ROLE_ATLAS) {
    return;
  }

  if (!_linked && millis() - _lastHelloMs > 2000) {
    _lastHelloMs = millis();
    sendHello();
  }
}

bool UnitLinkWireless::sendHello() {
  return sendPacket(PKT_HELLO, "UNITLINK_HELLO");
}

bool UnitLinkWireless::sendTestMessage(const char* message) {
  return sendPacket(PKT_PING, message);
}

bool UnitLinkWireless::isLinked() {
  return _linked;
}

void UnitLinkWireless::onMessage(UnitLinkMessageCallback callback) {
  _messageCallback = callback;
}

bool UnitLinkWireless::sendPacket(UnitLinkPacketType type, const char* message) {
  UnitLinkWirelessPacket packet = {};

  packet.magic = UNITLINK_MAGIC;
  packet.version = UNITLINK_VERSION;
  packet.type = type;
  packet.fromRole = _role;
  packet.toRole = (_role == ROLE_APOLLO) ? ROLE_ATLAS : ROLE_APOLLO;
  packet.sequence = ++_sequence;
  packet.uptimeMs = millis();

  strncpy(packet.message, message, sizeof(packet.message) - 1);
  packet.message[sizeof(packet.message) - 1] = '\0';

  esp_err_t result = esp_now_send(_peerMac, (uint8_t*)&packet, sizeof(packet));

  if (result == ESP_OK) {
    Serial.print("ESP-NOW send queued: ");
    Serial.println(packet.message);
    return true;
  }

  Serial.print("ESP-NOW send failed immediately. Error: ");
  Serial.println(result);
  return false;
}

void UnitLinkWireless::onDataSent(const uint8_t* mac, esp_now_send_status_t status) {
  Serial.print("ESP-NOW delivery status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "MAC delivered" : "MAC failed");
}

#if ESP_ARDUINO_VERSION_MAJOR >= 3
void UnitLinkWireless::onDataRecv(const esp_now_recv_info_t* info, const uint8_t* data, int len) {
#else
void UnitLinkWireless::onDataRecv(const uint8_t* mac, const uint8_t* data, int len) {
#endif
  if (!instance) return;

  if (len != sizeof(UnitLinkWirelessPacket)) {
    Serial.print("ESP-NOW packet ignored. Bad length: ");
    Serial.println(len);
    return;
  }

  UnitLinkWirelessPacket packet = {};
  memcpy(&packet, data, sizeof(packet));

  if (packet.magic != UNITLINK_MAGIC) {
    Serial.println("ESP-NOW packet ignored. Bad magic.");
    return;
  }

  if (packet.version != UNITLINK_VERSION) {
    Serial.println("ESP-NOW packet ignored. Bad version.");
    return;
  }

  Serial.println("----- UNITLINK WIRELESS PACKET -----");
  Serial.print("Type: ");
  Serial.println(packet.type);
  Serial.print("From role: ");
  Serial.println(packet.fromRole);
  Serial.print("To role: ");
  Serial.println(packet.toRole);
  Serial.print("Seq: ");
  Serial.println(packet.sequence);
  Serial.print("Message: ");
  Serial.println(packet.message);

  if (packet.toRole != instance->_role) {
    Serial.println("ESP-NOW packet ignored. Not for this role.");
    return;
  }

  if (packet.type == PKT_HELLO) {
    if (!instance->_linked) {
      instance->_linked = true;
      Serial.println("Apollo/Atlas linked!");
    }

    if (instance->_role == ROLE_APOLLO) {
      instance->sendPacket(PKT_HELLO_ACK, "HELLO_ACK");
    }

    return;
  }

  if (packet.type == PKT_HELLO_ACK) {
    if (!instance->_linked) {
      instance->_linked = true;
      Serial.println("UNIT LINK WIRELESS HANDSHAKE COMPLETE");
    }
    return;
  }

  if (instance->_messageCallback != nullptr) {
    instance->_messageCallback(packet);
  } else {
    Serial.println("No wireless message callback registered.");
  }
}