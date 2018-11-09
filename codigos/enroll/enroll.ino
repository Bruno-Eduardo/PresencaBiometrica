/***************************************************
  This is an example sketch for our optical Fingerprint sensor

  Designed specifically to work with the Adafruit BMP085 Breakout
  ----> http://www.adafruit.com/products/751

  These displays use TTL Serial to communicate, 2 pins are required to
  interface
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#include <Adafruit_Fingerprint.h>

// On Leonardo/Micro or others with hardware serial, use those! #0 is green wire, #1 is white
// uncomment this line:
// #define mySerial Serial1

// For UNO and others without hardware serial, we must use software serial...
// pin #2 is IN from sensor (GREEN wire)
// pin #3 is OUT from arduino  (WHITE wire)
// comment these two lines if using hardware serial
#include <SoftwareSerial.h>
SoftwareSerial mySerial(8, 9);

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

uint8_t ide;
bool sucesso = false;


void setup()
{
  Serial.begin(9600);
  while (!Serial);  // For Yun/Leo/Micro/Zero/...
  delay(100);
  Serial.println("\n\nAdafruit Fingerprint sensor enrollment");

  // set the data rate for the sensor serial port
  finger.begin(57600);

  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) {
      delay(1);
    }
  }
}

void loop()                     // run over and over again
{
  sucesso = false;
  Serial.println("Gravando no numero 1");
  while (sucesso == false)
    while (!  getFingerprintEnroll(1) );

  Serial.println("------------------------------------------ Tudo certo");

  while (true) {
    ;
  }
}

uint8_t getFingerprintEnroll(int id) {
  sucesso = false;
  int p = -1;

  report(getString(16),0);

  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
  }

  // OK success!

  p = finger.image2Tz(1);
  if (p != FINGERPRINT_OK) {
    return p;
  }



  report(getString(17),2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }

  p = -1;
  report(getString(18),0);
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
    report(getString(19),0);
    sucesso = true;
    delay(10);
  }  else {
    return p;
  }
}

void report(String message, int tempo) {
  Serial.println(message);
  delay(tempo);
}

String getString(int ID) {
  switch (ID) {
    case  16:
      return "Coloque o dedo";
      break;
    case  17:
      return "Retire o dedo";
      break;
    case  18:
      return "Coloque o dedo novamente";
      break;
    case  19:
      return "Concluido";
      break;
  }

}
