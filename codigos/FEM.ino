// Programa para teste de funcionamento do Leitor Biometrico
// Traduzido e adaptado por Usinainfo

#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
#define PROFESSOR 1

typedef struct presenca{
  char RA[6];
  int data;
} Presenca;

Presenca sala[100];
unsigned short int dia  // dia = 0: esperando digital do professor
                        // dia = 1: lendo digitais

int getFingerprintIDez();

SoftwareSerial mySerial(2, 3);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

void setup()  {  
  Serial.begin(9600);
  Serial.println("Iniciando Leitor Biometrico");
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);

  finger.begin(57600);
  
  if (finger.verifyPassword()) {
    Serial.println("Leitor Biometrico Encontrado");
  } else {
    Serial.println("Leitor Biometrico nao encontrada");
    while (1);
  }
  Serial.println("Esperando Dedo para Verificar");
}

void loop()                   
{
  do{
    int entrada = leitura();
    if(entrada == PROFESSOR && dia == 0)
      dia = 1;
    else if(dia == 1){
      if(entrada == PROFESSOR)
        encerrarDia();
      else{
        
      }
    }
  }while()
}



int leitura(){
  int entrada = getFingerprintIDez();
  digitalWrite(12, HIGH);
  delay(50);
  return entrada;
}

int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;
  
  digitalWrite(12, LOW);
  digitalWrite(11, HIGH);
  delay(1000);
  digitalWrite(11, LOW);
  delay(1000);
  digitalWrite(12, HIGH);
  Serial.print("ID # Encontrado"); 
  Serial.print(finger.fingerID); 
  Serial.print(" com precisao de "); 
  Serial.println(finger.confidence);
  return finger.fingerID; 
}
