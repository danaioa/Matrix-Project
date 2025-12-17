#include <Arduino.h>
#include "lcd.h"


byte inimaPlina[8] = {
  0b00000,
  0b01010,
  0b11111,
  0b11111,
  0b11111,
  0b01110,
  0b00100,
  0b00000
};

byte inimaGoala[8] = {
  0b00000,
  0b01010,
  0b10101,
  0b10001,
  0b10001,
  0b01010,
  0b00100,
  0b00000
};

void LCD::initCustomChars() {
    lcd->createChar(0, inimaPlina);
    lcd->createChar(1, inimaGoala);
}


void LCD::init(LiquidCrystal* lcdPtr) {
    lcd = lcdPtr;
}


void LCD::actualizeaza(int iteme, int total, int vieti, int nivel, int scor) {
    lcd->setCursor(0, 0);
    lcd->print("I:");
    lcd->print(iteme);
    lcd->print("/");
    lcd->print(total);

    lcd->setCursor(13, 0); 

    for (int i = 0; i < 3; i++) {
        if (i < vieti) lcd->write(byte(0));  
        else lcd->write(byte(1));           
    }

    lcd->setCursor(0, 1);
    lcd->print("Niv:");
    lcd->print(nivel);
    lcd->print("  SC:");
    lcd->print(scor);
}



