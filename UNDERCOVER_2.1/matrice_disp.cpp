#include "matrice_disp.h"

void MatriceAfisaj::init() {

  lc.shutdown(0, false);
  lc.setIntensity(0, 1);
  lc.clearDisplay(0);
  clear();
  clearLogica();

  cursorX = -1;
  cursorY = -1;
}

void MatriceAfisaj::clear() {

  for (int y = 0; y < 8; y++) {
    for (int x = 0; x < 8; x++) {
      buffer8[y][x] = false;
    }
  }

}


void MatriceAfisaj::clearLogica() {

  for (int y = 0; y < 16; y++) {
    for (int x = 0; x < 16; x++) {
      buffer16[y][x] = false;
    }
  }

}


void MatriceAfisaj::setPixelGlobal(int x, int y, bool aprins) {

  if (x < 0 || x > 7 || y < 0 || y > 7) {
    return;
  }

  buffer8[y][x] = aprins;
}

void MatriceAfisaj::setPixelLogica16(int x, int y, bool aprins) {

  if (x < 0 || x > 15 || y < 0 || y > 15) {
    return;
  }
  buffer16[y][x] = aprins;
}


void MatriceAfisaj::seteazaFereastra(int ox, int oy) {

  fereastraOx = constrain(ox, 0, 8);
  fereastraOy = constrain(oy, 0, 8);
}


void MatriceAfisaj::setCursorPozitie(int x, int y) {

  cursorX = x;
  cursorY = y;
}

void MatriceAfisaj::actualizeazaBlink() {

  unsigned long timpCurent = millis();


  if (timpCurent - ultimBlink > 300) {
    ultimBlink = timpCurent;
    fazaBlink = !fazaBlink;
  }
}


void MatriceAfisaj::deseneazaMatrice() {
  clear();

  for (int y = 0; y < 8; y++) {
    for (int x = 0; x < 8; x++) {
      buffer8[y][x] = buffer16[fereastraOy + y][fereastraOx + x];
    }
  }

  if (cursorX >= 0 && cursorY >= 0) {
    if (cursorX >= fereastraOx && cursorX < fereastraOx + 8 &&
        cursorY >= fereastraOy && cursorY < fereastraOy + 8) {

      int cx = cursorX - fereastraOx;
      int cy = cursorY - fereastraOy;

      buffer8[cy][cx] = true;
    }
  }


  for (int y = 0; y < 8; y++) {
    byte linia = 0;
    for (int x = 0; x < 8; x++)
      if (buffer8[y][x]) linia |= (1 << x);

    lc.setRow(0, y, linia);
  }
}


bool MatriceAfisaj::coordValide16(int x, int y) {
  return (x >= 0 && x < 16 && y >= 0 && y < 16);
}

void MatriceAfisaj::stergeMatriceLogica16() {
  clearLogica();
}

void MatriceAfisaj::afiseazaMatriceLogica() {
  deseneazaMatrice();
}






void MatriceAfisaj::afiseazaIconita16(const byte iconita[8]) {
  clearLogica();

  for (int y = 0; y < 8; y++) {
    for (int x = 0; x < 8; x++) {

      bool aprins = (iconita[y] & (0b10000000 >> x));

      buffer16[y][x] = aprins;
    }
  }

  deseneazaMatrice();
}




void MatriceAfisaj::setLuminozitate(byte nivel) {
  lc.setIntensity(0, constrain(nivel, 0, 15));
}



