#include "strip_driver.hpp"

StripDriver::StripDriver(Adafruit_NeoPixel & strip) {
  m_strip = strip;
}

void StripDriver::init() {
  m_strip.begin();
  m_strip.setBrightness(BRIGHTNESS); //adjust brightness here
  m_strip.show(); // Initialize all pixels to 'off'
  clear_ring();
}

uint32_t StripDriver::wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return m_strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return m_strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return m_strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

void StripDriver::cold_animation() {
  if (m_context.stopped) {
    return;
  }
  for(uint16_t i=0; i<m_strip.numPixels(); i+=2) {
    m_strip.setPixelColor(i, BLUE_COLOR);
    m_strip.show();
  }
  m_strip.setBrightness(m_context.brightness);
  m_context.brightness-=5;
  m_strip.show();
  if (m_context.brightness <= 0) {
    m_context.stopped = true;
  }
}
void StripDriver::clear_ring(){
  colorWipe(m_strip.Color(0, 0, 0));
  m_strip.setBrightness(BRIGHTNESS);
  m_strip.show();
}

void StripDriver::fade_animation() {
  if (m_context.stopped) {
    return;
  }
  m_strip.setBrightness(m_context.brightness);
  m_strip.show();
  m_context.brightness -= 2;
  if (m_context.brightness <= 0) {
    m_context.stopped = true;
    clear_ring();
  }
}

void StripDriver::simple_color_animation() {
  if (m_context.stopped) {
    return;
  }
  colorWipe(m_context.color);
  m_strip.setBrightness(m_context.brightness);
  m_strip.show();
  m_context.stopped = true;
}

void StripDriver::hot_animation() {
  if (m_context.stopped) {
    return;
  }
  for(uint16_t i=0; i<m_strip.numPixels(); i+=2) {
    m_strip.setPixelColor(i, ORANGE_COLOR);
    m_strip.show();
  }
  m_strip.setBrightness(m_context.brightness);
  m_context.brightness-=5;
  m_strip.show();
  if (m_context.brightness <= 0) {
    m_context.stopped = true;
  }
}

void StripDriver::colorWipe(uint32_t c) {
  for(uint16_t i=0; i<m_strip.numPixels(); i++) {
      m_strip.setPixelColor(i, c);
      m_strip.show();
  }
}

void StripDriver::draw() {
  switch (m_context.job) {
    case Job::Rainbow:
    rainbow();
    break;
    case Job::FadeAnimation:
    fade_animation();
    break;
    case Job::SimpleColorTask:
    simple_color_animation();
    break;
    case Job::HotAnimation:
    hot_animation();
    break;
    case Job::ColdAnimation:
    cold_animation();
    break;
  }
}

void StripDriver::set_rainbow_task() {
  m_context.stopped = false;
  m_context.brightness = BRIGHTNESS;
  m_context.color = 0;
  m_context.job = Job::Rainbow;
  Serial.println("set rainbow task");
}

void StripDriver::set_hot_animation_task() {
  if (m_context.stopped == false) {
    return;
  }
  m_context.stopped = false;
  m_context.brightness = BRIGHTNESS;
  m_context.color = 0;
  m_context.job = Job::HotAnimation;
  Serial.println("set hot animation task");
}

void StripDriver::set_cold_animation_task() {
  if (m_context.stopped == false) {
    return;
  }
  m_context.stopped = false;
  m_context.brightness = BRIGHTNESS;
  m_context.color = 0;
  m_context.job = Job::ColdAnimation;
  Serial.println("set cold animation task");
}


void StripDriver::set_simple_color_task(uint32_t color, uint8_t brightness) {
  m_context.stopped = false;
  m_context.brightness = brightness;
  m_context.color = color;
  m_context.job = Job::SimpleColorTask;
  Serial.println("set simple color task");
}

const Context & StripDriver::get_context() const {
  return m_context;
}

void StripDriver::set_fade_animation_task() {
  if (m_context.stopped == false) {
    return;
  }
  m_context.stopped = false;
  m_context.brightness = BRIGHTNESS;
  m_context.color = 0;
  m_context.job = Job::FadeAnimation;
  Serial.println("set fade task");
}

void StripDriver::rainbow() {
  if (m_context.stopped) {
    return;
  }
  for(uint8_t i=0; i<m_strip.numPixels(); i++) {
    m_strip.setPixelColor(i, wheel((i + m_context.color) & 255));
  }
  m_strip.show();
  if (m_context.color == 255 * 3) {
    m_context.stopped = true;
  }
  m_context.color += 1;
}