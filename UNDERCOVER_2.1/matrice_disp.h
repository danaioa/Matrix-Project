#pragma once
#include <LedControl.h>
#include <Arduino.h>

class MatriceAfisaj {
public:
  void init();
  void clear();
  void setPixelGlobal(int x, int y, bool aprins);  
  void setCursorPozitie(int x, int y);           
  void deseneazaMatrice();                         
  void actualizeazaBlink();
  void setPixelLogica16(int x, int y, bool aprins);
  void seteazaFereastra(int ox, int oy);  
  void clearLogica();
  bool coordValide16(int x, int y);
  void stergeMatriceLogica16();
  void afiseazaMatriceLogica();
  void afiseazaIconita(const byte iconita[8]);
  void afiseazaIconita16(const byte iconita[8]);
  void umple(byte valoare);
  void goleste();
  void setLuminozitate(byte nivel);
  void afiseazaText(const char* txt);


private:
  const byte dinPin = 12;
  const byte clockPin = 11;
  const byte loadPin = 10;
  LedControl lc{ dinPin, clockPin, loadPin, 1 };
  bool buffer8[8][8];
  bool buffer16[16][16];
  int fereastraOx = 0;
  int fereastraOy = 0;
  int cursorX = 0, cursorY = 0;
  unsigned long ultimBlink = 0;
  bool fazaBlink = false;
};
