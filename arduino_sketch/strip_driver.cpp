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
  for(uint16_t j=0; j<BRIGHTNESS; j+=2) {
    for(uint16_t i=0; i<m_strip.numPixels(); i+=2) {
      m_strip.setPixelColor(i, BLUE_COLOR);
      m_strip.show();
      delay(10);
    }
    m_strip.setBrightness(BRIGHTNESS - j);
    m_strip.show();
  }
}
void StripDriver::clear_ring(){
  colorWipe(m_strip.Color(0, 0, 0), 50);
  m_strip.setBrightness(BRIGHTNESS);
}

void StripDriver::hot_animation() {
  for(uint16_t j=0; j<BRIGHTNESS; j+=5) {
    for(uint16_t i=0; i<m_strip.numPixels(); i+=2) {
      m_strip.setPixelColor(i, ORANGE_COLOR);
      m_strip.show();
      delay(10);
    }
    m_strip.setBrightness(BRIGHTNESS - j);
    m_strip.show();
  }
}

void StripDriver::colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<m_strip.numPixels(); i++) {
      m_strip.setPixelColor(i, c);
      m_strip.show();
      delay(wait);
  }
}

void StripDriver::draw() {
  switch (m_context.job) {
    case Job::Rainbow:
    rainbow();
  }
}


void StripDriver::set_rainbow_task() {
  m_context.brightness = BRIGHTNESS;
  m_context.color = 85;
  m_context.job = Job::Rainbow;
  Serial.println("set rainbow task");
}

void StripDriver::rainbow() {
  if (m_context.stopped) {
    return;
  }
  for(uint8_t i=0; i<m_strip.numPixels(); i++) {
    m_strip.setPixelColor(i, wheel((i + m_context.color) & 255));
  }
  m_strip.show();
  if (m_context.color == 170) {
    m_context.stopped = true;
  }
  m_context.color += 1;
}