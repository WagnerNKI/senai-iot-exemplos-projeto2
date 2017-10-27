// Inclusão da biblioteca do sensor
// Essa biblioteca não vem por padrão com o arduino e 
// precisa ser instalada pelo gerenciador de bibliotecas
#include <Ultrasonic.h>
// include the library code:
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Inicializa o objeto do sensor ultrasônico
// Usando as portas 12 e 13 para trigger e echo
Ultrasonic ultrasonic(7, 6);
int ledRed = 13;
int ledYellow =9;


void setup() {
  // Inicializa a porta Serial
  Serial.begin(9600);
  pinMode ( ledYellow , OUTPUT);
  pinMode ( ledRed , OUTPUT);
    // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("Distance in CM:");
}

void loop() {
  // Lê o valor do sensor
  int distancia = ultrasonic.distanceRead();
  String msg;
  
  // Escreve o valor da distância no painel Serial
  Serial.print("Distance in CM: ");
  Serial.println(distancia);

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
    
  if (distancia <= 5){
    digitalWrite(ledRed,HIGH);
    digitalWrite(ledYellow,LOW);
    lcd.setCursor(5, 1);
    lcd.print("           ");
    lcd.setCursor(5, 1);
    lcd.print("ooh deu!!");    
  }
  else if(distancia > 5 && distancia <= 30){
    digitalWrite(ledYellow,HIGH);
    digitalWrite(ledRed,LOW);
    lcd.setCursor(5, 1);
    lcd.print("           ");
    lcd.setCursor(5, 1);
    lcd.print("Devagar!!");
  }
 else{
    digitalWrite(ledRed,LOW);
    digitalWrite(ledYellow,LOW);
    lcd.setCursor(5, 1);
    lcd.print("           ");
    lcd.setCursor(5, 1);
    lcd.print("Pode vir!!");
 }
   // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  lcd.print("     ");
  lcd.setCursor(0, 1);
  lcd.print(distancia);
  delay(500);
}
