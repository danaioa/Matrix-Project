#ifndef HIGHSCORE_H
#define HIGHSCORE_H

#include <Arduino.h>
#include <EEPROM.h>

struct HighscoreEntry {
    char nume[4];   
    int scor;
};

class Highscore {
private:
    static const int maxim = 5;
    HighscoreEntry clasament[maxim];

    const int EepromHighscore = 3;  
    const int validare = 0x42; 

public:
    Highscore();

    void initializare();
    void salveazaEEPROM();  
    void reseteaza();       

    void adaugaScor(const char* nume, int scor);

    const char* getNume(int index);
    int  getScor(int index);

    HighscoreEntry get(int index);
    int count();
};

#endif
