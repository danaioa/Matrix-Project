#include "highscore.h"
#include <string.h>


Highscore::Highscore() {
  initializare();
}

void Highscore::initializare() {

  if (EEPROM.read(EepromHighscore) != validare) {
    reseteaza();
    return;
  }

  int pozitie = EepromHighscore + 1;

  for (int i = 0; i < maxim; i++) {

    for (int j = 0; j < 4; j++) {
      clasament[i].nume[j] = EEPROM.read(pozitie++);
    }

    byte mic  = EEPROM.read(pozitie++);
    byte mare = EEPROM.read(pozitie++);

    clasament[i].scor = mic + mare * 256;
  }
}



void Highscore::salveazaEEPROM() {

  EEPROM.write(EepromHighscore, validare);

  int pozitie = EepromHighscore + 1;

  for (int i = 0; i < maxim; i++) {

    for (int j = 0; j < 4; j++) {
      EEPROM.write(pozitie++, clasament[i].nume[j]);
    }

    byte mic  = clasament[i].scor % 256;
    byte mare = clasament[i].scor / 256;

    EEPROM.write(pozitie++, mic);
    EEPROM.write(pozitie++, mare);
  }
}




void Highscore::reseteaza() {

  for (int i = 0; i < maxim; i++) {
    strcpy(clasament[i].nume, "---");
    clasament[i].scor = 0;
  }

  salveazaEEPROM();
}



void Highscore::adaugaScor(const char* nume, int scor) {

  for (int i = 0; i < maxim; i++) {

    if (scor > clasament[i].scor) {

      for (int j = maxim - 1; j > i; j--) {
        clasament[j] = clasament[j - 1];
      }

      strncpy(clasament[i].nume, nume, 3);
      clasament[i].nume[3] = '\0';
      clasament[i].scor = scor;
      salveazaEEPROM();
      return;
    }
  }
}



HighscoreEntry Highscore::get(int index) {

  if (index < 0 || index >= maxim) {
    HighscoreEntry invalid = { "---", 0 };
    return invalid;
  }

  return clasament[index];
}

const char* Highscore::getNume(int index) {

  if (index < 0 || index >= maxim) {
    return "---";
  }

  return clasament[index].nume;
}

int Highscore::getScor(int index) {

  if (index < 0 || index >= maxim) {
    return 0;
  }

  return clasament[index].scor;
}

int Highscore::count() {
  return maxim;
}
