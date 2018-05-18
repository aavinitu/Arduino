#define BUTTON_PIN  3
#define LED_PIN     13


#include <SFE_BMP180.h>
#include <Wire.h>


//#include <LiquidCrystal.h>
// DHT датчик***********************
#include "DHT.h"

#define DHTPIN A0     // what pin we're connected to

// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);
// DHT датчик***********************


// monitor 20x4***********************
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4); // set the LCD address to 0x27 for a 16 chars and 2 line display
// monitor 20x4***********************
// You will need to create an SFE_BMP180 object, here called "pressure":

SFE_BMP180 pressure;
//LiquidCrystal lcd(13, 12, 11, 10,  9,  8);

#define ALTITUDE 1655.0 // Altitude of SparkFun's HQ in Boulder, CO. in meters

#define BUTTON_PIN  3
#define LED_PIN     7

boolean buttonWasUp = true;  // была ли кнопка отпущена?
boolean ledEnabled = false;  // включен ли свет?
int counter = 0;
int interval = 10;
int period = 5000;
boolean  start = true;


void setup()
{
  // monitor 20x4***********************
  lcd.init();                      // initialize the lcd
  //  lcd.begin();                      // initialize the lcd
  lcd.backlight();
  Serial.begin(9600);

  lcd.clear();
  // monitor 20x4***********************
  Serial.begin(9600);
  Serial.println("REBOOT");

  // кнопка
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);


  // Initialize the DHT sensor
  dht.begin();

  // Initialize the sensor (it is important to get calibration values stored on the device).

  if (pressure.begin())
    Serial.println("BMP180 init success");
  else
  {
    // Oops, something went wrong, this is usually a connection problem,
    // see the comments at the top of this sketch for the proper connections.

    Serial.println("BMP180 init fail\n\n");
    while (1); // Pause forever.
  }
}


void loop()
{

  if (start == true)
  {
    loop_();
    start = false;
  }
  else
  {
    delay (10);
    counter = counter + interval;
    // Serial.println();
    // Serial.print("counter");
    //   Serial.print(counter);
    if (counter == period)
    {
      loop_();
      counter = 0;

    }

  }
  // определить момент «клика» несколько сложнее, чем факт того,
  // что кнопка сейчас просто нажата. Для определения клика мы
  // сначала понимаем, отпущена ли кнопка прямо сейчас...
  boolean buttonIsUp = digitalRead(BUTTON_PIN);

  // ...если «кнопка была отпущена и (&&) не отпущена сейчас»...
  if (buttonWasUp && !buttonIsUp) {
    // ...может это «клик», а может и ложный сигнал (дребезг),
    // возникающий в момент замыкания/размыкания пластин кнопки,
    // поэтому даём кнопке полностью «успокоиться»...

    delay(10);


    // ...и считываем сигнал снова
    buttonIsUp = digitalRead(BUTTON_PIN);
    if (!buttonIsUp) {  // если она всё ещё нажата...
      // ...это клик! Переворачиваем сигнал светодиода
      ledEnabled = !ledEnabled;
      digitalWrite(LED_PIN, ledEnabled);
      loop_();
    }
  }

  // запоминаем последнее состояние кнопки для новой итерации
  buttonWasUp = buttonIsUp;

}


void loop_()
{
  char status;
  double T, P, p0, a;

  // Loop here getting pressure readings every 10 seconds.

  // If you want sea-level-compensated pressure, as used in weather reports,
  // you will need to know the altitude at which your measurements are taken.
  // We're using a constant called ALTITUDE in this sketch:

  Serial.println();
  Serial.print("provided altitude: ");
  Serial.print(ALTITUDE, 0);
  Serial.print(" meters, ");
  Serial.print(ALTITUDE * 3.28084, 0);
  Serial.println(" feet");


  // If you want to measure altitude, and not pressure, you will instead need
  // to provide a known baseline pressure. This is shown at the end of the sketch.

  // You must first get a temperature measurement to perform a pressure reading.

  // Start a temperature measurement:
  // If request is successful, the number of ms to wait is returned.
  // If request is unsuccessful, 0 is returned.

  status = pressure.startTemperature();
  if (status != 0)
  {
    // Wait for the measurement to complete:
    delay(status);

    // Retrieve the completed temperature measurement:
    // Note that the measurement is stored in the variable T.
    // Function returns 1 if successful, 0 if failure.

    status = pressure.getTemperature(T);
    if (status != 0)
    {
      // Print out the measurement:
      Serial.print("temperature: ");
      Serial.print(T, 2);
      Serial.print(" deg C, ");
      Serial.print((9.0 / 5.0)*T + 32.0, 2);
      Serial.println(" deg F");

      if (ledEnabled == true)
      {
        // устанавливаем курсор, колонку 0, строку 0. На деле — это
        // левый квадрат 1-й строки, т.к. нумерация начинается с нуля
        lcd.clear();
        lcd.setCursor(0, 0);
        // печатаем температуру
        lcd.print("t:");
        lcd.print(T, 1);
        // следом печатаем единицы измерения
        lcd.print(" C, ");
        lcd.print((9.0 / 5.0)*T + 32.0, 1);
        lcd.print(" F");
      }
      // Start a pressure measurement:
      // The parameter is the oversampling setting, from 0 to 3 (highest res, longest wait).
      // If request is successful, the number of ms to wait is returned.
      // If request is unsuccessful, 0 is returned.

      status = pressure.startPressure(3);
      if (status != 0)
      {
        // Wait for the measurement to complete:
        delay(status);

        // Retrieve the completed pressure measurement:
        // Note that the measurement is stored in the variable P.
        // Note also that the function requires the previous temperature measurement (T).
        // (If temperature is stable, you can do one temperature measurement for a number of pressure measurements.)
        // Function returns 1 if successful, 0 if failure.

        status = pressure.getPressure(P, T);
        if (status != 0)
        {
          // Print out the measurement:
          Serial.print("absolute pressure: ");
          Serial.print(P / 1.333, 2);
          Serial.print(" мм рт ст, ");
          Serial.print(P, 2);
          Serial.print(" mb, ");
          Serial.print(P * 0.0295333727, 2);
          Serial.println(" inHg");

          if (ledEnabled == true)
          {
            // устанавливаем курсор, колонку 0, строку 1. На деле — это
            // левый квадрат 2-й строки, т.к. нумерация начинается с нуля
            lcd.setCursor(0, 1);
            // печатаем давление
            lcd.print("p:");
            lcd.print(P / 1.333, 1);
            // следом печатаем единицы измерения
            lcd.print(" mm rt st");
            //   lcd.print(P*0.0295333727, 1);
            //  lcd.print(" inHg");

            //Измеряем влажность и температуру с DHT и допечатываем на экране

            float hum_dht = dht.readHumidity();
            float temp_dht = dht.readTemperature();

            if (isnan(temp_dht) || isnan(hum_dht))
            {
              Serial.println("Failed to read from DHT");
            }
            else
            {
              Serial.print("Humidity: ");
              Serial.print(hum_dht);
              Serial.print(" %\t");
              Serial.print("Temperature: ");
              Serial.print(temp_dht);
              Serial.println(" *C");

              lcd.setCursor(0, 2);
              // печатаем влажность
              lcd.print("Humidity:");
              lcd.print(hum_dht, 1);
              // следом печатаем единицы измерения
              lcd.print(" %");

                            lcd.setCursor(0, 3);
              // печатаем температуру DHT
              lcd.print("Temp DHT:");
              lcd.print(temp_dht, 1);
              // следом печатаем единицы измерения
              lcd.print(" *C");
            }


          }
          // The pressure sensor returns abolute pressure, which varies with altitude.
          // To remove the effects of altitude, use the sealevel function and your current altitude.
          // This number is commonly used in weather reports.
          // Parameters: P = absolute pressure in mb, ALTITUDE = current altitude in m.
          // Result: p0 = sea-level compensated pressure in mb

          p0 = pressure.sealevel(P, ALTITUDE); // we're at 1655 meters (Boulder, CO)
          Serial.print("relative (sea-level) pressure: ");
          Serial.print(p0, 2);
          Serial.print(" mb, ");
          Serial.print(p0 * 0.0295333727, 2);
          Serial.println(" inHg");

          if (ledEnabled == false)
          {
            lcd.clear();
            lcd.setCursor(0, 0);
            // печатаем высоту над уровнем моря
            lcd.print("SeaL:");
            lcd.print(p0, 2);
            // следом печатаем единицы измерения
            lcd.print("mb,");
            lcd.print(p0 * 0.0295333727, 2);
            lcd.print(" inHg");
          }

          // On the other hand, if you want to determine your altitude from the pressure reading,
          // use the altitude function along with a baseline pressure (sea-level or other).
          // Parameters: P = absolute pressure in mb, p0 = baseline pressure in mb.
          // Result: a = altitude in m.

          a = pressure.altitude(P, p0);
          Serial.print("computed altitude: ");
          Serial.print(a, 0);
          Serial.print(" meters, ");
          Serial.print(a * 3.28084, 0);
          Serial.println(" feet");

          if (ledEnabled == false)
          {
            lcd.setCursor(0, 1);
            // печатаем высоту
            lcd.print("ALT:");
            lcd.print(a, 0);
            // следом печатаем единицы измерения
            lcd.print("m,");
            lcd.print(a * 3.28084, 0);
            lcd.print("feet");
          }

        }
        else Serial.println("error retrieving pressure measurement\n");
      }
      else Serial.println("error starting pressure measurement\n");
    }
    else Serial.println("error retrieving temperature measurement\n");
  }
  else Serial.println("error starting temperature measurement\n");

  //  delay(5000);  // Pause for 5 seconds.
}
