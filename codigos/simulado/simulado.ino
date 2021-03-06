/*

   Desenvolvido por Bruno Oliveira - bruno97br@gmail.com
    Ultimo update - 29/05/2018

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


/*
    Gasto com variaveis globais:
      Original 75%
      apenas global var 53%
      apenas objetos 50%
      apenas bibliotecas 38%

      1578 -> 76%
      1528 -> 74%
      1488 -> 72%
      1460 -> 71%
      1454 -> 70%
*/


#include <LiquidCrystal.h>
#include <DS1307.h>
#include <SPI.h>
#include <SD.h>
#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>

#define RTC_ON true
#define DESLIGAR asm volatile ("  jmp 0")
#define ARQUIVODEENTRADAS "ENTRADAS.CSV"
#define ARQUIVODEALUNOS "ALUNOS.TXT"
#define TEMPOLIMITE 1000
#define VAZIO -2



long professores[] = {0};

//Global Var
String dataHj = "8.03.2018";
long ultimoCadastrado;
short int quantDeAlunos;
bool sucesso;
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
DS1307 rtc(A4, A5);
SoftwareSerial mySerial(8, 9);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
String lista[] = {ARQUIVODEALUNOS, ARQUIVODEENTRADAS};

//Lista de Strings
String problemSD = "Err1";

/****************************************************************************/

void setup() {
  long leitura;

  inicializarPerifericos();
  dataHj = rtc.getDateStr(FORMAT_SHORT);
  delay(100);

  do {
    report(getString(2), 0);
    leitura = digitalLida();
    if (!ehProfessor(leitura) && leitura != VAZIO) {
      report(getString(3), 700);
      report(getString(2), 0);
    }
    delay(100);
  } while (!ehProfessor(leitura));
  iniciarDia();
}

void loop() {
  delay(1000);
  report(getString(4), 0);

  // Se o programa do computador estiver comunicando execute suas instrucoes

  if (Serial.available() > 0) {
    char interrupChar = Serial.peek();
    if (isAlpha(interrupChar)) {
      interrupcao(interrupChar);
      delay(1000);
      report(getString(4), 0);
    }
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
      report(getString(5), 0);
    }
  }
}

//Funções auxiliares
long digitalLida() {
  int IDlido;

  int tempoini = millis();
  boolean flagNotOut = true;
  /*
    while ((millis() < tempoini + TEMPOLIMITE) and (flagNotOut)) {
      if (Serial.available() > 0)
        if (isAlpha(Serial.peek()) == false)
          flagNotOut = false;
        else {
          interrupcao(Serial.peek());
          delay(2000);
          report("",10);
        }
    }

    if (flagNotOut == true)
      return VAZIO;
  */
  if (Serial.available() > 0) {
    interrupcao(Serial.peek());
    delay(2000);
    return VAZIO;
  }

  IDlido = getFingerprintIDez(); //---------------------------------------FLAG LEITORBIOMETRICO-----------

  if (IDlido > quantDeAlunos)
    return -1;
  if (IDlido < 0)
    return IDlido;

  return ID2RA(IDlido);
}

long ID2RA(int IDlido) {
  long RA = 0;
  int li;
  int IDref;

  //Tenta ler a lista de alunos
  File arqID2RA = SD.open(ARQUIVODEALUNOS);

  // Se o arquivo estiver nao estiver legivel, retorne um RA Invalido (-1):
  if (arqID2RA) {
    while (arqID2RA.available()) {
      //Le o RA
      RA = 0;
      for (int i = 0; i < 6; i++) {
        li = arqID2RA.read() - '0';
        RA = RA * 10 +  li;
      }
      IDref = arqID2RA.read();

      // Se o ID for o certo retorne esse RA
      if (IDref == IDlido) {
        arqID2RA.close();
        return RA;
      }
    }
  }
  // Se o arquivo estiver nao estiver legivel ou o ID não existir, retorne um RA Invalido (-1):

  if (arqID2RA)
    arqID2RA.close();
  return -1;
}

bool gravarPresenca(long leitura, String data, bool forcado) {
  //Insere o RA e a data da presenca no arquivo aberto
  if (jaCadastrado(leitura) and not(forcado)) {
    report(getString(6), 0);
    return false;
  }
  else {
    // Salva a presenca no cartao
    File arqPresenca = SD.open(ARQUIVODEENTRADAS, FILE_WRITE);
    // Se conseguir abrir, salve
    if (arqPresenca) {
      arqPresenca.println(String(leitura) + "," + data);
      arqPresenca.close();
    }
    // Se nao conseguir abrir reporte
    else {
      report(getString(7), 0);
    }

    report("Aluno " + String(leitura) + " presente!", 0);
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
      report(getString(8), 1000);

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
  long aluno;
  
  report(getString(9), 1000);

  for (int i = 0; i < 6; i++)
    RAnovo[i] = Serial.read();

  if (enroll(RAnovo) == true) {
    report("RA " + RAnovo + " Cadastrado!", 0);
    Serial.write("1");
    delay(1000);
    if(RAnovo != "000000"){
      aluno = 0;
      for (int i = 0; i < 6; i++) {
        aluno = aluno * 10 + int(RAnovo[i] - '0');
      }
      gravarPresenca(aluno, dataHj, true);
      delay(1000);
    }
  }
  else {
    report(getString(10), 0);
    Serial.write("0");
    delay(1000);
  }

}

bool enroll(String RA) {
  bool flag = true;
  File arqEnroll = SD.open(ARQUIVODEALUNOS, FILE_WRITE);
  // Se conseguir abrir, salve
  if (arqEnroll) {

    sucesso = false;
    while (sucesso == false)
      while (!  getFingerprintEnroll(quantDeAlunos + 1) );

    quantDeAlunos++;
    arqEnroll.print(RA);
    arqEnroll.print(char(quantDeAlunos));
    arqEnroll.close();
    return true;
  }
  report(getString(11), 2000);
  return false;
}

void dumpFile(String arquivo) {
  File arqDump = SD.open(arquivo);

  // Se o arquivo estiver legivel, copie:
  if (arqDump) {
    while (arqDump.available()) {
      Serial.write(arqDump.read());
    }
    arqDump.close();
    report(getString(12), 0);
  }
  // Se nao conseguir ler, reporte o erro e desligue:
  else {
    report(getString(7), 0);
    delay(2000);
    DESLIGAR;
  }
}

bool delFiles(String lista[], int sizeList) {
  for (int i = 0; i < sizeList; i++) {
    if (SD.remove(lista[i]) == false)
      return false;
  }
  finger.emptyDatabase();
  delay(20);
  return true;
}

void iniciarDia() {
  dataHj = rtc.getDateStr(FORMAT_SHORT);
  report(getString(13), 1000);
}

void encerrarDia() {
  report(getString(14), 1000);
  Serial.end();
  DESLIGAR;
}

void report(String message, int delaytime) {
  String linhas[2];
  quebra(message, linhas);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(linhas[0]);
  lcd.setCursor(0, 1);
  lcd.print(linhas[1]);

  delay(delaytime);

}

void inicializarPerifericos() {
  String dataHj = "0";
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
    report(getString(7), 2000);
    DESLIGAR;
    return;
  }
  else {
    File arqInicia = SD.open(ARQUIVODEALUNOS);

    // Se o arquivo estiver legivel, veja quantos alunos tem:
    if (arqInicia) {
      while (arqInicia.available()) {
        quantDeAlunos = int(arqInicia.read());
      }
      arqInicia.close();
    }
    else {
      quantDeAlunos = 0;
    }
  }

  //Biometric Sensor setup
  finger.begin(57600);
  if (finger.verifyPassword() == false) {
    report(getString(15), 2000);
    DESLIGAR;
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


uint8_t getFingerprintEnroll(int id) {
  sucesso = false;
  int p = -1;

  report(getString(16), 0);

  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
  }

  // OK success!

  p = finger.image2Tz(1);
  if (p != FINGERPRINT_OK) {
    return p;
  }



  report(getString(17), 2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }

  p = -1;
  report(getString(18), 0);
  while (p != FINGERPRINT_OK)
    p = finger.getImage();

  // OK success!

  p = finger.image2Tz(2);

  if (p != FINGERPRINT_OK) {
    return p;
  }

  // OK converted!

  p = finger.createModel();
  if (p != FINGERPRINT_OK) {
    return p;
  }

  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    report(getString(19), 0);
    sucesso = true;
    delay(10);
  }  else {
    return p;
  }
}

String getString(int ID) {
  String resposta = "                                ";

  File arqgetString = SD.open("Strings.txt");
  if (arqgetString) {
    int i = 1;
    bool flag = true;

    while (flag and arqgetString.available()) {
      char indice = arqgetString.read();
      if (int(indice) == ID) {
        for (i = 0; i < 32; i++)
          resposta[i] = arqgetString.read();
        flag = false;
      } else {
        for (i = 0; i < 33; i++)
          arqgetString.read();
      }
    }

    arqgetString.close();
  }

  return resposta;

}

int getFingerprintIDez() {
  int tempoentrada = millis();

  uint8_t p = finger.getImage();
  while (p != FINGERPRINT_OK && millis()<(tempoentrada+5000) && (Serial.available() == 0))
    p = finger.getImage();

  if(millis()>(tempoentrada+5000) || (Serial.available() > 0))
    return -2;
    
  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;

  return finger.fingerID;
}
