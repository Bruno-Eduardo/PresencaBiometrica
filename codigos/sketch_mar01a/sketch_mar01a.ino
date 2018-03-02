/*
 *
 * Desenvolvido por Bruno Oliveira - bruno97br@gmail.com
 *  Ultimo update - 01/03/2017
 *
 * Infos:
 *      RTC:
 *          https://www.filipeflop.com/blog/relogio-rtc-ds1307-arduino/
 *      Biometrico:
 *          http://blog.usinainfo.com.br/leitor-biometrico-arduino-sistema-de-cadastramento-e-leitura-de-digitais/
 *      CartaoSD:
 *          https://www.filipeflop.com/blog/cartao-sd-com-arduino/
 *      LCD:
 *          Arquvos exemplos do Arduino IDE
 */


 /*  TODO:
  *  Importar bibliotecas (Biometrico/Cartao/LCD)
  *  Implementar digitalLida()/iniciarDia()/encerrarDia()/gravarPresenca()/horario()
  *  Implementar saidas de texto por LCD = report()
  *  Implementar entrada e comparação de datas para quando o professor inserir a digital duas vezes no mesmo dia
  *  digitalLida()
  *      pegar no pc da FEM
  *
 */


#include <DS1307.h> //RTC-lib
#include <LiquidCrystal.h>LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

#define PROFESSOR 1

//Global Var
String dataHj;
DS1307 rtc(A4, A5);
LiquidCrystal lcd(13, 12, 11, 10, 9, 8);

//int digitalLida(){return 1;}
//void gravarPresenca(int leitura, String data){}
//void iniciarDia(){}
//void encerrarDia(){}

void setup() {
  int leitura;
  do{
    leitura = digitalLida();
  }while(leitura != PROFESSOR);
  iniciarDia();
}

void loop() {
  int leitura = digitalLida();
  if(leitura == PROFESSOR){
    encerrarDia();
  }
  else{
   gravarPresenca(leitura, dataHj);
  }

}

//Funções auxiliares
int digitalLida(){
  return 1;
}
void gravarPresenca(int leitura, String data){
  ;
}
void iniciarDia(){
  dataHj = rtc.getDateStr(FORMAT_SHORT);
}
void encerrarDia(){
  asm volatile ("  jmp 0");
}
void report(String message){
  // Print a message to the LCD.
  lcd.print(message);
}
void inicializarPerifericos(){
  //Zerar data de inicio
  String dataHj = "inicio";

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  //Starts rtc
  rtc.halt(false);
  rtc.setSQWRate(SQW_RATE_1);
  rtc.enableSQW(true);
}
