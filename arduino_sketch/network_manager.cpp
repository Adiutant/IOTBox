#include "network_manager.hpp"

void NetworkManager::set_creds(const LoginPass &creds){
  credentials = creds;
  credentials_changed = true;
}

NetworkManager::NetworkManager(const WiFiClient &client) {
  espClient = client;
}

InterfaceState NetworkManager::get_interface_state() const {
  return interface_state;
}

LoginPass NetworkManager::get_credentials() const {
  return credentials;
}

SignalToOs NetworkManager::loop() {
  if (interface_state == WifiAp && !credentials_changed) {
    return SignalToOs::Idle;
  }
  if (WiFi.status() == WL_CONNECTED && interface_state == WifiNet ) {
    return SignalToOs::Idle;
  }
  if (credentials_changed) {
    Serial.print("Connecting to ");
    //EEPROM.get(128, mqtt);
    Serial.println(credentials.ssid);
    WiFi.begin(credentials.ssid, credentials.pass);
  }
  credentials_changed = false;
  if (WiFi.status() != WL_CONNECTED) {
    reconnection_attempts -= 1;
    if (reconnection_attempts == 0) {
      WiFi.disconnect();
      Serial.println("cant connect to network: ");
      Serial.println(credentials.ssid);
      interface_state = WifiAp;
      reconnection_attempts = 15;
      return SignalToOs::StartLocal;
    }
  } else if (WiFi.status() == WL_CONNECTED) {
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    interface_state = WifiNet;
    reconnection_attempts = 15;
  }
  return SignalToOs::Idle;
}