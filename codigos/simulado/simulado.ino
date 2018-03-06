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
  *    Implementar struct{int ID,StringRA}
  *    Criar vetor de RAs arbitrarios
  *    Criar funcao de um professor para posterior evolucao (mais de um professor)
  *    Criar uma nova enroll q salve num txt a lista de IDs e RAs
  *    Criar funcao q importe os IDs e RAs dum txt
  *    Converter todas as strings de RA (7 bytes) por um long(4 bytes)
  *      long val = 165215;
  *      Serial.println(String(165215));
  *     
 */
#define PROFESSOR "Labaki"                         //RA do professor

typedef struct aluno{
  int ID;
  String RA;
} Aluno;


Aluno sala[] = {
                {0, "Labaki"},
                {1, "165215"},
                {2, "165216"},
                {3, "165217"},
                };
//Global Var
String dataHj;
String ultimoCadastrado;
/****************************************************************************/

void setup() {
  String leitura;
  inicializarPerifericos();
  
  do{
    leitura = digitalLida();
    if(!ehProfessor(leitura))
      report("Esperando professor");
    delay(100);
  }while(!ehProfessor(leitura));
  iniciarDia();
}

void loop() {
  String leitura = digitalLida();
  if(valido(leitura)){  
    if(ehProfessor(leitura)){
      encerrarDia();
    }
    else{
     bool ok = gravarPresenca(leitura, dataHj);
     if(ok)
       report("Aluno " + leitura + " presente!");
    }
  }
  else{
    report("Problema ao ler digital");
  }
}

//Funções auxiliares
String digitalLida(){
  int IDlido;
  
  while(Serial.available() <= 0){
    delay(100);
  }

  IDlido = int(Serial.read()) - '0'; //---------------------------------------FLAG LEITORBIOMETRICO-----------
  
  while(Serial.available() > 0){
    char flush = Serial.read();
    delay(10);
  }
  if(IDlido >= (sizeof( sala ) / sizeof( sala[0] )) && IDlido < 0)
    return "-1";
  return sala[IDlido].RA;
}

bool gravarPresenca(String leitura, String data){
  //Insere o RA e a data da presenca no arquivo aberto
  if(jaCadastrado(leitura)){
    report("Presenca ja cadastrada");
    return false;
  }
  else{
    Serial.print("Mensagem gravada no cartao: "); //---------------------------------------FLAG SD CARD-----------
    Serial.print(leitura);
    Serial.print(" ");
    Serial.println(data);
    return true;
  }
}

void iniciarDia(){
  dataHj = word(analogRead(A0)); //---------------------------------------FLAG RTC-----------
  report("Inicio do programa");
}

void encerrarDia(){
  report("Fim do dia");
  delay(1000);
  asm volatile ("  jmp 0");
}

void report(String message){
  String linhas[2];
  quebra(message,linhas);
  Serial.println(linhas[0]); //---------------------------------------FLAG LCD-----------
  Serial.println(linhas[1]);
}

void inicializarPerifericos(){ //---------------------------------------FLAG all setups-----------
  String dataHj = "inicio";
  Serial.begin(9600);
}

bool jaCadastrado(String aluno){
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

bool ehProfessor(String candidato){
  if(candidato == PROFESSOR)
    return true;
  return false;
}

bool valido(String RA){
  if(RA[0] == '-')
    return false;
  return true;  
}
