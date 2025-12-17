#include "ultrasonic.h"

const byte PinTrig = 13;
const byte PinEcho = A4;

void initUltrasonic() {
  pinMode(PinTrig, OUTPUT);
  pinMode(PinEcho, INPUT);
  digitalWrite(PinTrig, LOW);
}

long citesteDistantaCM() {
  digitalWrite(PinTrig, LOW);
  delayMicroseconds(2);
  digitalWrite(PinTrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(PinTrig, LOW);

  long durata = pulseIn(PinEcho, HIGH, 25000); 
  if (durata == 0) return -1; 
  long distanta = durata / 58; 
  return distanta;
}
