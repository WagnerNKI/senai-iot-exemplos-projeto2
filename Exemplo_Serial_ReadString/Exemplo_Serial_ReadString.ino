#include <LiquidCrystal.h>

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
String msg;

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.print("Mensagem foi:");
}



void loop() {

  if (Serial.available() > 0){
      msg = Serial.readString();
      Serial.print("Mensagem recebida: ");
      Serial.print(msg);
      Serial.println();
      lcd.setCursor(0,1);
      lcd.print("                   ");
      lcd.setCursor(0,1);
      lcd.print(msg);
      delay(1000);
  }
    
  }

