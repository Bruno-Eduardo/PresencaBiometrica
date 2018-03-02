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
  *
 */


#include <DS1307.h>               //RTC-lib
#include <LiquidCrystal.h>        //LCD-lib
#include <SdFat.h>                //SDCard-lib

#define PROFESSOR 1                         //RA do professor
#define NOME_DO_ARQUIVO presencaBruta.txt   //Nome do arquivo em que os dados serao salvo

//Global Var
String dataHj;
LiquidCrystal lcd(13, 12, 11, 10, 9, 8);
SdFat sdCard;
SdFile meuArquivo;
const int chipSelect = 4;

/****************************************************************************/

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
  return Serial.parseInt();
}

void gravarPresenca(int leitura, String data){
  //Insere o RA e a data da presenca no arquivo aberto
  meuArquivo.print(leitura);
  meuArquivo.print(" ");
  meuArquivo.println("data");
}

void iniciarDia(){
  dataHj = analogRead(A0));
}

void encerrarDia(){
  meuArquivo.close();
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
 
 //Starts SD card
 if(!sdCard.begin(chipSelect,SPI_HALF_SPEED))sdCard.initErrorHalt();
  // Abre o arquivo LER_POT.TXT
  if (!meuArquivo.open("NOME_DO_ARQUIVO", O_RDWR | O_CREAT | O_AT_END))
  {
    sdCard.errorHalt("Erro na abertura do arquivo de presenca bruta");
    asm volatile ("  jmp 0");
  }
  
  Serial.begin(9600);
  
}
