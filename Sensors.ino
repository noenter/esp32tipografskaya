// -----------------  DHT
void BME_init() {
  bme1.begin(0x77);
    jsonWrite(configJson, "t0", bme1.readTemperature());  // отправить температуру в configJson
    jsonWrite(configJson, "h0", bme1.readHumidity());        // отправить влажность в configJson

    ts.add(0, 2000, [&](void*) { // Запустим задачу 0 с интервалом test
      String t0 = (String)bme1.readTemperature();
      String h0 = (String)bme1.readHumidity();
      SoketData ("t0", t0, jsonRead(configJson,"t0"));
      SoketData ("h0", h0, jsonRead(configJson,"h0"));
      jsonWrite(configJson, "t0", t0);   // отправить температуру в configJson
      jsonWrite(configJson, "h0", h0);         // отправить влажность в configJson      
      //Serial.print(".");
    }, nullptr, true);
  }
void level_init() {
  pinMode(pumppin, OUTPUT);
  digitalWrite(pumppin, LOW);  // реле выключено
  jsonWrite(configJson, "lLow", analogRead(lLow));  // отправить температуру в configJson
  jsonWrite(configJson, "lHigh", analogRead(lHigh));
  jsonWrite(configJson, "lWar", analogRead(lWar));
  jsonWrite(configJson, "pumpstat", pumpstat);

    ts.add(2, 1000, [&](void*) { // Запустим задачу
      String lLow = String(analogRead(36));
      String lHigh = String(analogRead(34));
      String lWar = String(analogRead(35));
      SoketData ("lLow", lLow, jsonRead(configJson,"lLow"));
      SoketData ("lHigh", lHigh, jsonRead(configJson,"lHigh"));
      SoketData ("lWar", lWar, jsonRead(configJson,"lWar"));
      jsonWrite(configJson, "lLow", lLow);   // отправить температуру в configJson
      jsonWrite(configJson, "lHigh", lHigh);         // отправить влажность в configJson      
      jsonWrite(configJson, "lWar", lWar);
      //Serial.print(".");
    }, nullptr, true);

  ts.add(3, 2000, [&](void*) { // Запустим задачу
    int lL = analogRead(lLow);
    int lH = analogRead(lHigh); // поменять местами верхний с аварийным
    int lW = analogRead(lWar);

    String lSelect = String(lSel);
    SoketData ("lSelect", lSelect, jsonRead(configJson,"lSelect"));
    jsonWrite(configJson, "lSelect", lSelect);


if (l1 == false){
   if (lL <= lSel){
   lLcount = lLcount + 1;
    if (lLcount == 3){l1 = true;}
       }}

if (l2 == false){
  if (lH <= lSel){
  lHcount = lHcount + 1;
    if (lHcount == 3){l2 = true;}
   }}

if (l3 == false){
  if (lW <= lSel){
  lWcount = lWcount + 1;
    if (lWcount == 3){l3 = true;}
   }}


  if (lL > lSel){
    if (lLcount > 0){
  lLcount = lLcount - 1;
    if (lLcount == 0){l1 = false;}
   }}
  if (lH > lSel){
    if (lHcount > 0){
  lHcount = lHcount - 1;
    if (lHcount == 0){l2 = false;}
  }}
  if (lW > lSel){
    if (lWcount > 0){
  lWcount = lWcount - 1;
    if (lWcount == 0){l3 = false;}
  }}
   
    
 //   if (lL <= lSel && lH <= lSel){
    if (l1 && l2){  
      digitalWrite(pumppin, HIGH);
      pumpstat = true;
      pump_Name = "ON";
  SoketData ("pump_Name", pump_Name, jsonRead(configJson,"pump_Name"));
  jsonWrite(configJson, "pump_Name", pump_Name);
  Serial.println(pump_Name);
      }
    if (!l1 && !l2){
      if (pumpstat == true){
        digitalWrite(pumppin,LOW);
        pumpstat = false;
        pump_Name = "OFF";
        SoketData ("pump_Name", pump_Name, jsonRead(configJson,"pump_Name"));
        jsonWrite(configJson, "pump_Name", pump_Name);
        Serial.println(pump_Name);
     }
    }}, nullptr, true);

}

void pump(){
  pumpstat = !pumpstat;
  //r1_Name = HTTP.arg(r1); // Получаем значение ssdp из запроса сохраняем в глобальной переменной
  //saveConfig();                 // Функция сохранения данных во Flash пока пустая
  //HTTP.send(200, "text/plain", "OK"); // отправляем ответ о выполнении
  Serial.println(pumpstat);
  if (pumpstat == true){
  digitalWrite(pumppin, HIGH);
  pump_Name = "ON";
  SoketData ("pump_Name", pump_Name, jsonRead(configJson,"pump_Name"));
  jsonWrite(configJson, "pump_Name", pump_Name);
  Serial.println(pump_Name);
  //servo.write(s1);
  //saveConfig();                 // Функция сохранения данных во Flash пока пустая
  HTTP.send(200, "text/plain", "OK");} // отправляем ответ о выполнении
  else{
  digitalWrite(pumppin, LOW);
  pump_Name = "OFF";
  SoketData ("pump_Name", pump_Name, jsonRead(configJson,"pump_Name"));
  jsonWrite(configJson, "pump_Name", pump_Name);
  Serial.println(pump_Name);
  //servo.write(0);
  //saveConfig();                 // Функция сохранения данных во Flash пока пустая
  HTTP.send(200, "text/plain", "OK"); // отправляем ответ о выполнении
  }}

// -----------------  Вывод времени и даты в /config.live.json каждую секунду
void sec_init() {
  ts.add(1, 30000, [&](void*) { // Запустим задачу 1 с интервалом 1000ms
    // поместим данные для web страницы в json строку configJson
    // Будем вызывать эту функцию каждый раз при запросе /config.live.json
    // jsonWrite(строка, "ключ", значение_число); Так можно дабавить или обнавить json значение ключа в строке
    // jsonWrite(строка, "ключ", "значение_текст");
    String timeS = GetTime();
    String date = GetDate();
    SoketData ("time", timeS, jsonRead(configJson,"time"));
    SoketData ("date", date, jsonRead(configJson,"date"));
    jsonWrite(configJson, "time", timeS); // отправить время в configJson
    jsonWrite(configJson, "date", date); // отправить дату в configJson
  }, nullptr, true);
}
/* ---------------- Задание для закрепления материала
    Заставьте мигать светодиод на любом pin с частотой 5 секунд
    сделайте новую задачу под индексом 2
    в /config.live.json отправляйте состояние светодиода с ключем "stateLed"
    Выводите состояние светодиода на график по запросу вида /charts.json?data=stateLed
    Процедуру blink_init() инициализируйте в setup
*/

void blink_init() {
  // здесь пишите код решения
}
