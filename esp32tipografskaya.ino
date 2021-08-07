#include "set.h"  //подключение баблиотек в отдельном файле set.h

void setup() {

  Serial.begin(115200);
  delay(10);
  Serial.println("esp32Tipografskaya");
  Serial.println("Start 4-FS");
  FS_init(); //Запускаем файловую систему
  Serial.println("Step7-FileConfig");
  configSetup = readFile("config.json", 4096);
  jsonWrite(configJson, "SSDP", jsonRead(configSetup, "SSDP"));
  Serial.println(configSetup);
  Serial.println("Start 1-WIFI");
  WIFIinit();   //Запускаем WIFI
  MDNS.begin(host);  //http://esp32.local
  Serial.println("mDNS responder started");
  Serial.println("Start 8-Time");
  Time_init();       // Получаем время из сети
  Serial.println("Start 3-SSDP");
  SSDP_init();       //Настраиваем и запускаем SSDP интерфейс
  Serial.println("Start 2-WebServer");
  HTTP_init();      //Настраиваем и запускаем HTTP интерфейс
  //Настраиваем и запускаем webSoket интерфейс
  Serial.println("Start 14-webSoket_init");
  webSoket_init();   //Настраиваем и запускаем webSoket интерфейс
  Serial.println("Start 13-sec_init");
  sec_init();       // Включаем Вывод времени и даты каждую секунду
  Serial.println("Start 13-BME280(0X77)");
  BME_init();       // Инициализация датчиков BME280
  Serial.println("Start 15-AHT10");
  AHT_init();       // Инициализация датчиков AHT10
  Serial.println("Start 16-Датчики уровней");
  level_init();     // Инициализация датчиков уровня воды
  Serial.println("Start 17-SMPT Отправки оповещения на почту");
  SMTP_Init();      // Подготовка сообщения на почту
  Serial.println("Start 18-Подготовка реле");
  relays_init();    // Подготовка реле к работе
}

void loop() {
  ts.update(); //планировщик задач
  HTTP.handleClient(); // Работа Web сервера
  yield();
  webSocket.loop(); // Работа webSocket

String l11 = String(l1);
SoketData ("l11", l11, jsonRead(configJson,"l11"));
jsonWrite(configJson, "l11", l11);

String l22 = String(l2);
SoketData ("l22", l22, jsonRead(configJson,"l22"));
jsonWrite(configJson, "l22", l22);  

String l33 = String(l3);
SoketData ("l33", l33, jsonRead(configJson,"l33"));
jsonWrite(configJson, "l33", l33);  

String lLcoun = String(lLcount);
SoketData ("lLcoun", lLcoun, jsonRead(configJson,"lLcoun"));
jsonWrite(configJson, "lLcoun", lLcoun);

String lHcoun = String(lHcount);
SoketData ("lHcoun", lHcoun, jsonRead(configJson,"lHcoun"));
jsonWrite(configJson, "lHcoun", lHcoun);

String lWcoun = String(lWcount);
SoketData ("lWcoun", lWcoun, jsonRead(configJson,"lWcoun"));
jsonWrite(configJson, "lWcoun", lWcoun);


}
