# IOTBox написан на Arduino ESP8266 NodeMCU3
## Концепт устройства
Умная колонка с mqtt интеграцией организация программного кода выполнена с помощью GyverOs, в потоке выполнения убраны практически все блокирующие вызовы delay. Написан свой драйвер для светодиодной матрицы работающий без задержек. 
Есть офлайн и онлайн мод в онлайн моде интегрирован в платформу greenpl идентификатор al_project. В офлайн моде является хостингом страницы для управления.
## Онлайн мод 
![image](https://github.com/Adiutant/IOTBox/assets/17684112/a176489a-6a3a-4fc9-9f2b-3e79f17bbb14)
![image](https://github.com/Adiutant/IOTBox/assets/17684112/44d0a61c-9969-4a2c-baf8-969641dd5eb2)

## Офлайн мод 

## Библиотеки
GyverOS
DHT
PubSubClient
Adafruit_Sensor
GyverBME280
TM1637Display
NTPClient
WiFiUdp
GyverPortal
ArduinoJson

## Схема и используемые компоненты
https://www.ozon.ru/product/bluetooth-audio-modul-vhm-314-mp3-dekoder-bez-poter-plata-audiopriemnika-besprovodnogo-stereo-663273907/

https://www.ozon.ru/product/rupornyy-gromkogovoritel-dinamik-8-om-1-vt-23mm-korpus-dlya-dinamikov-933521322/

https://www.ozon.ru/product/sensornyy-datchik-kasaniya-ttp223-knopka-vyklyuchatel-komplektuyushchie-dlya-platformy-943198718/

https://www.ozon.ru/product/modul-tm-1637-svetodiodnyy-indikator-dlya-arduino-3-sht-956940693/

https://www.ozon.ru/search/?deny_category_prediction=true&from_global=true&text=Электронный+модуль&product_id=624374772



D2 - LED CLK
D1 - LED SDA
D0 - DHT VCC
D4 - DHT IN
D7 - BUTTON IN
D6 - LED RING IN

VCC и GND устройств развести в шину
модуль bluetooth:

шина left шина right шина gnd звука и подключить динамики соответственно

