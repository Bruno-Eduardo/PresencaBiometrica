/*
  SD card read/write

  This example shows how to read and write data to and from an SD card file
  The circuit:
   SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4 (for MKRZero SD: SDCARD_SS_PIN)

  created   Nov 2010
  by David A. Mellis
  modified 9 Apr 2012
  by Tom Igoe

  This example code is in the public domain.

*/

#include <SPI.h>
#include <SD.h>

File myFile;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


  Serial.print("Initializing SD card...");

  if (!SD.begin(10)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");

}

void loop() {
  int quantStrings;
  String vazio = "99991111----2222----3333----444";
  String textos[] = {"----1111----2222----3333----444", // Linha 1
                     "Esperando Professor            ", // Linha 2
                     "Digital invalida               ", // Linha 3
                     "Esperando digital              ", // Linha 4
                     "Problema ao ler digital        ", // Linha 5
                     "Presenca ja cadastrada         "/*, // Linha 6
                     "Problemas com Leitor SD        ", // Linha 7
                     "Limpando dados!!    !!!!!!!!!!!", // Linha 8
                     "Novo cadastro                  ", // Linha 9
                     "Problemas ao cadastrar         ", // Linha 10
                     "Nao cadastrado                 ", // Linha 11
                     "Dados transmitidos             ", // Linha 12
                     "Inicio do dia                  ", // Linha 13
                     "Fim do dia                     ", // Linha 14
                     "Problemas no leitor biometrico ", // Linha 15
                     "Coloque o dedo                 ", // Linha 16
                     "Retire o dedo                  ", // Linha 17
                     "Coloque o dedo novamente       ", // Linha 18
                     "Concluido                      "  // Linha 19
                    */};

  quantStrings = sizeof(textos) / 6;
  myFile = SD.open("Strings.txt", FILE_WRITE);
  if (myFile) {
    for (int i = 0; i < 19; i++) {
      myFile.print(char(int(i+1)));
      Serial.print("Digite a frase referente ao indice ");
      Serial.println(i+1);
      for(int j=0; j<31; j++){
        while(Serial.available() == 0){;}
        vazio[j] = Serial.read();
      }
      Serial.print("Li a String ->");
      Serial.print(vazio);
      Serial.println("<-");
      myFile.println(vazio);
    }
    myFile.close();
    Serial.println("done.");
    delay(500);
    Serial.println("Arquivo fechado");
  }
  while (true) {
    ;
  }
}







