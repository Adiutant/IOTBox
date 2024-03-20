#include <Adafruit_NeoPixel.h>

#define BLUE_COLOR 0x00BFFF
#define ORANGE_COLOR 0xff8500
#define BRIGHTNESS 200

enum Job {
  Rainbow = 0, 
  HotAnimation,
  ColdAnimation,
  FadeAnimation, 
  SimpleColorTask
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
    void fade_animation();
    void simple_color_animation();
    void colorWipe(uint32_t c);
    void rainbow();
    Adafruit_NeoPixel m_strip;
    Context m_context;
  public:
    const Context & get_context() const;
    void draw();
    void set_rainbow_task();
    void set_fade_animation_task();
    void set_hot_animation_task();
    void set_cold_animation_task();
    void set_simple_color_task(uint32_t color, uint8_t brightness);
    void init();
    StripDriver(Adafruit_NeoPixel & strip);
};

typedef void(StripDriver::*DrawFunction)(void);

