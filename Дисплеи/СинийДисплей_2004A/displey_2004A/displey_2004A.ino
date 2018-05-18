#include <FastIO.h>
#include <I2CIO.h>
#include <LCD.h>
#include <LiquidCrystal.h>
#include <LiquidCrystal_I2C.h>



#include <Wire.h>  
// библиотека для LCD I2C: 
// https://bitbucket.org/fmalpartida/new-liquidcrystal/downloads
#include <LiquidCrystal_I2C.h>
//  LCD адрес - 0x27 для 20 символьного 4 строкового дисплея
// назначение контактов на I2C LCD:
//                    addr, en,rw,rs,d4,d5,d6,d7,bl,blpol
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // bl - backlight, blpol - полярность подсветки
 
void setup()   
{
  Serial.begin(9600);  // 
 
  lcd.begin(20,4);   // инициализация lcd 20 символьного 4 строкового дисплея, подсветка включена
  delay(2000);
  lcd.noBacklight(); // выключить подсветку
  delay(2000);
  lcd.backlight();   // включить подсветку
  
  // нумерация позиции курсора для строки и символа начинается с 0  
  lcd.setCursor(0,0); // начало с символа 1 строка 1
  lcd.print("Char 1, Row 1");
    
}
 
void loop()  
{

}
