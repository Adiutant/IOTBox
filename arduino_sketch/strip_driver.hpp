#include <Adafruit_NeoPixel.h>

#define BLUE_COLOR 0x00BFFF
#define ORANGE_COLOR 0xff8500
#define BRIGHTNESS 200

enum Job {
  Rainbow, 
  HotAnimation,
  ColdAnimation
};


struct Context {
  uint8_t brightness;
  uint32_t color;
  bool stopped = false;
  uint8_t num_led;
  Job job;
  
};


class StripDriver {
  private:
    uint32_t wheel(byte WheelPos);
    void cold_animation();
    void clear_ring();
    void hot_animation();
    void colorWipe(uint32_t c, uint8_t wait);
    Adafruit_NeoPixel m_strip;
    Context m_context;
  public:
    void rainbow();
    void draw();
    void set_rainbow_task();
    void init();
    StripDriver(Adafruit_NeoPixel & strip);
};

typedef void(StripDriver::*DrawFunction)(void);

