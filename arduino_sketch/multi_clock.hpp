#pragma once
#include <Arduino.h>

enum LedDisplayMode {
  Time = 1 << 0,
  Temperature = 1 << 2,
  Humidity = 1 << 3
};

struct DisplayTime {
  uint16_t display_time;
  uint32_t dot_mask;
};

class MultiClock {
private:
int m_hour = 12;
int m_minute = 30;
int m_button_pin;
bool m_display_dots_mask = true;
LedDisplayMode m_led_display_mode;
bool m_button_was_up = false;
public:
MultiClock(const LedDisplayMode &initial_mode, int button_pin);
~MultiClock() = default;
void set_led_display_mode_from_button();
void set_hour(int hour);
void set_minute(int minute);
int get_hour() const;
int get_minute() const;
DisplayTime get_time();
LedDisplayMode get_led_display_mode() const;


};