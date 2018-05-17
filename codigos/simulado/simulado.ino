/*

   Desenvolvido por Bruno Oliveira - bruno97br@gmail.com
    Ultimo update - 12/04/2018

   Infos:
        RTC:
            https://www.filipeflop.com/blog/relogio-rtc-ds1307-arduino/
        Biometrico:
            http://blog.usinainfo.com.br/leitor-biometrico-arduino-sistema-de-cadastramento-e-leitura-de-digitais/
        CartaoSD:
            https://www.filipeflop.com/blog/cartao-sd-com-arduino/
        LCD:
            Arquvos exemplos do Arduino IDE
*/


/*  TODO:
      Criar uma nova enroll q salve num txt a lista de IDs e RAs
      Criar funcao q importe os IDs e RAs dum txt

*/
#include <LiquidCrystal.h>
#include <DS1307.h>
#include <SPI.h>
#include <SD.h>

#define RTC_ON false
#define DESLIGAR asm volatile ("  jmp 0")
#define ARQUIVODEENTRADAS "ENTRADAS.CSV"
#define ARQUIVODEALUNOS "ALUNOS.TXT"
#define TEMPOLIMITE 1000
#define VAZIO -2

long professores[] = {0};

//Global Var
String dataHj = "8.03.2018";
long ultimoCadastrado;
int quantDeAlunos;
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
DS1307 rtc(A4, A5);
File myFile;
String lista[] = {ARQUIVODEALUNOS, ARQUIVODEENTRADAS};
/****************************************************************************/

void setup() {
  long leitura;

  inicializarPerifericos();
  delay(100);

  do {
    report("Esperando professor");
    leitura = digitalLida();
    if (!ehProfessor(leitura) && leitura != VAZIO) {
      report("Digital invalida");
      delay(700);
      report("Esperando professor");
    }
    delay(100);
  } while (!ehProfessor(leitura));
  iniciarDia();
}

void loop() {
  delay(1000);
  report("Esperando digital");

  // Se o programa do computador estiver comunicando execute suas instrucoes

  while (Serial.available() == 0) {
    ;
  }

  char interrupChar = Serial.peek();
  if (isAlpha(interrupChar)) {
    interrupcao(interrupChar);
    delay(1000);
    report("Esperando digital");
  }

  // Se o programa nao estiver, continue com a rotina de ler digitais
  else {
    long leitura = digitalLida();

    if (valido(leitura)) {
      if (ehProfessor(leitura)) {
        encerrarDia();
      }
      else {
        gravarPresenca(leitura, dataHj, false);
        delay(1000);
      }
    }
    //Caso nao tenha encontrado um digital em TEMPOLIMITE(mili segundos), reinicie a rotina
    else if (leitura == VAZIO) {
      ;
    }
    else {
      report("Problema ao ler digital");
    }
  }
}

//Funções auxiliares
long digitalLida() {
  int IDlido;

  int tempoini = millis();
  boolean flagNotOut = true;

  while ((millis() < tempoini + TEMPOLIMITE) and (flagNotOut)) {
    if (Serial.available() > 0)
      if (isAlpha(Serial.peek()) == false)
        flagNotOut = false;
      else {
        interrupcao(Serial.peek());
      }
  }

  if (flagNotOut == true)
    return VAZIO;

  IDlido = int(Serial.read()) - '0'; //---------------------------------------FLAG LEITORBIOMETRICO-----------

  while (Serial.available() > 0) {
    char flush = Serial.read();
    delay(10);
  }
  if (IDlido > quantDeAlunos || IDlido < 0)
    return -1;

  return ID2RA(IDlido);
}

long ID2RA(int IDlido) {

  //Tenta ler a lista de alunos
  File dataFile = SD.open(ARQUIVODEALUNOS);
  long RA = 0;
  int li;
  int IDref;
  // Se o arquivo estiver nao estiver legivel, retorne um RA Invalido (-1):
  if (dataFile) {
    while (dataFile.available()) {
      //Le o RA
      RA = 0;
      for (int i = 0; i < 6; i++) {
        li = dataFile.read() - '0';
        RA = RA * 10 +  li;
      }
      IDref = dataFile.read();

      // Se o ID for o certo retorne esse RA
      if (IDref == IDlido) {
        dataFile.close();
        return RA;
      }
    }
  }
  // Se o arquivo estiver nao estiver legivel ou o ID não existir, retorne um RA Invalido (-1):

  dataFile.close();
  return -1;
}

bool gravarPresenca(long leitura, String data, bool forcado) {
  //Insere o RA e a data da presenca no arquivo aberto
  if (jaCadastrado(leitura) and not(forcado)) {
    report("Presenca ja cadastrada");
    return false;
  }
  else {
    // Salva a presenca no cartao
    myFile = SD.open(ARQUIVODEENTRADAS, FILE_WRITE);
    // Se conseguir abrir, salve
    if (myFile) {
      myFile.println(String(leitura) + "," + data);
      myFile.close();
    }
    // Se nao conseguir abrir reporte
    else {
      report("Erro no SD Card");
    }

    report("Aluno " + String(leitura) + " presente!");
  }
}

void interrupcao(char interrup) {
  // Lista de interrupcoes possiveis pelo  programa e seus significados
  // a : Cadastrar novo aluno
  // b : Enviar txt com a lista de presenca
  // c : Deletar todos os dados
  // d : Imprimir lista de alunos
  // e : Presenca manual
  // f : Nao implementado
  // g : Nao implementado
  // h : Nao implementado
  // i : Nao implementado
  // j : Nao implementado

  Serial.read(); // Le o caracter relativo a interrpcao

  switch (interrup) {
      bool limpou;
      long aluno;

    case 'a':
      cadastro();
      break;
    case 'b':
      dumpFile(ARQUIVODEENTRADAS);
      break;
    case 'c':
      report("Limpando todos dados!!!!!!");
      delay(1000);

      limpou = delFiles(lista, 2);

      quantDeAlunos = 0;

      if (limpou)
        Serial.write("1");
      else
        Serial.write("0");
      break;
    case 'd':
      dumpFile(ARQUIVODEALUNOS);
      break;
    case 'e':
      delay(30);
      while (Serial.peek() == -1) {
        Serial.read(); Serial.read();
      }
      aluno = 0;
      for (int i = 0; i < 6; i++) {
        aluno = aluno * 10 + int(Serial.read() - '0');
      }
      gravarPresenca(aluno, dataHj, true);
      delay(1000);
      break;
    case 'f':
      ;
      break;
    case 'g':
      ;
      break;
    case 'h':
      ;
      break;
    case 'i':
      ;
      break;
    case 'j':
      ;
      break;
  }
}

void cadastro() {
  String RAnovo = "000000";

  report("Novo Cadastro");

  delay(1000);

  for (int i = 0; i < 6; i++)
    RAnovo[i] = Serial.read();

  if (enroll(RAnovo) == true) {
    report("RA " + RAnovo + " Cadastrado!");
    Serial.write("1");
    delay(1000);
  }
  else {
    report("Problema ao cadastrar");
    Serial.write("0");
    delay(1000);
  }

}

bool enroll(String RA) {

  myFile = SD.open(ARQUIVODEALUNOS, FILE_WRITE);
  // Se conseguir abrir, salve
  if (myFile) {
    quantDeAlunos++;
    myFile.print(RA);
    myFile.print(char(quantDeAlunos));
    myFile.close();
    return true;
  }
  else
    return false;
}

void dumpFile(String arquivo) {
  File dataFile = SD.open(arquivo);

  // Se o arquivo estiver legivel, copie:
  if (dataFile) {
    while (dataFile.available()) {
      Serial.write(dataFile.read());
    }
    dataFile.close();
    report("Arquivo transmitido");
  }
  // Se nao conseguir ler, reporte o erro e desligue:
  else {
    report("ERRO NO CARTAO"); delay(2000);
    report("NENHUM DADO TRANSMITIDO"); delay(2000);
    DESLIGAR;
  }
}

bool delFiles(String lista[], int sizeList) {
  for (int i = 0; i < sizeList; i++) {
    if (SD.remove(lista[i]) == false)
      return false;
  }
  return true;
}

void iniciarDia() {
  int leitura = analogRead(A0) % 31;
  dataHj =  String(leitura) + ".03.2018"; //---------------------------------------FLAG RTC-----------
  if (RTC_ON)
    dataHj = rtc.getDateStr(FORMAT_SHORT);
  report("Inicio do programa");
}

void encerrarDia() {
  report("Fim do dia");
  Serial.end();
  delay(1000);
  DESLIGAR;
}

void report(String message) {
  String linhas[2];
  quebra(message, linhas);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(linhas[0]);
  lcd.setCursor(0, 1);
  lcd.print(linhas[1]);
}

void inicializarPerifericos() { //---------------------------------------FLAG all setups-----------
  String dataHj = "inicio";
  Serial.begin(9600);

  //LCD setup
  lcd.begin(16, 2);

  //RTC setup
  if (RTC_ON) {
    rtc.halt(false);
    rtc.setSQWRate(SQW_RATE_1);
    rtc.enableSQW(true);
  }

  //SD CARD setup
  if (!SD.begin(10)) {
    report("Problema com o cartao SD");
    delay(2000);
    DESLIGAR;
    return;
  }
  else {
    File dataFile = SD.open(ARQUIVODEALUNOS);

    // Se o arquivo estiver legivel, veja quantos alunos tem:
    if (dataFile) {
      while (dataFile.available()) {
        quantDeAlunos = int(dataFile.read());
      }
    }
    else {
      quantDeAlunos = 0;
    }
    dataFile.close();
  }
}

bool jaCadastrado(long aluno) {
  if (ultimoCadastrado == aluno)
    return true;
  ultimoCadastrado = aluno;
  return false;
}

void quebra(String message, String linhas[]) {
  int i;
  for (i = 16; i >= 0; i--) {
    if (message[i] == ' ')
      break;
  }
  linhas[0] = (message.substring(0, i));
  linhas[1] = (message.substring(i + 1));
}

bool ehProfessor(long candidato) {
  int tamanho = (sizeof( professores ) / sizeof( professores[0] ));
  for (int i = 0; i < tamanho; i++) {
    if (candidato == professores[i]) {
      return true;
    }
  }
  return false;
}

bool valido(long RA) {
  if (RA < 0)
    return false;
  return true;
}
