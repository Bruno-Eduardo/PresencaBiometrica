
#include <SPI.h>
#include <SD.h>

File myFile;

void setup() {
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
  for(int i=1;i<=10;i++){
    Serial.print("->");
    Serial.print(encontrarString(i));
    Serial.println("<-");
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}

String encontrarString(int ID) {
  String resposta = "                                ";

  myFile = SD.open("Strings.txt");
  if (myFile) {
    int i = 1;
    bool flag = true;

    while (flag and myFile.available()) {
      char indice = myFile.read();
      if (int(indice) == ID) {
        for (i = 0; i < 32; i++)
          resposta[i] = myFile.read();
        flag = false;
      } else {
        for (i = 0; i < 33; i++)
          myFile.read();
      }
    }

    myFile.close();
  }

  return resposta;

}
