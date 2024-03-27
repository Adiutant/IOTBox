#pragma once
#define DHT_IDLE 100
#include <DHT.h>

template <class T> class Data
{
    T m_data;
public:
    Data<T>(T data) {
      m_data = data;
    }
    T data() const {
      return m_data;
    }
    ~Data() = default;

};

class SensorsData {
private:
Data<float> m_humidity{0};
DHT* m_dht;
uint32_t m_dht_cycle = 0;
bool m_read_temp = true;
bool m_read_hum = false;
Data<float> m_air_temperature{0};
public:
const Data<float> & get_humidity() const;
const Data<float> & get_air_temp() const;
void update();
SensorsData(DHT *dht);
~SensorsData() = default;
};