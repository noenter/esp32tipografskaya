
void HTTP_init(void) {
//  jsonWrite(configJson, "flashChip", String(ESP.getFlashChipId(), HEX));

  // -------------------построение графика по запросу вида /charts.json?data=A0&data2=stateLed
  HTTP.on("/charts.json", HTTP_GET, []() {
    String message = "{";                       // создадим json на лету
    uint8_t j = HTTP.args();                    // получим количество аргументов
    for (uint8_t i = 0; i < j; i++) { // Будем читать все аргументы по порядку
      String nameArg = HTTP.argName(i);         // Возьмем имя аргумента и зададим массив с ключём по имени аргумента
      String keyArg = HTTP.arg(i);
      String value = jsonRead(configJson, HTTP.arg(i)); // Считаем из configJson значение с ключём keyArg
      if (value != "")  { // если значение есть добавим имя массива
        message += "\"" + nameArg + "\":["; // теперь в строке {"Имя аргумента":[
        message += value; // добавим данные в массив теперь в строке {"Имя аргумента":[value
        value = "";       // очистим value
      }
      message += "]"; // завершим массив теперь в строке {"Имя аргумента":[value]
      if (i<j-1) message += ","; // если элемент не последний добавит , теперь в строке {"Имя аргумента":[value],
    }
    message += "}";
    // теперь json строка полная
    jsonWrite(message, "points", 10); // зададим количество точек по умолчанию
    jsonWrite(message, "refresh", 1000); // зададим время обнавления графика по умолчанию
    HTTP.send(200, "application/json", message);
  });


  // --------------------Выдаем данные configJson
  HTTP.on("/config.live.json", HTTP_GET, []() {
    HTTP.send(200, "application/json", configJson);
  });
  // -------------------Выдаем данные configSetup
  HTTP.on("/config.setup.json", HTTP_GET, []() {
    HTTP.send(200, "application/json", configSetup);
  });
  // -------------------Перезагрузка модуля
  HTTP.on("/restart", HTTP_GET, []() {
    String restart = HTTP.arg("device");          // Получаем значение device из запроса
    if (restart == "ok") {                         // Если значение равно Ок
      HTTP.send(200, "text / plain", "Reset OK"); // Oтправляем ответ Reset OK
      ESP.restart();                                // перезагружаем модуль
    }
    else {                                        // иначе
      HTTP.send(200, "text / plain", "No Reset"); // Oтправляем ответ No Reset
    }
  });
// ssr
HTTP.on("/pump", HTTP_GET, []() {pump();});  

  // реле1
  HTTP.on("/relay1", HTTP_GET, []() {relay1();});  

  // реле2
  HTTP.on("/relay2", HTTP_GET, []() {relay2();});

 HTTP.on("/serverIndex", HTTP_GET, []() {
    HTTP.sendHeader("Connection", "close");
    HTTP.send(200, "text/html", serverIndex);
  });
  /*handling uploading firmware file */
  HTTP.on("/update", HTTP_POST, []() {
    HTTP.sendHeader("Connection", "close");
    HTTP.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
    ESP.restart();
  }, []() {
    HTTPUpload& upload = HTTP.upload();
    if (upload.status == UPLOAD_FILE_START) {
      Serial.printf("Update: %s\n", upload.filename.c_str());
      if (!Update.begin(UPDATE_SIZE_UNKNOWN)) { //start with max available size
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_WRITE) {
      /* flashing firmware to ESP*/
      if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_END) {
      if (Update.end(true)) { //true to set the size to the current progress
        Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
      } else {
        Update.printError(Serial);
      }
    }
  });

  // Добавляем функцию Update для перезаписи прошивки по WiFi при 1М(256K SPIFFS) и выше
//  httpUpdater.setup(&HTTP);
  // Запускаем HTTP сервер
  HTTP.begin();
}
