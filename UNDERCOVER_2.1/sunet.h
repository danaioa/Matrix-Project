#pragma once
#include <Arduino.h>

class Sunet {
public:
  void initalizare(int pinBuzzer);
  void sunetPas();
  void sunetItem();
  void sunetLovit();
  void sunetColiziune();
  void sunetNivelTrecut();
  void setSunetOn(bool on);
  bool esteSunetOn();
  void sunetScutOn();
  void sunetScutOff();
  void setEnabled(bool v);
  bool isEnabled();

private:
  int pin = A2;
 

  bool sunetOn = true;
};
