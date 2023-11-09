#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define pintriger 3  
#define pinecho 4

void setup() {
  Serial.begin (9600);
  lcd.init();
  pinMode(pintriger, OUTPUT);
  pinMode(pinecho, INPUT);
  lcd.backlight();
}

void loop() {
  long durasi, jarak;
  digitalWrite(pintriger, LOW); 
  delayMicroseconds(2);
  digitalWrite(pintriger, HIGH);
  delayMicroseconds(10);
  digitalWrite(pintriger, LOW);
  durasi = pulseIn(pinecho, HIGH);
  jarak = (durasi/2) / 29;
  
  if (jarak < 50)
  {
    lcd.setCursor(0,0);
    lcd.print("JARAK BENDA");
    lcd.setCursor(0,1);
    lcd.print("KURANG DARI 50cm");
  }
  else if (jarak < 90)
  {
    lcd.setCursor(0,0);
    lcd.print("JARAK BENDA");
    lcd.setCursor(0,1);
    lcd.print("KURANG DARI 90cm");
  }
  else {
    lcd.setCursor(0,0);
    lcd.print("JARAK BENDA");
    lcd.setCursor(0,1);
    lcd.print("LEBIH DARI 100cm");
    
  }
}
