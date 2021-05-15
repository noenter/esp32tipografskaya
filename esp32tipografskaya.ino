#include "set.h"

void setup() {

  Serial.begin(115200);
  delay(5);
//  SerialBT.begin("ESP32test"); //Bluetooth device name
  Serial.println("");
  //Запускаем файловую систему
  Serial.println("Start 4-FS");
  FS_init();
  Serial.println("Step7-FileConfig");
  configSetup = readFile("config.json", 4096);
  jsonWrite(configJson, "SSDP", jsonRead(configSetup, "SSDP"));
  Serial.println(configSetup);
  Serial.println("Start 1-WIFI");
  //Запускаем WIFI
  WIFIinit();
  
  if (!MDNS.begin(host)) { //http://esp32.local
    Serial.println("Error setting up MDNS responder!");
    while (1) {
      delay(1000);
    }
  }
  Serial.println("mDNS responder started");
  Serial.println("Start 8-Time");
  // Получаем время из сети
  Time_init();
  //Настраиваем и запускаем SSDP интерфейс
  Serial.println("Start 3-SSDP");
  SSDP_init();
  //Настраиваем и запускаем HTTP интерфейс
  Serial.println("Start 2-WebServer");
  HTTP_init();
  //Настраиваем и запускаем webSoket интерфейс
  Serial.println("Start 14-webSoket_init");
  webSoket_init();
    // Включаем Вывод времени и даты каждую секунду
  Serial.println("Start 13-sec_init");
  sec_init();
  Serial.println("Start 13-BME280(0X77)");
//  DHT_init();
BME_init();
AHT_init();

level_init();

SMTP_Init();

relays_init();

}

void loop() {
  ts.update(); //планировщик задач
  HTTP.handleClient(); // Работа Web сервера
  yield();
  webSocket.loop(); // Работа webSocket
//  dnsServer.processNextRequest(); // Для работы DNS в режиме AP

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

//if (Serial.available()) {
//    SerialBT.write(Serial.read());
//  }
//  if (SerialBT.available()) {
//    Serial.write(SerialBT.read());
//  }


}
