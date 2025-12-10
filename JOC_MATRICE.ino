#include <LiquidCrystal.h>
#include <EEPROM.h>
#include "LedControl.h"

constexpr byte PinLcdRs = 9;
constexpr byte PinLcdEn = 8;
constexpr byte PinLcdD4 = 7;
constexpr byte PinLcdD5 = 6;
constexpr byte PinLcdD6 = 5;
constexpr byte PinLcdD7 = 4;

constexpr byte PinLuminaFundal = 3;

constexpr byte PinJoyX = A0;
constexpr byte PinJoyY = A1;
constexpr byte PinButonJoy = 2;

constexpr int PragSus = 400;
constexpr int PragJos = 600;

const int PinBuzzer = A3;
const byte PinLedVerde = A5;
const byte PinLedRosu = A2;
constexpr byte PinTrig = 13;
constexpr byte PinEcho = A4;

unsigned long ultimaClipireVerde = 0;
bool stareLedVerde = false;

constexpr unsigned long TimpNavigareMeniu = 180;
constexpr unsigned long TimpApasareLunga = 800;

constexpr unsigned long ClipireLentMs = 500;
constexpr unsigned long ClipireRapidMs = 150;

LiquidCrystal lcd(PinLcdRs, PinLcdEn, PinLcdD4, PinLcdD5, PinLcdD6, PinLcdD7);

byte NivelLuminozitate = 2;
byte ValoriLuminozitate[5] = { 50, 100, 150, 200, 255 };

const byte DinPin = 12;
const byte ClockPin = 11;
const byte LoadPin = 10;

LedControl matrice(DinPin, ClockPin, LoadPin, 1);

int CursorX = 0;
int CursorY = 0;
unsigned long UltimaMiscareJoy = 0;

enum class StareAplicatie {
  MeniuPrincipal,
  MeniuLuminozitate,
  ModJoc
};


unsigned long timpAfisareGameOver = 0;
bool asteaptaGameOver = false;

StareAplicatie StareCurenta = StareAplicatie::MeniuPrincipal;

int IndexMeniuPrincipal = 0;
int NivelLuminozitateTemporar;

unsigned long UltimaNavigareMeniu = 0;

const int NivelLumin = 5;
const int NivLumImplicit = 2;

bool PrevButonDebounced = false;
unsigned long UltimSchimbareButon = 0;
const unsigned long DebounceButonMs = 50;
bool ApasatEveniment = false;


unsigned long ultimSunetMiscare = 0;
const unsigned long DelaySunetMiscare = 120;  


const byte LUNGIME_HARTA = 16;
const byte LATIME_VIEW = 8;
const byte INAMICI_MAX = 6;


byte hartaLogica[LUNGIME_HARTA][LUNGIME_HARTA];


bool fog[LUNGIME_HARTA][LUNGIME_HARTA];

int viewportX = 0;
int viewportY = 0;



unsigned long timpScanActiv = 0;
const unsigned long DURATA_SCAN_MS = 1000;
bool scanActiv = false;


const unsigned long TimpPasPlayerMs = 150;
const unsigned long TimpMiscareInamicMin = 400;
const unsigned long TimpMiscareInamicMax = 900;


bool jocPornit = false;
unsigned long timpNivelStart = 0;
int nivelCurent = 1;
const int FRAGMENTE_PENTRU_TRECERE = 3;

unsigned long ultimaAnimatieInima = 0;
bool stareInimaBlink = false;

struct Player {
  int x;
  int y;
  int vieti;
  int fragmenteColectate;
  unsigned long ultimPas;
  unsigned long cooldownDash;  
  bool invulnerabil;
  unsigned long timpInvulnerabil;
} jucator;


struct Inamic {
  bool activ;
  int x;
  int y;
  int directieX; 
  int directieY;
  unsigned long ultimPas;
  unsigned long intervalMiscare;
  bool urmareste;  
} inamici[INAMICI_MAX];



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

void AfiseazaVietiLCD() {
  lcd.setCursor(13, 0);
  for (int i = 0; i < 3; i++) {
    if (i < jucator.vieti) {
      if (jucator.vieti == 1 && stareInimaBlink) {
        lcd.write(byte(1));  
      } else {
        lcd.write(byte(0)); 
      }
    } else {
      lcd.print(" ");  
    }
  }
}


void InitializeazaJoc();
void GestioneazaClipireVerde();
void ActualizeazaMeniuPrincipal();
void ActualizeazaMeniuLuminozitate();
void CitesteButon(unsigned long t, bool &butonApasat, bool &apasareDetectata);
void GestioneazaMeniuPrincipal(int joyY, int joyX, bool apasatEveniment);
void GestioneazaMeniuLuminozitate(int joyY, int joyX, bool apasatEveniment);
void ActualizeazaScorLCD();

void setup() {

  lcd.begin(16, 2);
  lcd.createChar(0, inimaPlina);
  lcd.createChar(1, inimaGoala);
  pinMode(PinLuminaFundal, OUTPUT);
  pinMode(PinButonJoy, INPUT_PULLUP);

  pinMode(PinLedVerde, OUTPUT);

  matrice.shutdown(0, false);
  matrice.setIntensity(0, 1);
  matrice.clearDisplay(0);

  NivelLuminozitate = EEPROM.read(0);

  if (NivelLuminozitate > NivelLumin)
    NivelLuminozitate = NivLumImplicit;

  analogWrite(PinLuminaFundal, ValoriLuminozitate[NivelLuminozitate]);
  pinMode(PinBuzzer, OUTPUT);
  noTone(PinBuzzer);

  ActualizeazaMeniuPrincipal();
}
void InitializeazaJoc();
void GenereazaHartaInitiala();
void ResetFog();
void PlaseazaPlayerSiInamici();
void GestioneazaModJoc(int joyY, bool butonApasat);
void ActualizeazaInamici();
void ActualizeazaPlayer(int joyY, int joyX, bool butonApasat);
void DeseneazaMatrice();
void DeseneazaViewport();
void SeteazaLedPhysical(int xViz, int yViz, bool stare);  
void ActiveazaScanare();
int CitesteUltrasonicCm();
void SunetEvent(const char *tip);

void loop() {
  unsigned long timpCurent = millis();

  int ValoareJoyY = analogRead(PinJoyY);
  int ValoareJoyX = analogRead(PinJoyX);

  bool butonApasat;
  bool apasareDetectata;
  CitesteButon(timpCurent, butonApasat, apasareDetectata);

  switch (StareCurenta) {
    case StareAplicatie::MeniuPrincipal:
      GestioneazaMeniuPrincipal(ValoareJoyY, ValoareJoyX, apasareDetectata);
      break;

    case StareAplicatie::MeniuLuminozitate:
      GestioneazaMeniuLuminozitate(ValoareJoyY, ValoareJoyX, apasareDetectata);
      break;

    case StareAplicatie::ModJoc:
      GestioneazaModJoc(ValoareJoyY, butonApasat);
      GestioneazaClipireVerde();

      if (jucator.vieti == 1) {
        if (timpCurent - ultimaAnimatieInima > 400) {  
          ultimaAnimatieInima = timpCurent;
          stareInimaBlink = !stareInimaBlink;
          AfiseazaVietiLCD();
        }
      }
      break;
  }

  DeseneazaMatrice();
}

void CitesteButon(unsigned long t, bool &butonApasat, bool &apasareDetectata) {

  static bool stareStabila = false;
  static bool stareBruta = false;
  static unsigned long tSchimbare = 0;

  bool citireBruta = !digitalRead(PinButonJoy);
  apasareDetectata = false;

  if (citireBruta != stareBruta) {
    stareBruta = citireBruta;
    tSchimbare = t;
  }

  if (t - tSchimbare > DebounceButonMs) {

    if (stareStabila != stareBruta) {

      if (stareBruta == true && stareStabila == false) {
        apasareDetectata = true;
      }

      stareStabila = stareBruta;
    }
  }

  butonApasat = stareStabila;
}

void ActualizeazaMeniuPrincipal() {
  lcd.clear();

  lcd.setCursor(0, 0);

  if (IndexMeniuPrincipal == 0)
    lcd.print(">Start Joc");
  else
    lcd.print(" Start Joc");

  lcd.setCursor(0, 1);

  if (IndexMeniuPrincipal == 1)
    lcd.print(">Luminozitate: ");
  else
    lcd.print(" Luminozitate: ");

  lcd.print(NivelLuminozitate + 1);
}

void GestioneazaMeniuPrincipal(int joyY, int joyX, bool apasatEveniment) {

  unsigned long t = millis();

  if (t - UltimaNavigareMeniu > TimpNavigareMeniu) {

    if (joyY < PragSus && IndexMeniuPrincipal > 0) {

      IndexMeniuPrincipal--;
      ActualizeazaMeniuPrincipal();
    }

    if (joyY > PragJos && IndexMeniuPrincipal < 1) {

      IndexMeniuPrincipal++;
      ActualizeazaMeniuPrincipal();
    }

    UltimaNavigareMeniu = t;
  }

  bool joyCentral = (joyX > PragSus && joyX < PragJos && joyY > PragSus && joyY < PragJos);

  if (apasatEveniment && joyCentral) {

    if (IndexMeniuPrincipal == 0) {

      lcd.clear();
      lcd.print("Mod Joc");
      StareCurenta = StareAplicatie::ModJoc;
    }

    else {

      NivelLuminozitateTemporar = NivelLuminozitate;
      StareCurenta = StareAplicatie::MeniuLuminozitate;
      ActualizeazaMeniuLuminozitate();
    }
  }
}

void ActualizeazaMeniuLuminozitate() {

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Alege nivel:");
  lcd.setCursor(0, 1);
  lcd.print("Nivel: ");
  lcd.print(NivelLuminozitateTemporar + 1);
}

void GestioneazaMeniuLuminozitate(int joyY, int joyX, bool apasatEveniment) {

  unsigned long t = millis();

  if (t - UltimaNavigareMeniu > TimpNavigareMeniu) {

    if (joyY < PragSus && NivelLuminozitateTemporar > 0)
      NivelLuminozitateTemporar--;

    if (joyY > PragJos && NivelLuminozitateTemporar < 4)
      NivelLuminozitateTemporar++;

    analogWrite(PinLuminaFundal, ValoriLuminozitate[NivelLuminozitateTemporar]);
    ActualizeazaMeniuLuminozitate();

    UltimaNavigareMeniu = t;
  }

  bool joyCentral = (joyX > PragSus && joyX < PragJos && joyY > PragSus && joyY < PragJos);

  if (apasatEveniment && joyCentral) {

    NivelLuminozitate = NivelLuminozitateTemporar;
    EEPROM.update(0, NivelLuminozitate);

    StareCurenta = StareAplicatie::MeniuPrincipal;
    ActualizeazaMeniuPrincipal();
  }
}


void ActualizeazaScorLCD() {
  lcd.setCursor(0, 1);  
  lcd.print("Frag: ");
  lcd.print(jucator.fragmenteColectate);
  lcd.print("/");
  lcd.print(FRAGMENTE_PENTRU_TRECERE);
  lcd.print("    "); 
}




bool InDomeniu(int x, int y) {
  return x >= 0 && x < LUNGIME_HARTA && y >= 0 && y < LUNGIME_HARTA;
}


void InitializeazaJoc() {
  for (int i = 0; i < LUNGIME_HARTA; ++i)
    for (int j = 0; j < LUNGIME_HARTA; ++j) {
      hartaLogica[i][j] = 0;
      fog[i][j] = false;
    }

  GenereazaHartaInitiala();
  ResetFog();
  PlaseazaPlayerSiInamici();

  viewportX = constrain(jucator.x - LATIME_VIEW / 2, 0, LUNGIME_HARTA - LATIME_VIEW);
  viewportY = constrain(jucator.y - LATIME_VIEW / 2, 0, LUNGIME_HARTA - LATIME_VIEW);

  jocPornit = true;
  timpNivelStart = millis();
  nivelCurent = 1;
  AfiseazaVietiLCD();
}


int DistantaPanaLaDocumenteProximitate() {
  int distMinima = 10; 


  for (int dx = -4; dx <= 4; dx++) {
    for (int dy = -4; dy <= 4; dy++) {
      int hx = jucator.x + dx;
      int hy = jucator.y + dy;

      if (InDomeniu(hx, hy) && hartaLogica[hx][hy] == 2) {
        int dist = abs(dx) + abs(dy); 
        if (dist < distMinima) distMinima = dist;
      }
    }
  }
  return distMinima;  
}



void GestioneazaClipireVerde() {
  int d = DistantaPanaLaDocumenteProximitate();

  if (d >= 10) {
    digitalWrite(PinLedVerde, LOW);
    return;
  }


  unsigned long interval = 50 + (d * 55);

  if (millis() - ultimaClipireVerde >= interval) {
    ultimaClipireVerde = millis();
    stareLedVerde = !stareLedVerde;
    digitalWrite(PinLedVerde, stareLedVerde);
  }
}


void GenereazaHartaInitiala() {
  for (int i = 0; i < LUNGIME_HARTA; ++i)
    for (int j = 0; j < LUNGIME_HARTA; ++j)
      hartaLogica[i][j] = 0;

  for (int camX = 0; camX < 2; ++camX) {
    for (int camY = 0; camY < 2; ++camY) {
      int ox = camX * 8;
      int oy = camY * 8;
      for (int x = ox + 1; x < ox + 7; x += 3) {
        for (int y = oy + 1; y < oy + 7; ++y) hartaLogica[x][y] = 1;
      }
      for (int y = oy + 2; y < oy + 7; y += 3) {
        for (int x = ox + 1; x < ox + 7; ++x) hartaLogica[x][y] = 1;
      }
    }
  }

 
  hartaLogica[7][7] = 0;  
  hartaLogica[8][8] = 0;


  hartaLogica[2][2] = 2;
  hartaLogica[13][2] = 2;
  hartaLogica[1][2] = 2;

 
  hartaLogica[7][8] = 3; 
}


void ResetFog() {
  for (int i = 0; i < LUNGIME_HARTA; ++i)
    for (int j = 0; j < LUNGIME_HARTA; ++j)
      fog[i][j] = false;
}


void PlaseazaPlayerSiInamici() {
  jucator.x = 1;
  jucator.y = 1;
  jucator.vieti = 3;
  jucator.fragmenteColectate = 0;
  jucator.ultimPas = 0;
  jucator.cooldownDash = 0;
  jucator.invulnerabil = false;
  jucator.timpInvulnerabil = 0;

 
  for (int i = 0; i < INAMICI_MAX; ++i) {
    inamici[i].activ = false;
  }

  inamici[0].activ = true;
  inamici[0].x = 6;
  inamici[0].y = 2;
  inamici[0].intervalMiscare = 600;
  inamici[0].ultimPas = 0;
  inamici[1].activ = true;
  inamici[1].x = 12;
  inamici[1].y = 4;
  inamici[1].intervalMiscare = 700;
  inamici[1].ultimPas = 0;
  inamici[2].activ = true;
  inamici[2].x = 11;
  inamici[2].y = 13;
  inamici[2].intervalMiscare = 500;
  inamici[2].ultimPas = 0;

  for (int i = 0; i < INAMICI_MAX; ++i) {
    inamici[i].directieX = random(-1, 2);
    inamici[i].directieY = random(-1, 2);
    inamici[i].urmareste = false;
  }
}


void GestioneazaModJoc(int joyY, bool butonApasat) {
  int joyX = analogRead(PinJoyX);

  if (!jocPornit) InitializeazaJoc();

  unsigned long t = millis();


  int distCm = CitesteUltrasonicCm();
  if (distCm > 0 && distCm < 20 && !scanActiv) {
    ActiveazaScanare();
  }
  if (scanActiv && t - timpScanActiv > DURATA_SCAN_MS) {
    scanActiv = false;
  }


  ActualizeazaPlayer(joyY, joyX, butonApasat);
  ActualizeazaInamici();


  for (int dx = -3; dx <= 3; ++dx) {
    for (int dy = -3; dy <= 3; ++dy) {
      int nx = jucator.x + dx;
      int ny = jucator.y + dy;
      if (InDomeniu(nx, ny)) fog[nx][ny] = true;
    }
  }

  viewportX = constrain(jucator.x - LATIME_VIEW / 2, 0, LUNGIME_HARTA - LATIME_VIEW);
  viewportY = constrain(jucator.y - LATIME_VIEW / 2, 0, LUNGIME_HARTA - LATIME_VIEW);

  
  int totalFragmente = 0;
  for (int i = 0; i < LUNGIME_HARTA; ++i)
    for (int j = 0; j < LUNGIME_HARTA; ++j)
      if (hartaLogica[i][j] == 2) totalFragmente++;


  if (hartaLogica[jucator.x][jucator.y] == 3 && jucator.fragmenteColectate >= FRAGMENTE_PENTRU_TRECERE) {
    SunetEvent("nivelup");
    nivelCurent++;
    for (int i = 0; i < INAMICI_MAX; ++i) {
      if (!inamici[i].activ) {
        inamici[i].activ = true;
        inamici[i].x = random(2, LUNGIME_HARTA - 2);
        inamici[i].y = random(2, LUNGIME_HARTA - 2);
        inamici[i].intervalMiscare = max(200UL, TimpMiscareInamicMin - nivelCurent * 30);
        break;
      }
    }
    GenereazaHartaInitiala();
    jucator.fragmenteColectate = 0;
    ResetFog();
    PlaseazaPlayerSiInamici();
    ActualizeazaScorLCD(); 
  }

  for (int i = 0; i < INAMICI_MAX; ++i) {
    if (!inamici[i].activ) continue;

    if (inamici[i].x == jucator.x && inamici[i].y == jucator.y) {
      if (!jucator.invulnerabil) {
        jucator.vieti--;  
        jucator.invulnerabil = true;
        jucator.timpInvulnerabil = t;
        SunetEvent("hit");

        AfiseazaVietiLCD(); 

        if (jucator.vieti <= 0) {
          jocPornit = false;

        
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("   GAME OVER!");
          lcd.setCursor(0, 1);
          lcd.print("   Nivel: ");
          lcd.print(nivelCurent);

          StareCurenta = StareAplicatie::MeniuPrincipal; 
  
           ActualizeazaMeniuPrincipal();

          return;  
        }
      }
    }
  }

  if (jucator.invulnerabil && t - jucator.timpInvulnerabil > 1500) {
    jucator.invulnerabil = false;
  }

  DeseneazaMatrice();
}

void ActualizeazaPlayer(int joyY, int joyX, bool butonApasat) {
  unsigned long t = millis();
  if (t - jucator.ultimPas < TimpPasPlayerMs) return;

  int dx = 0;
  int dy = 0;

  if (joyY < PragSus) dy = -1;
  if (joyY > PragJos) dy = 1;

  if (joyX < PragSus) dx = -1;
  if (joyX > PragJos) dx = 1;

  int nx = jucator.x + dx;
  int ny = jucator.y + dy;


  if (dx != 0 || dy != 0) {
    if (InDomeniu(nx, ny) && hartaLogica[nx][ny] != 1) {
      jucator.x = nx;
      jucator.y = ny;
      jucator.ultimPas = t;

      SunetEvent("pas");

 
      if (hartaLogica[jucator.x][jucator.y] == 2) {
        hartaLogica[jucator.x][jucator.y] = 0;
        jucator.fragmenteColectate++;
        SunetEvent("collect");
        ActualizeazaScorLCD();
      }
    }
  }
}

void ActualizeazaInamici() {
  unsigned long t = millis();
  for (int i = 0; i < INAMICI_MAX; ++i) {
    if (!inamici[i].activ) continue;
    if (t - inamici[i].ultimPas < inamici[i].intervalMiscare) continue;

    inamici[i].ultimPas = t;

    int dx = jucator.x - inamici[i].x;
    int dy = jucator.y - inamici[i].y;
    int dist = abs(dx) + abs(dy);
    if (dist <= 5 && random(0, 100) < 70) {

      int mx = (dx == 0) ? 0 : (dx > 0 ? 1 : -1);
      int my = (dy == 0) ? 0 : (dy > 0 ? 1 : -1);
      int nx = inamici[i].x + mx;
      int ny = inamici[i].y + my;
      if (InDomeniu(nx, ny) && hartaLogica[nx][ny] != 1) {
        inamici[i].x = nx;
        inamici[i].y = ny;
      } else {
        int rx = random(-1, 2);
        int ry = random(-1, 2);
        int rnx = inamici[i].x + rx;
        int rny = inamici[i].y + ry;
        if (InDomeniu(rnx, rny) && hartaLogica[rnx][rny] != 1) {
          inamici[i].x = rnx;
          inamici[i].y = rny;
        }
      }
    } else {
      int rx = random(-1, 2);
      int ry = random(-1, 2);
      int rnx = inamici[i].x + rx;
      int rny = inamici[i].y + ry;
      if (InDomeniu(rnx, rny) && hartaLogica[rnx][rny] != 1) {
        inamici[i].x = rnx;
        inamici[i].y = rny;
      }
    }
  }
}


void DeseneazaMatrice() {
  if (scanActiv) {
    DeseneazaViewport();
    return;
  } else {
    DeseneazaViewport();
  }
}

void DeseneazaViewport() {


  matrice.clearDisplay(0);

  for (int vx = 0; vx < LATIME_VIEW; ++vx) {
    for (int vy = 0; vy < LATIME_VIEW; ++vy) {
      int hx = viewportX + vx;
      int hy = viewportY + vy;
      bool vizibil = fog[hx][hy] || scanActiv;

      bool ledOn = false;

      if (!InDomeniu(hx, hy)) {
        ledOn = false;
      } else if (!vizibil) {
      
        ledOn = false;
      } else {
        byte cell = hartaLogica[hx][hy];
        if (cell == 1) {
       
          ledOn = true;
        } else if (cell == 2) {
        
          ledOn = true;
        } else if (cell == 3) {
          
          ledOn = true;
        } else {
          ledOn = false;
        }
      }


      if (vizibil && jucator.x == hx && jucator.y == hy) {
        SeteazaLedPhysical(vx, vy, true);
        continue;
      }

      bool inamicAcolo = false;
      for (int k = 0; k < INAMICI_MAX; ++k) {
        if (!inamici[k].activ) continue;
        if (inamici[k].x == hx && inamici[k].y == hy && (fog[hx][hy] || scanActiv)) {
          inamicAcolo = true;
          break;
        }
      }
      if (inamicAcolo) {
        SeteazaLedPhysical(vx, vy, true);
        continue;
      }

     
      if (vizibil && (hartaLogica[hx][hy] == 1 || hartaLogica[hx][hy] == 2 || hartaLogica[hx][hy] == 3)) {
        SeteazaLedPhysical(vx, vy, true);
      } else {
        SeteazaLedPhysical(vx, vy, false);
      }
    }
  }
}


void SeteazaLedPhysical(int xViz, int yViz, bool stare) {
  if (xViz < 0 || xViz >= 8 || yViz < 0 || yViz >= 8) return;
  matrice.setLed(0, yViz, xViz, stare);
}


int CitesteUltrasonicCm() {
  digitalWrite(PinTrig, LOW);
  delayMicroseconds(2);
  digitalWrite(PinTrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(PinTrig, LOW);
  long durata = pulseIn(PinEcho, HIGH, 20000);  
  if (durata == 0) return -1;
  int dist = (int)(durata / 58);  
  return dist;
}

void ActiveazaScanare() {
  scanActiv = true;
  timpScanActiv = millis();
  SunetEvent("scan");
  for (int i = 0; i < LUNGIME_HARTA; ++i)
    for (int j = 0; j < LUNGIME_HARTA; ++j)
      fog[i][j] = true;
}


void SunetEvent(const char *tip) {
  noTone(PinBuzzer);

  if (strcmp(tip, "pas") == 0) {
    tone(PinBuzzer, 500, 30);
  } else if (strcmp(tip, "collect") == 0) {
    tone(PinBuzzer, 1500, 100);  
  } else if (strcmp(tip, "hit") == 0) {
    tone(PinBuzzer, 150, 200);  
  } else if (strcmp(tip, "scan") == 0) {
    tone(PinBuzzer, 800, 50);
  }
}


