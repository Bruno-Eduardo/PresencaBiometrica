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
  *  Implementar quebra de linha do LCD
 */
#define PROFESSOR "165215"                         //RA do professor

//Global Var
String dataHj;
String listaDePresenca[100];
String ultimoCadastrado;
int AlunosRegistrados;
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
   bool ok = gravarPresenca(leitura, dataHj);
   if(ok)
     report("Aluno " + leitura + " presente!");
  }

}

//Funções auxiliares
String digitalLida(){
  String RA= "000000";
  while(Serial.available() <= 0){
    delay(100);
  }
  for(int i=0;i<6;i++){
    RA[i] = Serial.read();
    delay(10);
  }
  while(Serial.available() > 0){
    char flush = Serial.read();
    delay(10);
  }
  
  return RA;
}

bool gravarPresenca(String leitura, String data){
  //Insere o RA e a data da presenca no arquivo aberto
  if(jaCadastrado(leitura)){
    report("Presenca ja cadastrada");
    return false;
  }
  else{
    Serial.print("Mensagem gravada no cartao: ");
    Serial.print(leitura);
    Serial.print(" ");
    Serial.println(data);
    listaDePresenca[AlunosRegistrados] = leitura;
    AlunosRegistrados++;
    return true;
  }
}

void iniciarDia(){
  dataHj = word(analogRead(A0));
  report("Inicio do programa");
  AlunosRegistrados = 0;
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

bool jaCadastrado(String aluno){
  if(ultimoCadastrado == aluno)
    return true;
  ultimoCadastrado = aluno;
  return false;
}
