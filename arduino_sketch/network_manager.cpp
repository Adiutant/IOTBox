#include "network_manager.hpp"

void NetworkManager::set_creds(const LoginPass &creds){
  credentials = creds;
  interface_state = Pending;
}

NetworkManager::NetworkManager() {
}

InterfaceState NetworkManager::get_interface_state() const {
  return interface_state;
}

LoginPass NetworkManager::get_credentials() const {
  return credentials;
}

SignalToOs NetworkManager::loop() {
  if (WiFi.status() != WL_CONNECTED && interface_state == WifiNet) {
    interface_state = Pending;
  }
  if (interface_state == WifiAp) {
    return SignalToOs::Idle;
  }
  if (interface_state == WifiNet ) {
    return SignalToOs::Idle;
  }
  if (interface_state == Pending) {
    Serial.print("Connecting to ");
    //EEPROM.get(128, mqtt);
    Serial.println(credentials.ssid);
    interface_state = Connecting;
    WiFi.begin(credentials.ssid, credentials.pass);
  }
  if (WiFi.status() != WL_CONNECTED) {
    reconnection_attempts -= 1;
    if (reconnection_attempts == 0) {
      WiFi.disconnect();
      Serial.println("cant connect to network: ");
      Serial.println(credentials.ssid);
      interface_state = WifiAp;
      reconnection_attempts = 25;
      return SignalToOs::StartLocal;
    }
  } else if (WiFi.status() == WL_CONNECTED) {
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    interface_state = WifiNet;
    reconnection_attempts = 25;
  }
  return SignalToOs::Idle;
}