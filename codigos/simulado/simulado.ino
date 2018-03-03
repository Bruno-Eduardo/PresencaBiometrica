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
#define PROFESSOR "165215"                         //RA do professor

//Global Var
String dataHj;

/****************************************************************************/

void setup() {
  String leitura;
  inicializarPerifericos();
  do{
    leitura = digitalLida();
    if(leitura != PROFESSOR)
      report("Esperando professor");
    delay(100);
  }while(leitura != PROFESSOR);
  iniciarDia();
}

void loop() {
  String leitura = digitalLida();
  if(leitura == PROFESSOR){
    encerrarDia();
  }
  else{
   gravarPresenca(leitura, dataHj);
  }

}

//Funções auxiliares
String digitalLida(){
  String RA= "000000";
  while(Serial.available() <= 0){
    delay(100);
  }
  int caracteres = Serial.available();
  for(int i=0;i<caracteres;i++){
    RA[i] = Serial.read();
  }
  return RA;
}

void gravarPresenca(String leitura, String data){
  //Insere o RA e a data da presenca no arquivo aberto
  Serial.print("Mensagem gravada no cartao: ");
  Serial.print(leitura);
  Serial.print(" ");
  Serial.println(data);
}

void iniciarDia(){
  dataHj = "hoje eh 03";
  report("Inicio do programa");
}

void encerrarDia(){
  report("Fim do dia");
  delay(1000);
  asm volatile ("  jmp 0");
}

void report(String message){
  Serial.println(message);
}

void inicializarPerifericos(){
  String dataHj = "inicio";
  Serial.begin(9600);
  
}
