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
  *    Criar uma nova enroll q salve num txt a lista de IDs e RAs
  *    Criar funcao q importe os IDs e RAs dum txt
  *     
 */
#include <LiquidCrystal.h>
#include <DS1307.h>

#define RTC_ON false

typedef struct aluno{
  short int ID;
  long RA;
} Aluno;

long professores[] = {0};

Aluno sala[] = {
                {0,0},
                {1,165215},
                {2,165216},
                {3,165217},
                {4,165218},
                {5,165219},
                };

//Global Var
String dataHj;
long ultimoCadastrado;
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
DS1307 rtc(A4, A5);
/****************************************************************************/

void setup() {
  long leitura;
  
  inicializarPerifericos();
  delay(100);
  report("Esperando professor");
  
  do{
    leitura = digitalLida();
    if(!ehProfessor(leitura)){
      report("Digital invalida");
      delay(700);
      report("Esperando professor");  
    }
    delay(100);
  }while(!ehProfessor(leitura));
  iniciarDia();
}

void loop() {
  delay(1000);
  report("Esperando digital");
  
  long leitura = digitalLida();
  
  if(valido(leitura)){  
    if(ehProfessor(leitura)){
      encerrarDia();
    }
    else{
     bool ok = gravarPresenca(leitura, dataHj);
     if(ok){
       report("Aluno " + String(leitura) + " presente!");
     }
    }
  }
  else{
    report("Problema ao ler digital");
  }
}

//Funções auxiliares
long digitalLida(){
  int IDlido;

  while(Serial.available() <= 0){
    delay(100);
  }

  IDlido = int(Serial.read()) - '0'; //---------------------------------------FLAG LEITORBIOMETRICO-----------
  
  while(Serial.available() > 0){
    char flush = Serial.read();
    delay(10);
  }
  if(IDlido >= (sizeof( sala ) / sizeof( sala[0] )) || IDlido < 0)
    return -1;
  return sala[IDlido].RA;
}

bool gravarPresenca(long leitura, String data){
  //Insere o RA e a data da presenca no arquivo aberto
  if(jaCadastrado(leitura)){
    report("Presenca ja cadastrada");
    return false;
  }
  else{
    //Serial.print("Mensagem gravada no cartao: "); //---------------------------------------FLAG SD CARD-----------
    Serial.print(String(leitura));
    Serial.print(",");
    Serial.println(data + ".03.2018");
    return true;
  }
}

void iniciarDia(){
  dataHj = word(analogRead(A0))%31; //---------------------------------------FLAG RTC-----------
  if(RTC_ON)
    dataHj = rtc.getDateStr(FORMAT_SHORT);
  report("Inicio do programa");
  delay(1000);
  report("Esperando digital");
  Serial.println("Mensagem gravada no cartao: ");
}

void encerrarDia(){
  report("Fim do dia");
  Serial.end();
  delay(1000);
  asm volatile ("  jmp 0");
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

void inicializarPerifericos(){ //---------------------------------------FLAG all setups-----------
  String dataHj = "inicio";
  Serial.begin(9600);
  lcd.begin(16, 2);
  if(RTC_ON){
    rtc.halt(false);
    rtc.setSQWRate(SQW_RATE_1);
    rtc.enableSQW(true);  
  }
}

bool jaCadastrado(long aluno){
  if(ultimoCadastrado == aluno)
    return true;
  ultimoCadastrado = aluno;
  return false;
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

bool ehProfessor(long candidato){
  int tamanho = (sizeof( professores ) / sizeof( professores[0] ));
  for(int i=0; i<tamanho; i++){
    if(candidato == professores[i])
      return true;
    }
  return false;
}

bool valido(long RA){
  if(RA < 0)
    return false;
  return true;  
}
