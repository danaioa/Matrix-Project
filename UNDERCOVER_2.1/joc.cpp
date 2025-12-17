#include "joc.h"
#include "ultrasonic.h"
#include <Arduino.h>

void Joc::initializare(MatriceAfisaj* m, Sunet* s, Highscore* h) {
  matrice = m;
  sunet = s;
  hs = h;


  pinMode(PinLedPortal, OUTPUT);
  digitalWrite(PinLedPortal, LOW);


  ultimBlinkPortal = 0;
  stareLedPortal = false;
}



void Joc::actualizeazaVizibilitate() {
  const int razaVizibilitate = 2;

  for (int dy = -razaVizibilitate; dy <= razaVizibilitate; dy++) {
    for (int dx = -razaVizibilitate; dx <= razaVizibilitate; dx++) {

      int x = jucator.x + dx;
      int y = jucator.y + dy;

      if (x >= 0 && x < dimensiuneHarta && y >= 0 && y < dimensiuneHarta) {
        descoperit[y][x] = true;
      }
    }
  }
}


void Joc::startNivel(int nivel) {

  nivelCurent = constrain(nivel, nivelMin, nivelMax);
  jocActiv = true;

  jucator.x = startX;
  jucator.y = startY;

  cameraOx = 0;
  cameraOy = 0;

  nrInamici = min(inamiciMax, inamiciBaza + nivelCurent * inamiciPeNivel);

  intervalMiscareInamici = intervalInamiciStart - (nivelCurent - 1) * intervalInamiciScadere;

  intervalMiscareInamici = max(intervalMiscareInamici, intervalInamiciMin);

  if (nivelCurent == 1)
    nrItemi = itemiNivel1;

  else if (nivelCurent == 2)
    nrItemi = itemiNivel2;

  else nrItemi = itemiNivel3;

  vieti = vietiStart;
  if (nivelCurent == 1) {
    scor = scorStart;
  }

  genereazaHarta(nivelCurent);
  initializareItemiSiInamici();

  for (int y = 0; y < dimensiuneHarta; y++) {
    for (int x = 0; x < dimensiuneHarta; x++) {
      descoperit[y][x] = false;
    }
  }

  actualizeazaVizibilitate();

  modRadar = false;
  ceataActiv = true;
  ultimTimpMiscareInamici = millis();
  ultimPas = millis();

  digitalWrite(PinLedPortal, LOW);
  stareLedPortal = false;
}


void Joc::deseneazaPortal(int cx, int cy) {

  if (matrice->coordValide16(cx, cy - 1)) matrice->setPixelLogica16(cx, cy - 1, true);
  if (matrice->coordValide16(cx - 1, cy)) matrice->setPixelLogica16(cx - 1, cy, true);
  if (matrice->coordValide16(cx + 1, cy)) matrice->setPixelLogica16(cx + 1, cy, true);
  if (matrice->coordValide16(cx, cy + 1)) matrice->setPixelLogica16(cx, cy + 1, true);
}


void Joc::genereazaHarta(int nivel) {

  for (int y = 0; y < dimensiuneHarta; y++)
    for (int x = 0; x < dimensiuneHarta; x++)
      harta[y][x] = 0;

  for (int camX = 0; camX < 2; camX++) {
    for (int camY = 0; camY < 2; camY++) {

      int ox = camX * 8;
      int oy = camY * 8;

      for (int x = ox + 1; x < ox + 7; x += 3) {
        for (int y = oy + 1; y < oy + 7; y++)
          harta[y][x] = 1;
      }

      for (int y = oy + 2; y < oy + 7; y += 3) {
        for (int x = ox + 1; x < ox + 7; x++)
          harta[y][x] = 1;
      }
    }
  }

  harta[7][7] = 0;
  harta[7][8] = 0;

  harta[8][7] = 0;
  harta[8][8] = 0;


  harta[2][2] = 2;
  harta[13][2] = 2;
  harta[1][2] = 2;

  harta[7][8] = 3;


  harta[1][1] = 0;
  harta[15][15] = 0;

  harta[5][10] = 0;
  harta[13][11] = 0;
}


bool Joc::ePozitieValida(int x, int y) {
  if (x < 0 || x >= dimensiuneHarta || y < 0 || y >= dimensiuneHarta) {
    return false;
  }

  bool nuEstePerete = (harta[y][x] != 1);

  return nuEstePerete;
}



void Joc::initializareItemiSiInamici() {
  int plasati = 0;

  while (plasati < nrItemi) {

    int x = random(0, dimensiuneHarta);
    int y = random(0, dimensiuneHarta);

    if (harta[y][x] != 0) continue;
    if (x == jucator.x && y == jucator.y) continue;
    if (x == 15 && y == 15) continue;

    bool ok = true;
    for (int i = 0; i < plasati; i++) {
      if (itemi[i].x == x && itemi[i].y == y) {
        ok = false;
        break;
      }
    }

    if (!ok) continue;

    itemi[plasati].x = x;
    itemi[plasati].y = y;
    itemColectat[plasati] = false;
    plasati++;
  }

  for (int i = 0; i < nrInamici; i++) {
    bool asignat = false;

    for (int incercare = 0; incercare < nrMaxincercari && !asignat; incercare++) {
      int x = random(0, dimensiuneHarta);
      int y = random(0, dimensiuneHarta);

      if (!ePozitieValida(x, y)) continue;
      if (abs(x - jucator.x) + abs(y - jucator.y) < 5) continue;
      if (x == portalX && y == portalY) continue;

      bool ocupat = false;
      for (int j = 0; j < i; j++) {
        if (inamici[j].x == x && inamici[j].y == y) {
          ocupat = true;
          break;
        }
      }

      if (ocupat) continue;

      inamici[i].x = x;
      inamici[i].y = y;
      asignat = true;
    }

    if (!asignat) {
      inamici[i].x = dimensiuneHarta - 1 - i;
      inamici[i].y = dimensiuneHarta - 1;
    }
  }
}



void Joc::mutaInamici() {

  unsigned long timpCurent = millis();
  if (timpCurent - ultimTimpMiscareInamici < intervalMiscareInamici) {
    return;
  }

  ultimTimpMiscareInamici = timpCurent;

  for (int i = 0; i < nrInamici; i++) {
    if (inamici[i].x < 0) continue;

    int dx = 0;
    int dy = 0;

    int mod = random(0, 100);

    if (mod < 60) {
      if (inamici[i].x < jucator.x)
        dx = 1;
      else if (inamici[i].x > jucator.x)
        dx = -1;

      if (inamici[i].y < jucator.y)
        dy = 1;
      else if (inamici[i].y > jucator.y)
        dy = -1;

      if (random(0, 2) == 0)
        dy = 0;

      else dx = 0;
    } else {
      int r = random(0, 4);
      if (r == 0)
        dx = 1;
      else if (r == 1)
        dx = -1;
      else if (r == 2)
        dy = 1;
      else dy = -1;
    }

    int nx = inamici[i].x + dx;
    int ny = inamici[i].y + dy;

    if (nx == portalX && ny == portalY) continue;

    if (!ePozitieValida(nx, ny)) continue;

    bool liber = true;
    for (int j = 0; j < nrInamici; j++) {
      if (j != i && inamici[j].x == nx && inamici[j].y == ny) {
        liber = false;
        break;
      }
    }

    if (!liber) continue;

    inamici[i].x = nx;
    inamici[i].y = ny;
  }
}


void Joc::verificaRadarSiFog() {

  unsigned long acum = millis();

  if (scor <= 10) {
    modRadar = false;
    ultimTimpPenalizareRadar = acum;
    return;
  }

  long distanta = citesteDistantaCM();


  if (distanta > 0 && distanta < distantaRadarMax) {
    modRadar = true;
    timpRadar = acum;
  }

  if (modRadar && acum - timpRadar > durataRadar) {
    modRadar = false;
  }

  if (modRadar) {

    if (acum - ultimTimpPenalizareRadar >= 400) {

      scor -= nivelCurent;

      if (scor < 10) {
        modRadar = false;
      }

      ultimTimpPenalizareRadar = acum;
    }

  } else {
    ultimTimpPenalizareRadar = acum;
  }
}


bool trebuieAfisatItem(const unsigned long timpCurent, int nivelCurent, Pozitie pozJucator, Pozitie pozItem, bool esteColectat) {
  if (esteColectat) {
    return false;
  }

  int distanta = abs(pozJucator.x - pozItem.x) + abs(pozJucator.y - pozItem.y);

  const int distantaMaxima = 16;
  distanta = constrain(distanta, 0, distantaMaxima);

  int proximitate = distantaMaxima - distanta;

  int perioadaClipire;
  if (nivelCurent == 1) {
    perioadaClipire = map(proximitate, 0, distantaMaxima, 600, 150);
  } else {
    perioadaClipire = map(proximitate, 0, distantaMaxima, 800, 200);
  }

  perioadaClipire = max(perioadaClipire, 120);

  unsigned long fazaCurenta = timpCurent % perioadaClipire;


  unsigned long timpAprins = perioadaClipire * 7 / 10;

  return fazaCurenta < timpAprins;
}






void Joc::deseneazaScena() {

  matrice->clearLogica();

  bool afiseazaTot = (!ceataActiv) || modRadar;
  unsigned long t = millis();

  for (int y = 0; y < 16; y++) {
    for (int x = 0; x < 16; x++) {

      if (afiseazaTot || descoperit[y][x]) {
        if (harta[y][x] == 1) {
          matrice->setPixelLogica16(x, y, true);
        }
      }
    }
  }


  for (int i = 0; i < nrItemi; i++) {

    if (itemColectat[i]) continue;

    int x = itemi[i].x;
    int y = itemi[i].y;

    bool vizibil = afiseazaTot || descoperit[y][x];
    if (!vizibil) continue;

    bool aprinde = trebuieAfisatItem(t, nivelCurent, jucator, itemi[i], false);

    if (aprinde) {
      matrice->setPixelLogica16(x, y, true);
    }
  }

  for (int i = 0; i < nrInamici; i++) {

    if (inamici[i].x < 0) continue;

    int x = inamici[i].x;
    int y = inamici[i].y;

    if (afiseazaTot || descoperit[y][x]) {
      matrice->setPixelLogica16(x, y, true);
    }
  }

  bool toateItemeleColectate = true;

  for (int i = 0; i < nrItemi; i++) {
    if (!itemColectat[i]) {
      toateItemeleColectate = false;
      break;
    }
  }


  if (toateItemeleColectate && stareLedPortal) {

    const int px = 15;
    const int py = 15;

    if (afiseazaTot || descoperit[py][px]) {
      deseneazaPortal(px, py);
    }
  }


  matrice->setCursorPozitie(jucator.x, jucator.y);

  cameraOx = constrain(jucator.x - 4, 0, 8);
  cameraOy = constrain(jucator.y - 4, 0, 8);

  matrice->seteazaFereastra(cameraOx, cameraOy);
}



bool Joc::toateItemeleSuntColectate() {

  for (int i = 0; i < nrItemi; i++) {
    if (!itemColectat[i])
      return false;
  }
  return true;
}


void Joc::updateLedPortal(unsigned long t, bool toateItemele) {

  if (!toateItemele) {
    digitalWrite(PinLedPortal, LOW);
    stareLedPortal = false;
    return;
  }

  if (t - ultimBlinkPortal >= timpMin) {
    ultimBlinkPortal = t;
    stareLedPortal = !stareLedPortal;
    digitalWrite(PinLedPortal, stareLedPortal ? HIGH : LOW);
  }
}



void Joc::updateScut() {
  scutActiv = false;

  long distanta = citesteDistantaCM();
  if (distanta > 0 && distanta < 12) {
    scutActiv = true;
  }
}



bool Joc::proceseazaLaser(bool butonApasat, unsigned long timpCurent) {

  static bool butonAnterior = false;
  static unsigned long timpApasare = 0;

  // detectare apasare
  if (butonApasat && !butonAnterior) {
    timpApasare = timpCurent;
  }

  // detectare click scurt
  if (!butonApasat && butonAnterior) {
    if (timpCurent - timpApasare < timpMin) {

      int directieX = dirX;
      int directieY = dirY;

      if (directieX == 0 && directieY == 0) {
        directieX = 1;
        directieY = 0;
      }

      const int maxLaser = 8;
      int laserX[maxLaser];
      int laserY[maxLaser];
      int lungimeLaser = 0;


      for (int pas = 1; pas <= maxLaser; pas++) {

        int lx = jucator.x + directieX * pas;
        int ly = jucator.y + directieY * pas;

        if (lx < 0 || lx >= dimensiuneHarta || ly < 0 || ly >= dimensiuneHarta)
          break;

        if (harta[ly][lx] == 1)
          break;

        laserX[lungimeLaser] = lx;
        laserY[lungimeLaser] = ly;
        lungimeLaser++;

        // lovire inamic
        for (int i = 0; i < nrInamici; i++) {
          if (inamici[i].x == lx && inamici[i].y == ly) {
            inamici[i].x = -1;
            inamici[i].y = -1;
            scor += 20;
            sunet->sunetLovit();
            goto desen;
          }
        }
      }

desen:
      deseneazaScena();

      for (int i = 0; i < lungimeLaser; i++) {
        matrice->setPixelLogica16(laserX[i], laserY[i], true);
      }

      matrice->afiseazaMatriceLogica();
      butonAnterior = butonApasat;
      return true;
    }
  }

  butonAnterior = butonApasat;
  return false;
}



void Joc::mutaJucator(int joystickX, int joystickY, unsigned long timpCurent) {

  static unsigned long ultimulTimpPas = 0;

  if (timpCurent - ultimulTimpPas < timp)
    return;

  ultimulTimpPas = timpCurent;


  int directieX = 0;
  int directieY = 0;

  if (joystickX < PragSus)
    directieX = -1;

  else if (joystickX > PragJos)
    directieX = 1;



  if (joystickY < PragSus)
    directieY = -1;

  else if (joystickY > PragJos)
    directieY = 1;


  if (directieX == 0 && directieY == 0)
    return;


  dirX = directieX;
  dirY = directieY;


  int pozitieNouaX = jucator.x + directieX;
  int pozitieNouaY = jucator.y + directieY;



  if (ePozitieValida(pozitieNouaX, pozitieNouaY)) {

    jucator.x = pozitieNouaX;
    jucator.y = pozitieNouaY;

    actualizeazaVizibilitate();
    sunet->sunetPas();

  } else
    sunet->sunetColiziune();
}



bool Joc::verificaInamici() {


  for (int i = 0; i < nrInamici; i++) {

    if (inamici[i].x < 0)
      continue;

    bool coliziune = (inamici[i].x == jucator.x) && (inamici[i].y == jucator.y);

    if (!coliziune)
      continue;

    sunet->sunetLovit();

    if (scutActiv)
      continue;


    vieti--;

    if (jucator.x < 8)
      jucator.x = 1;
    else
      jucator.x = 8;

    if (jucator.y < 8)
      jucator.y = 1;
    else
      jucator.y = 8;


    actualizeazaVizibilitate();



    if (vieti <= 0) {
      jocActiv = false;
      digitalWrite(PinLedPortal, LOW);
      return true;
    }
  }

  return false;
}





void Joc::loopJoc(int joyX, int joyY, bool butonApasat) {


  if (!jocActiv) {
    digitalWrite(PinLedPortal, LOW);
    return;
  }

  unsigned long timpCurent = millis();

  bool toateItemeleColectate = toateItemeleSuntColectate();

  updateLedPortal(timpCurent, toateItemeleColectate);
  updateScut();

  if (proceseazaLaser(butonApasat, timpCurent))
    return;

  mutaJucator(joyX, joyY, timpCurent);

  for (int i = 0; i < nrItemi; i++) {

    if (itemColectat[i])
      continue;

    if (itemi[i].x == jucator.x && itemi[i].y == jucator.y) {
      itemColectat[i] = true;
      scor += 10;
      sunet->sunetItem();
    }
  }

  mutaInamici();

  if (verificaInamici())
    return;

  if (toateItemeleColectate && jucator.x == 15 && jucator.y == 15) {

    sunet->sunetNivelTrecut();

    if (nivelCurent < 3)
      startNivel(nivelCurent + 1);
    else
      jocActiv = false;

    digitalWrite(PinLedPortal, LOW);
    return;
  }

  verificaRadarSiFog();

  deseneazaScena();
  matrice->afiseazaMatriceLogica();
}







bool Joc::esteActiv() {
  return jocActiv;
}


int Joc::getItemeColectate() {
  int count = 0;
  for (int i = 0; i < nrItemi; i++) {
    if (itemColectat[i]) count++;
  }
  return count;
}

int Joc::getItemeTotal() {
  return nrItemi;
}
int Joc::getNivel() {
  return nivelCurent;
}
int Joc::getScor() {
  return scor;
}
int Joc::getVieti() {
  return vieti;
}
bool Joc::esteGameOver() {
  return gameOver;
}

bool Joc::aCastigat() const {
  return castigat;
}