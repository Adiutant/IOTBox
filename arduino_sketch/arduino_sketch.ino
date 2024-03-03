#include <GyverOS.h>
#include <OneWire.h>
#include <DHT.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <GyverBME280.h>
#include <TM1637Display.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "strip_driver.hpp"

#define CLK D2
#define DIO D1
#define TIME_FORMAT        12    // 12 = 12 hours format || 24 = 24 hours format 

#define COMFORT_TEMP_LOW_EDGE 18
#define COMFORT_TEMP_HIGH_EDGE 25

struct DisplayTime {
  int hour;
  int minute;
};

enum LedDisplayMode {
  Time = 1 << 0,
  Temperature = 1 << 2,
  Humidity = 1 << 3
} led_display_mode;

const char* ssid = "APLN-0002";
const char* password = "93154000";

// установка параметров подключения к MQTT брокеру
const char* mqtt_server = "192.168.0.101";
const int mqtt_port = 1883;
const char* mqtt_user = "mqtt";
const char* mqtt_password = "lr93154000";

// переменные для хранения показаний термодатчиков и датчика влажности
float airTemp;
float humidity;
int hour;
int minute;
bool display_dots_mask = true;
boolean button_was_up = false;

TM1637Display display = TM1637Display(CLK, DIO);
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "ru.pool.ntp.org");
DHT dht(D4, DHT22);
WiFiClient espClient;
PubSubClient client(espClient);
Adafruit_NeoPixel strip = Adafruit_NeoPixel(8, D6, NEO_GRB + NEO_KHZ800);
StripDriver strip_driver(strip);
GyverOS<10> OS;

// функция для подключения к MQTT брокеру
void reconnect() {
  if (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP8266Client", mqtt_user, mqtt_password)) {
      //if (client.connect("ESP8266Client")) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
    }
  }
}

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void update_dht_info() {
  // замер температуры воздуха и влажности
  humidity = dht.readHumidity();
  delay(100);
  airTemp = dht.readTemperature();
  Serial.println(humidity);
  Serial.println(airTemp);
  char airTempStr[10];
  dtostrf(airTemp, 4, 2, airTempStr);
  char humidityStr[10];
  dtostrf(humidity, 4, 2, humidityStr);
  if (client.connected()) {
    client.publish("home/room_roman/air_temperature", airTempStr);
    client.publish("home/room_roman/humidity", humidityStr);
  }
}

void reconnect_client() {
  if (!client.connected()) {
    reconnect();
  }
}

void client_loop() {
  if (client.connected()) {
    client.loop();
  }
}

void strip_driver_draw_wrapper() {
  strip_driver.draw();
}

void update_time() {
  if (WiFi.status() != WL_CONNECTED) {
    minute++;
    if (minute == 60) {
      hour++;
      minute = 0;
    }
    if (hour == 24) {
      hour = 0;
      minute = 0;
    }
    return;
  }
  timeClient.update();
  unsigned long now_epoch_time = timeClient.getEpochTime();
  String formatted_time = timeClient.getFormattedTime();
  struct tm * ptm;
  time_t rawtime = timeClient.getEpochTime();
  ptm = localtime (&rawtime);
  hour = ptm->tm_hour;
  minute = ptm->tm_min;
}

void display_led_info() {
  switch (led_display_mode) {
    case Time:
    {
      int displaytime = (hour * 100) + minute;
      uint32_t dot_mask = display_dots_mask ? 0b11100000 : 0;
      display_dots_mask = !display_dots_mask;
      display.showNumberDecEx(displaytime, dot_mask, true);
    }
    break;
    case Temperature:
    {
      display.showNumberDecEx(airTemp, 0, true);
    }
    break;
    case Humidity:
    {
      display.showNumberDecEx(humidity, 0, true);
    }
    break;
  }
}

void stop_welcome_animation_subprocess() {
  if (strip_driver.get_context().stopped) {
    strip_driver.set_fade_animation_task();
    OS.detach(6);
  }
}

void check_button_pressed() {
  boolean button_is_up = digitalRead(D7);
  boolean change_state = false;
  if (button_was_up && !button_is_up) {
    button_is_up = digitalRead(D7);
    if (!button_is_up) { change_state = !change_state; }
  }
  button_was_up = button_is_up;
  if (change_state && led_display_mode == LedDisplayMode::Time) {
    led_display_mode = LedDisplayMode::Temperature;
  } else if (change_state && led_display_mode == LedDisplayMode::Temperature) {
    led_display_mode = LedDisplayMode::Humidity;
  } else if (change_state && led_display_mode == LedDisplayMode::Humidity) {
    led_display_mode = LedDisplayMode::Time;
  }
}

void setup() {
  led_display_mode = LedDisplayMode::Time;
  Serial.begin(9600);
  display.clear();
  display.setBrightness(7);
  strip_driver.init();
  strip_driver.set_rainbow_task();
  setup_wifi();
  timeClient.begin();
  timeClient.setTimeOffset(10800);
  client.setServer(mqtt_server, mqtt_port);
  pinMode(D4, INPUT);
  pinMode(D0, OUTPUT);
  digitalWrite(D0, HIGH);
  pinMode(D7, INPUT_PULLUP); 
  dht.begin();
  update_time();
  OS.attach(0, update_dht_info, 2000);
  OS.attach(1, reconnect_client, 5000);
  OS.attach(2, client_loop, 500);
  OS.attach(3, strip_driver_draw_wrapper , 16);
  OS.attach(4, display_led_info, 1000);
  OS.attach(5, update_time , 60000);
  OS.attach(6, stop_welcome_animation_subprocess, 100);
  OS.attach(7, check_button_pressed, 200);

}

void loop() {
  OS.tick();	
}