//Работа с ИК датчиком
//import the library in the sketch
#include <SharpIR.h>

//Create a new instance of the library
//Call the sensor "sensor"
//The model of the sensor is "GP2YA41SK0F"
//The sensor output pin is attached to the pin A0
SharpIR sensor(GP2Y0A21YK0F, A0);

//конец определений для ИК датчика

#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <WiFiUdp.h>

#define PIN_TRIG 12
#define PIN_ECHO 11
long duration, cm;


//#include <ESP8266WiFi.h>
#include "ESP8266.h"
#include <SoftwareSerial.h>
#include <math.h>
 
#define SSID     "AV_YOTA"
#define PASSWORD "puhvini35127"
#define TEMP_PIN A0

SoftwareSerial mySerial(4, 5);
ESP8266 wifi(mySerial);
String name = "sber_sc";
 

void setup() {



  // Инициализируем взаимодействие по последовательному порту
 
  Serial.begin (9600);
  //Определяем вводы и выводы
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);

    if (wifi.joinAP(SSID, PASSWORD)) {
    Serial.println("https://dweetpro.io/follow/" + name);
  } else {
    Serial.println("Wi-Fi connection error");
  }
}
 
void loop() {
//Работа с ИК датчиком
  int distance = sensor.getDistance(); //Calculate the distance in centimeters and store the value in a variable
  
  Serial.println(distance); //Print the value to the serial monitor 
 //Конец с ИК датчиком
 
  // Сначала генерируем короткий импульс длительностью 2-5 микросекунд.
 
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(5);
  digitalWrite(PIN_TRIG, HIGH);
 
  // Выставив высокий уровень сигнала, ждем около 10 микросекунд. В этот момент датчик будет посылать сигналы с частотой 40 КГц.
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);
 
  //  Время задержки акустического сигнала на эхолокаторе.
  duration = pulseIn(PIN_ECHO, HIGH);
 
  // Теперь осталось преобразовать время в расстояние
  cm = (duration / 2) / 29.1;
 
  Serial.print("Расстояние до объекта: ");
  Serial.print(cm);
  Serial.println(" см.");
 
  // Задержка между измерениями для корректной работы скеча
  delay(5000);
    String data = "GET /dweet/for/" + name + "?";
    data += "DISTANCE=" + String(cm) + " HTTP/1.1\r\n";
    data += "Host: dweetpro.io\r\n\r\n";
 Serial.println(data);

  //   float v_temp = 1023.0 / analogRead(TEMP_PIN) - 1.0;
   // float temp = 1.0 / ( -log(v_temp) / 3977.0
    //                          + 1.0 / 295.0 ) - 273.0;
  if (wifi.createTCP("www.dweetpro.io", 80)) {
    String data = "GET /dweet/for/" + name + "?";
    data += "DISTANCE=" + String(cm) + " HTTP/1.1\r\n";
    data += "Host: dweetpro.io\r\n\r\n";

 Serial.println(data);
 
    wifi.send(data.c_str(), data.length());
    wifi.releaseTCP();
  } else {
    Serial.println("create TCP error");
  }
  //delay(200);
}
