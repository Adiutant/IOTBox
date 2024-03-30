#pragma once

#include <ESP8266WiFi.h>

struct LoginPass {
  char ssid[20];
  char pass[20];
};
enum InterfaceState {
  Pending = 1 << 0,
  WifiNet = 1 << 2,
  WifiAp = 1 << 3,
  Connecting = 1 << 4
};

enum SignalToOs {
  Idle = 1 << 0,
  StartLocal = 1 << 2
};

class NetworkManager {
private:
LoginPass credentials;
InterfaceState interface_state = Pending;
int reconnection_attempts = 25;
public:
NetworkManager();
~NetworkManager() = default;
void set_creds(const LoginPass &creds);
InterfaceState get_interface_state() const;
LoginPass get_credentials() const;
SignalToOs loop();
};