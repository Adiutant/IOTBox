#include "sensors_data.hpp"

void SensorsData::update(){
  if (m_read_temp && millis() - m_dht_cycle >= DHT_IDLE) {
    m_air_temperature = Data<float>(m_dht->readTemperature());
    m_read_temp = false;
    m_read_hum = true;
    m_dht_cycle = millis();
  } else if (m_read_hum &&  millis() - m_dht_cycle >= DHT_IDLE) {
    m_humidity = Data<float>(m_dht->readHumidity());
    m_read_hum = false;
    m_read_temp = true;
    m_dht_cycle = millis();
  }

}
const Data<float> & SensorsData::get_humidity() const {
  return m_humidity;
}
const Data<float> & SensorsData::get_air_temp() const {
  return m_air_temperature;
}

SensorsData::SensorsData(DHT *dht) {
  m_dht = dht;
}
