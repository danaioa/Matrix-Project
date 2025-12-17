#pragma once
#include <Arduino.h>
#include "matrice_disp.h"
#include "sunet.h"
#include "highscore.h"
#include "setari.h"

extern const byte PinLedVerde;




struct Pozitie {
  int x;
  int y;
};

class Joc {
public:
  void initializare(MatriceAfisaj* m, Sunet* s, Highscore* h);
  void startNivel(int nivel);
  void loopJoc(int joyX, int joyY, bool butonApasat);
  byte calculeazaIntensitateLumina(byte luminozitateDeBaza);
  bool esteActiv();
  bool aCastigat() const;
  void startJocNou();

private:
  MatriceAfisaj* matrice = nullptr;
  Sunet* sunet = nullptr;
  Highscore* hs = nullptr;
  bool castigat = false;


  int nivelCurent = 1;
  bool jocActiv = false;
  Pozitie jucator;
  int cameraOx = 0;
  int cameraOy = 0;
  Pozitie itemi[8];
  bool itemColectat[8];
  int nrItemi = 0;
  int dimensiuneHarta = 16;
  Pozitie inamici[6];
  int nrInamici = 0;
  unsigned long ultimTimpMiscareInamici = 0;
  unsigned long intervalMiscareInamici = 800;

  byte harta[16][16];

  int portalX = 15;
  int portalY = 15;

  int nrMaxincercari = 200;
  static const int nivelMin = 1;
  static const int nivelMax = 3;

  static const int startX = 1;
  static const int startY = 1;

  static const int vietiStart = 3;
  static const int scorStart = 0;
  static const int distantaRadarMax = 15;

  static const unsigned long durataRadar = 1000;
  static const int inamiciMax = 10;
  static const int inamiciBaza = 2;
  static const int inamiciPeNivel = 2;

  static const unsigned long intervalInamiciStart = 800;
  static const unsigned long intervalInamiciScadere = 200;
  static const unsigned long intervalInamiciMin = 200;

  static const int itemiNivel1 = 6;
  static const int itemiNivel2 = 8;
  static const int itemiNivel3 = 8;
  bool ceataActiv = true;
  bool modRadar = false;
  unsigned long timpRadar = 0;
  bool descoperit[16][16];

  static const int timpMin = 500;
  static const int PragSus = 400;
 static const int PragJos = 600;

  static const int timp = 120;



  int scor = 0;
  int vieti = 3;
  unsigned long ultimPas = 0;
  bool gameOver = false;
  bool scutActiv = false;
  int dirX = 1;
  int dirY = 0;
  unsigned long ultimTimpPenalizareRadar;




  const int PinLedPortal = A2;
  unsigned long ultimBlinkPortal = 0;
  bool stareLedPortal = false;
  void genereazaHarta(int nivel);
  bool ePozitieValida(int x, int y);
  void initializareItemiSiInamici();
  void mutaInamici();
  int distManhattan(Pozitie a, Pozitie b);
  void deseneazaScena();
  void verificaRadarSiFog();
  void actualizeazaVizibilitate();
  void actualizeazaLedProximitate();
  void deseneazaExplozieFrame(int cx, int cy, int radius, bool fade);
  void explode(int cx, int cy);
  void distrugeObiecteInRaza(int cx, int cy, int raza);
  void trageLaser(int dx, int dy);
  void afiseazaLevelComplete(int scor);
  bool gasesteCale(int sx, int sy, int tx, int ty, int& outNx, int& outNy);
  void deseneazaPortal(int cx, int cy);
  bool toateItemeleSuntColectate();
  void updateLedPortal(unsigned long t, bool toateItemele);
  void updateScut();
  bool proceseazaLaser(bool butonApasat, unsigned long t);
  void mutaJucator(int joyX, int joyY, unsigned long t);
  bool verificaInamici();

public:
  int getItemeColectate();
  int getItemeTotal();
  int getNivel();
  int getScor();
  int getVieti();
  bool esteGameOver();
};
