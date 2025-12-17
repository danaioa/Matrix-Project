#include "sunet.h"
#include <Arduino.h>


extern bool SunetActiv;

void Sunet::initalizare(int pinBuzzer) {
  pin = pinBuzzer;
  pinMode(pin, OUTPUT);
  noTone(pin);
}

void Sunet::sunetPas() {
  if (!SunetActiv) return;
  tone(pin, 1000, 30);
}

void Sunet::sunetItem() {
  if (!SunetActiv) return;
  tone(pin, 1500, 80);
}

void Sunet::sunetLovit() {
  if (!SunetActiv) return;
  tone(pin, 300, 200);
}

void Sunet::sunetColiziune() {
  if (!SunetActiv) return;
  tone(pin, 600, 80);
}

void Sunet::sunetNivelTrecut() {
  if (!SunetActiv) return;
  tone(pin, 2000, 200);
}

void Sunet::sunetScutOn() {
  if (!SunetActiv) return;
  tone(pin, 1200, 80);
}

void Sunet::sunetScutOff() {
  if (!SunetActiv) return;
  tone(pin, 400, 80);
}
