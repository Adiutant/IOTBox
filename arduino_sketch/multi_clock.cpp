#include "multi_clock.hpp"

MultiClock::MultiClock(const LedDisplayMode &initial_mode, int button_pin) {
  m_led_display_mode = initial_mode;
  m_button_pin = button_pin;
}
void MultiClock::set_led_display_mode_from_button() {
  bool button_is_up = digitalRead(m_button_pin);
  bool change_state = false;
  if (m_button_was_up && !button_is_up) {
    button_is_up = digitalRead(m_button_pin);
    if (!button_is_up) { change_state = !change_state; }
  }
  m_button_was_up = button_is_up;
  if (change_state && m_led_display_mode == LedDisplayMode::Time) {
    m_led_display_mode = LedDisplayMode::Temperature;
  } else if (change_state && m_led_display_mode == LedDisplayMode::Temperature) {
    m_led_display_mode = LedDisplayMode::Humidity;
  } else if (change_state && m_led_display_mode == LedDisplayMode::Humidity) {
    m_led_display_mode = LedDisplayMode::Time;
  }
}
void MultiClock::set_hour(int hour) {
  m_hour = hour;
}
void MultiClock::set_minute(int minute) {
  m_minute = minute;
}
int MultiClock::get_hour() const {
  return m_hour;
}
int MultiClock::get_minute() const {
  return m_minute;
}

DisplayTime MultiClock::get_time() {
  int displaytime = (m_hour * 100) + m_minute;
  uint32_t dot_mask = m_display_dots_mask ? 0b11100000 : 0;
  m_display_dots_mask = !m_display_dots_mask;
  return { .display_time=displaytime, .dot_mask=dot_mask };
}

LedDisplayMode MultiClock::get_led_display_mode() const {
  return m_led_display_mode;
}