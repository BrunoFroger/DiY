#include <Wire.h>
#include <LiquidCrystal_I2C.h>
 
LiquidCrystal_I2C lcd(0x27,16,2);
//LiquidCrystal_I2C lcd(0x23,16,2);
//LiquidCrystal_I2C lcd(0x3F,16,2); // set the LCD address to 0x3F for a 16 chars and 2 line display
 
void setup()
{
  Serial.begin (115200);  
  delay(1000);
  lcd.init(); // initialize the lcd
  delay(1000);
  Serial.println();
  Serial.println("lcd initialise");
  // Print a message to the LCD.
  //lcd.backlight();
  
    lcd.setBacklight(HIGH); 
    lcd.clear();
  Serial.println("ecran eteint");
  delay(1000);
  lcd.setCursor(0,0);
  lcd.print("Hello world");
  Serial.println("1ere ligne ecrite");
  delay(1000);
  lcd.setCursor(0,1);
  lcd.print("I2C LCD");
  Serial.println("2eme ligne ecrite");
 
}
 
void loop()
{
}
