
#include <WiFi.h>        //Содержится в пакете. Видео с уроком http://esp8266-arduinoide.ru/step1-wifi
#include <WebServer.h>   //Содержится в пакете. Видео с уроком http://esp8266-arduinoide.ru/step2-webserver
#include <WiFiClient.h>
#include <ESP32SSDP.h>        //Содержится в пакете. Видео с уроком http://esp8266-arduinoide.ru/step3-ssdp
#include <FS.h>                 //Содержится в пакете. Видео с уроком http://esp8266-arduinoide.ru/step4-fswebserver
#include <ESPmDNS.h>
#include <Update.h>

//#include <SPIFFS.h>
//                    ПЕРЕДАЧА ДАННЫХ НА WEB СТРАНИЦУ. Видео с уроком http://esp8266-arduinoide.ru/step5-datapages/
//                    ПЕРЕДАЧА ДАННЫХ С WEB СТРАНИЦЫ.  Видео с уроком http://esp8266-arduinoide.ru/step6-datasend/
#include <ArduinoJson.h>        //Установить из менеджера библиотек. https://arduinojson.org/
//                    ЗАПИСЬ И ЧТЕНИЕ ПАРАМЕТРОВ КОНФИГУРАЦИИ В ФАЙЛ. Видео с уроком http://esp8266-arduinoide.ru/step7-fileconfig/
//#include <ESP8266HTTPUpdateServer.h>  //Содержится в пакете.  Видео с уроком http://esp8266-arduinoide.ru/step8-timeupdate/
//#include <DNSServer.h> //Содержится в пакете.  // Для работы символьных имен в режиме AP отвечает на любой запрос например: 1.ru
#include <TickerScheduler.h>         //https://github.com/Toshik/TickerScheduler Видео с уроком http://esp8266-arduinoide.ru/step8-timeupdate/
#include <WebSocketsServer.h>    //https://github.com/Links2004/arduinoWebSockets 
// Библиотеки устройств
// #include <DHT.h>                     //https://github.com/markruys/arduino-DHT   Support for DHT11 and DHT22/AM2302/RHT03

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#include "ESP32_MailClient.h"
#define emailSenderAccount    "esp32home76@gmail.com"    
#define emailSenderPassword   "esp987654321"
#define emailRecipient        "serov-online@mail.ru"
#define smtpServer            "smtp.gmail.com"
#define smtpServerPort        465
#define emailSubject          "ESP32 Home76"
SMTPData smtpData;


#define FILESYSTEM SPIFFS
// You only need to format the filesystem once
#define FORMAT_FILESYSTEM false

#if FILESYSTEM == FFat
#include <FFat.h>
#endif
#if FILESYSTEM == SPIFFS
#include <SPIFFS.h>
#endif

const int lLow = 36;
const int lHigh = 34;
const int lWar = 35;
int lSel = 3700;
int linfo = 0;
int lLcount = 0;
int lHcount = 0;
bool l1 = false;
bool l2 = false;

const int pumppin = 27;
const int relay1pin = 32;
const int relay2pin = 33;
bool pumpstat = false;

boolean r1 = false;
boolean r2 = false;
String r1_Name = "OFF";
String r2_Name = "OFF";
String pump_Name = "OFF";


// Объект для обнавления с web страницы
//ESP8266HTTPUpdateServer httpUpdater;

// Web интерфейс для устройства
WebServer HTTP(80);

// Для файловой системы
File fsUploadFile;

// Для работы символьных имен в режиме AP
//DNSServer dnsServer;

//Планировщик задач (Число задач)
TickerScheduler ts(4);

// Объект для  webSocket
WebSocketsServer webSocket = WebSocketsServer(81);

// Датчик DHT
//DHT dht;
//#define dhtPin 4
Adafruit_BME280 bme1;


String configSetup = "{}"; // данные для config.setup.json
String configJson = "{}";  // данные для config.live.json
