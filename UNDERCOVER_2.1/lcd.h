#ifndef LCD_H
#define LCD_H

#include <Arduino.h>
#include <LiquidCrystal.h>

class LCD {
public:
  void init(LiquidCrystal* lcdPtr);
  void initCustomChars();
  void scrollGameOver();

  void actualizeaza(
    int itemeColectate,
    int itemeTotal,
    int vieti,
    int nivel,
    int scor);

private:
  LiquidCrystal* lcd = nullptr;
};

#endif
