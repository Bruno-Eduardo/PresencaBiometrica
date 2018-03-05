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
  *  resolver problema de iteração num vetor de String
 */
#define PROFESSOR "165215"                         //RA do professor

//Global Var
String dataHj;
String listaDePresenca[100];
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
  if(caracteres > 6)
    caracteres = 6;
  for(int i=0;i<caracteres;i++){
    RA[i] = Serial.read();
  }
  while(Serial.available() > 0){
    char flush = Serial.read();
  }

  Serial.print("Acabei de ler ");
  Serial.println(RA);
  
  return RA;
}

void gravarPresenca(String leitura, String data){
  //Insere o RA e a data da presenca no arquivo aberto
  if(jaCadastrado(leitura)){
    report("Presenca ja cadastrada");
  }
  else{
    Serial.print("Mensagem gravada no cartao: ");
    Serial.print(leitura);
    Serial.print(" ");
    Serial.println(data);
    listaDePresenca[AlunosRegistrados] = leitura;
    AlunosRegistrados++;
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
  report("Impressao da lista de presenca----------");
  for(int i=0; i<AlunosRegistrados; i++){
    Serial.println(listaDePresenca[i]);
  }

  Serial.print("O aluno agora eh ");
  Serial.println(aluno);
  
  for(int i=0; i<AlunosRegistrados; i++){
    if(listaDePresenca[i][4] == aluno[4]);
      Serial.print(listaDePresenca[i][4]);
      Serial.print("O aluno lido ");
      Serial.print(aluno);
      Serial.print(" é igual o da lista");
      Serial.print(listaDePresenca[i]);
      report(aluno);
      return true;
  }
  return false;
}
