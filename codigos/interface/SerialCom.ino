#include <LiquidCrystal.h>

LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

void setup(){
  Serial.begin(9600);
  lcd.begin(16, 2);
  pinMode(13,OUTPUT);
}

void loop(){
  String texto;
  while(Serial.available() == 0){digitalWrite(13,LOW);}
  delay(100);
  digitalWrite(13,HIGH);
  int letras = Serial.available();
  for(int i=0; i<letras;i++)
    texto = texto + char(Serial.read());
  report(texto);
  delay(10000);
}

void quebra(String message, String linhas[]) {
  int i;
  for(i=16;i>=0;i--){
    if(message[i] == ' ')
      break;
  }
  linhas[0] = (message.substring(0,i));
  linhas[1] = (message.substring(i+1));
}

void report(String message){
  String linhas[2];
  quebra(message,linhas);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(linhas[0]);
  lcd.setCursor(0,1);
  lcd.print(linhas[1]);
}
