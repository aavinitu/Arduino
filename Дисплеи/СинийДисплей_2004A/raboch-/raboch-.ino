



/*
 * Displays text sent over the serial port (e.g. from the Serial Monitor) on
 * an attached LCD.
 * YWROBOT
 *Compatible with the Arduino IDE 1.0
 *Library version:1.1
 */
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

void setup()
{
 lcd.init();                      // initialize the lcd 
//  lcd.begin();                      // initialize the lcd 
  lcd.backlight();
  Serial.begin(9600);

     lcd.clear(); 
}

void loop()
{

   lcd.setCursor(0,0);
   lcd.print("Serial Monitor123456");
      lcd.setCursor(0,1);
   lcd.print("Serial Monitor123456");
         lcd.setCursor(0,2);
   lcd.print("Serial Monitor");
         lcd.setCursor(0,3);
   lcd.print("Serial Monitor123456");

//   blink();
 //     lcd.write("ggg");
    
}
