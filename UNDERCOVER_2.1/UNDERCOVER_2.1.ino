#include <LiquidCrystal.h>
#include <EEPROM.h>
#include "LedControl.h"

#include "joc.h"
#include "matrice_disp.h"
#include "sunet.h"
#include "ultrasonic.h"
#include "highscore.h"
#include "setari.h"
#include "lcd.h"


LCD lcd_afisaj;

constexpr byte PinLcdRs = 9;
constexpr byte PinLcdEn = 8;
constexpr byte PinLcdD4 = 7;
constexpr byte PinLcdD5 = 6;
constexpr byte PinLcdD6 = 5;
constexpr byte PinLcdD7 = 4;

LiquidCrystal lcd(
  PinLcdRs,
  PinLcdEn,
  PinLcdD4,
  PinLcdD5,
  PinLcdD6,
  PinLcdD7);


constexpr byte PinLuminaFundal = 3;
constexpr byte PinBuzzer = A3;
constexpr byte PinButonJoy = 2;
constexpr byte PinJoyX = A0;
constexpr byte PinJoyY = A1;
const byte PinLedVerde = A5;
bool SunetActiv = true;


constexpr int PragSus = 400;
constexpr int PragJos = 600;
constexpr unsigned long TimpNavigareMeniu = 180;
const unsigned long DebounceButonMs = 50;
int afisareTimpIntro = 2000;

bool necesitaIntroducereNume = false;
char numeTemp[4] = { 'A', 'A', 'A', '\0' };
int literaSelectata = 0;

bool primaIntrareGameOver = true;
bool primaIntrareGameOverSimplu = true;
bool afisareCastig = false;

unsigned long tScrollGO = 0;
unsigned long tGameOver = 0;
int offsetGO = 0;

bool joyBloc = false;

int IndexMeniuPrincipal = 0;
int IndexHighscore = 0;
unsigned long UltimaNavigareMeniu = 0;
int timpDebounceScurt = 150;



const int MeniuPrincipalCount = 6;
const int NrSetari = 4;
bool optiuneDA = true;

enum  StareAplicatie {
  MeniuPrincipal,
  MeniuLuminozitate,
  ModJoc,
  MeniuSetari,
  MeniuHighscore,
  MeniuIntro,
  MeniuDespre,
  MeniuCumSeJoaca,
  MeniuGameOver,
  MeniuIntroducereNume,
  MeniuClasament,
  MeniuResetHighscore,
  SetareLumLCD,
  SetareLumMatrix,
  SetareSunet,
  JocTerminatSucces,
};

StareAplicatie StareCurenta = StareAplicatie::MeniuIntro;

enum class SubMeniuSetari {
  LuminozLCD,
  LuminozMatrix,
  Sunet,
  Inapoi,
  Count
};

int IndexMeniuSetari = 0;

const char *ListaSetari[] = {
  "Luminoz LCD",
  "Luminoz Matrix",
  "Sunet",
  "Inapoi"
};


const byte IconitaStartJoc[8] = {
  0b00001000,
  0b00001100,
  0b00001110,
  0b00001111,
  0b00001110,
  0b00001100,
  0b00001000,
  0b00000000
};

const byte IconitaClasament[8] = {
  0b00111100,
  0b01111110,
  0b11011011,
  0b01111110,
  0b00111100,
  0b00011000,
  0b00111100,
  0b00011000
};

const byte IconitaLuminozitate[8] = {
  0b00100100,
  0b00011000,
  0b01111110,
  0b01100110,
  0b01100110,
  0b01111110,
  0b00011000,
  0b00100100
};

const byte IconitaResetCls[8] = {
  0b11000011,
  0b01100110,
  0b00111100,
  0b00011000,
  0b00011000,
  0b00111100,
  0b01100110,
  0b11000011
};

const byte IconitaCumSeJoaca[8] = {
  0b00111100,
  0b01000010,
  0b00000010,
  0b00000100,
  0b00011000,
  0b00000000,
  0b00011000,
  0b00011000
};

const byte IconitaDespre[8] = {
  0b00011000,
  0b00011000,
  0b00000000,
  0b00111100,
  0b00011000,
  0b00011000,
  0b00111100,
  0b00000000
};

const byte IconitaUnibucRobot[8] = {
  0b00111100,
  0b01100110,
  0b01100110,
  0b00111100,
  0b00111100,
  0b01111110,
  0b01011010,
  0b00100100
};

const byte IconitaEvadare[8] = {
  0b00111100,
  0b01111110,
  0b11000011,
  0b11011011,
  0b11011011,
  0b11000011,
  0b01111110,
  0b00111100
};

const byte *IconiteMeniuPrincipal[MeniuPrincipalCount] = {
  IconitaStartJoc,
  IconitaLuminozitate,
  IconitaClasament,
  IconitaResetCls,
  IconitaCumSeJoaca,
  IconitaDespre
};


const char gameOverMsg[] = "   GAME OVER   ";
const unsigned long intervalScrollGO = 200;


constexpr int NivelLumin = 5;
const byte NivLumImplicit = 2;
constexpr int NivelLuminMat = 15;

byte NivelLuminozitateLCD = NivLumImplicit;
byte NivelLuminozitateMatrix = 1;

int timpDebounceLung = 150;
byte ValoriLuminozitateLCD[5] = { 50, 100, 150, 200, 255 };

constexpr int ErpromLumLcd = 0;
constexpr int ErpromLumMatrix = 1;
constexpr int ErpromLumSunet = 2;


MatriceAfisaj matrice;
Sunet sunet;
Joc joc;
Highscore hs;



void CitesteButon(unsigned long t, bool &butonApasat, bool &apasareDetectata);
void ActualizeazaMeniuPrincipal();
void GestioneazaMeniuPrincipal(int joyY, int joyX, bool apasatEveniment);
void ActualizeazaMeniuLuminozitate();
void GestioneazaMeniuLuminozitate(int joyY, int joyX, bool apasatEveniment);
void AfiseazaIntro();

void AfiseazaCumSeJoaca();
void GestioneazaCumSeJoaca(bool apasatEveniment);
void AfiseazaDespre();
void GestioneazaDespre(bool apasatEveniment);


void afiseazaGameOverScroll();
bool introducereNumeLoop(bool apasareDetectata);
bool afiseazaClasamentLoop();

void AfiseazaSetareLuminozLCD();
void AfiseazaSetareLuminozMatrix();
void AfiseazaSetareSunet();

void GestioneazaSetareLumLCD(int joyX, bool inapoi);
void GestioneazaSetareLumMatrix(int joyX, bool inapoi);
void GestioneazaSetareSunet(int joyX, bool inapoi);


void SalvareSetariEEPROM();
void CitesteSetariEEPROM();
void afiseazaScorNivel();

void setup() {

  Serial.begin(9600);
  lcd.begin(16, 2);


  pinMode(PinLuminaFundal, OUTPUT);
  pinMode(PinButonJoy, INPUT_PULLUP);


  matrice.init();
  sunet.initalizare(PinBuzzer);
  initUltrasonic();
  hs.initializare();


  CitesteSetariEEPROM();
  Serial.println(NivelLuminozitateLCD);
  Serial.println(NivelLuminozitateMatrix);

  joc.initializare(&matrice, &sunet, &hs);

  matrice.setLuminozitate(NivelLuminozitateMatrix);
  byte sunet = EEPROM.read(ErpromLumSunet);
  SunetActiv = (sunet == 1);

  StareCurenta = StareAplicatie::MeniuIntro;

  lcd_afisaj.init(&lcd);
  lcd_afisaj.initCustomChars();

  AfiseazaIntro();
  analogWrite(PinLuminaFundal, ValoriLuminozitateLCD[NivelLuminozitateLCD]);
}



bool introducereNumeLoop(bool apasareDetectata) {
  static unsigned long ultimaApasare = 0;

  int joyX = analogRead(PinJoyX);
  int joyY = analogRead(PinJoyY);


  if (millis() - ultimaApasare > timpDebounceScurt) {

    if (joyY < PragSus) {
      if (numeTemp[literaSelectata] < 'Z') {
        numeTemp[literaSelectata]++;
      }
    } else if (joyY > PragJos) {
      if (numeTemp[literaSelectata] > 'A') {
        numeTemp[literaSelectata]--;
      }
    }


    if (joyX > PragJos && literaSelectata < 2) {
      literaSelectata++;
    } else if (joyX < PragSus && literaSelectata > 0) {
      literaSelectata--;
    }


    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("Introdu nume");

    lcd.setCursor(0, 1);
    lcd.print(numeTemp);

    lcd.setCursor(literaSelectata, 1);
    lcd.print(numeTemp[literaSelectata]);

    ultimaApasare = millis();
  }


  if (apasareDetectata || digitalRead(PinButonJoy) == LOW) {

    static unsigned long momentInceputApasare = 0;
    static bool apasareInCurs = false;

    bool butonApasat = (digitalRead(PinButonJoy) == LOW);

    /// am inceput apasarea
    if (butonApasat && !apasareInCurs) {
      apasareInCurs = true;
      momentInceputApasare = millis();
    }

    // eliberam butonul
    if (!butonApasat && apasareInCurs) {
      apasareInCurs = false;
      return true;
    }

    // apasare lunga
    if (butonApasat && apasareInCurs && (millis() - momentInceputApasare) > timpDebounceLung) {
      apasareInCurs = false;
      return true;
    }
  }

  return false;
}



bool afiseazaClasamentLoop() {
  static unsigned long timpStart = 0;
  static byte stareAfisare = 0;

  const unsigned long intervalAfisare = 2500;

  if (stareAfisare == 0) {
    lcd.clear();

    HighscoreEntry loc1 = hs.get(0);
    lcd.setCursor(0, 0);
    lcd.print("1.");
    lcd.print(loc1.nume);
    lcd.print(" S:");
    lcd.print(loc1.scor);

    HighscoreEntry loc2 = hs.get(1);
    lcd.setCursor(0, 1);
    lcd.print("2.");
    lcd.print(loc2.nume);
    lcd.print(" S:");
    lcd.print(loc2.scor);

    timpStart = millis();
    stareAfisare = 1;
  }

  if (stareAfisare == 1 && millis() - timpStart > intervalAfisare) {

    lcd.clear();

    HighscoreEntry loc3 = hs.get(2);
    lcd.setCursor(0, 0);
    lcd.print("Locul 3:");

    lcd.setCursor(0, 1);
    lcd.print("3.");
    lcd.print(loc3.nume);
    lcd.print(" S:");
    lcd.print(loc3.scor);

    timpStart = millis();
    stareAfisare = 2;
  }

  if (stareAfisare == 2 && millis() - timpStart > intervalAfisare) {

    stareAfisare = 0;
    return true;
  }

  return false;
}


void loop() {
  unsigned long timpCurent = millis();

  int valJoyY = analogRead(PinJoyY);
  int valJoyX = analogRead(PinJoyX);

  bool butonApasat;
  bool apasareDetectata;
  CitesteButon(timpCurent, butonApasat, apasareDetectata);


  switch (StareCurenta) {

    case StareAplicatie::MeniuIntro:
      {
        static unsigned long timpStartIntro = 0;

        if (timpStartIntro == 0) {
          timpStartIntro = millis();
        }

        if (millis() - timpStartIntro > afisareTimpIntro || apasareDetectata) {
          timpStartIntro = 0;
          StareCurenta = StareAplicatie::MeniuPrincipal;
          ActualizeazaMeniuPrincipal();
        }
        break;
      }



    case StareAplicatie::MeniuPrincipal:
      GestioneazaMeniuPrincipal(valJoyY, valJoyX, apasareDetectata);
      break;


    case StareAplicatie::MeniuSetari:
      GestioneazaMeniuSetari(valJoyY, apasareDetectata);
      break;


    case StareAplicatie::ModJoc:
      {
        joc.loopJoc(valJoyX, valJoyY, butonApasat);

        lcd_afisaj.actualizeaza(
          joc.getItemeColectate(),
          joc.getItemeTotal(),
          joc.getVieti(),
          joc.getNivel(),
          joc.getScor());


        if (!joc.esteActiv()) {
          lcd.clear();

          matrice.seteazaFereastra(0, 0);
          matrice.setCursorPozitie(-1, -1);

          afisareCastig = joc.aCastigat();
          primaIntrareGameOverSimplu = true;

          int scorCurent = joc.getScor();
          necesitaIntroducereNume = (scorCurent > hs.getScor(2));

          StareCurenta = StareAplicatie::MeniuGameOver;
        }

        break;
      }



    case StareAplicatie::MeniuCumSeJoaca:
      GestioneazaCumSeJoaca(apasareDetectata);
      break;


    case StareAplicatie::MeniuDespre:
      GestioneazaDespre(apasareDetectata);
      break;


    case StareAplicatie::MeniuHighscore:
      GestioneazaMeniuHighscore(valJoyY);

      if (apasareDetectata) {
        StareCurenta = StareAplicatie::MeniuPrincipal;
        ActualizeazaMeniuPrincipal();
      }
      break;


    case StareAplicatie::MeniuGameOver:
      {
        if (primaIntrareGameOverSimplu) {
          lcd.clear();

          lcd.setCursor(2, 0);
          lcd.print(afisareCastig ? "Good job!" : "GAME OVER!");

          lcd.setCursor(1, 1);
          lcd.print("S: ");
          lcd.print(joc.getScor());

          lcd.setCursor(1, 7);
          lcd.print("Nivel: ");
          lcd.print(joc.getNivel());

          tGameOver = millis();
          primaIntrareGameOverSimplu = false;
        }

        if (millis() - tGameOver >= 3000) {

          if (necesitaIntroducereNume) {
            literaSelectata = 0;
            numeTemp[0] = 'A';
            numeTemp[1] = 'A';
            numeTemp[2] = 'A';

            StareCurenta = StareAplicatie::MeniuIntroducereNume;
          } else {
            StareCurenta = StareAplicatie::MeniuPrincipal;
            ActualizeazaMeniuPrincipal();
          }
        }

        break;
      }


    case StareAplicatie::MeniuIntroducereNume:
      if (introducereNumeLoop(apasareDetectata)) {
        hs.adaugaScor(numeTemp, joc.getScor());
        StareCurenta = StareAplicatie::MeniuClasament;
      }
      break;


    case StareAplicatie::MeniuClasament:
      if (afiseazaClasamentLoop()) {
        StareCurenta = StareAplicatie::MeniuPrincipal;
        ActualizeazaMeniuPrincipal();
      }
      break;


    case StareAplicatie::MeniuResetHighscore:
      GestioneazaMeniuResetHighscore(valJoyX, apasareDetectata);
      break;


    case StareAplicatie::SetareLumLCD:
      GestioneazaSetareLumLCD(valJoyX, apasareDetectata);
      break;

    case StareAplicatie::SetareLumMatrix:
      GestioneazaSetareLumMatrix(valJoyX, apasareDetectata);
      break;

    case StareAplicatie::SetareSunet:
      GestioneazaSetareSunet(valJoyX, apasareDetectata);
      break;


    default:
      StareCurenta = StareAplicatie::MeniuPrincipal;
      ActualizeazaMeniuPrincipal();
      break;
  }


  matrice.actualizeazaBlink();
  matrice.deseneazaMatrice();

  analogWrite(PinLuminaFundal, ValoriLuminozitateLCD[NivelLuminozitateLCD]);
}



void CitesteButon(unsigned long timpCurent, bool &butonApasat, bool &apasareDetectata) {

  static bool stareStabila = false;
  static bool stareBruta = false;
  static unsigned long timpUltimaSchimbare = 0;


  bool citireBruta = !digitalRead(PinButonJoy);
  apasareDetectata = false;


  if (citireBruta != stareBruta) {
    stareBruta = citireBruta;
    timpUltimaSchimbare = timpCurent;
  }


  if (timpCurent - timpUltimaSchimbare > DebounceButonMs) {

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

  const char *items[MeniuPrincipalCount] = {
    "Start Joc",
    "Setari",
    "Clasament",
    "Resetare CLS",
    "Cum se joaca",
    "Despre"
  };

  lcd.clear();
  matrice.setCursorPozitie(-1, -1);


  lcd.setCursor(0, 0);
  lcd.print(">");
  lcd.print(items[IndexMeniuPrincipal]);


  int next = (IndexMeniuPrincipal + 1) % MeniuPrincipalCount;
  lcd.setCursor(0, 1);
  lcd.print(" ");
  lcd.print(items[next]);


  analogWrite(PinLuminaFundal, ValoriLuminozitateLCD[NivelLuminozitateLCD]);

  matrice.afiseazaIconita16(IconiteMeniuPrincipal[IndexMeniuPrincipal]);
}




void GestioneazaMeniuPrincipal(int joyY, int joyX, bool apasatEveniment) {
  unsigned long timpCurent = millis();
  int indexVechi = IndexMeniuPrincipal;

  if (timpCurent - UltimaNavigareMeniu > TimpNavigareMeniu) {

    // de la 5 la 0
    if (joyY < PragSus)
      IndexMeniuPrincipal = (IndexMeniuPrincipal + MeniuPrincipalCount - 1) % MeniuPrincipalCount;

    else if (joyY > PragJos) {
      IndexMeniuPrincipal = (IndexMeniuPrincipal + 1) % MeniuPrincipalCount;
    }

    if (IndexMeniuPrincipal != indexVechi) {
      sunet.sunetPas();
      ActualizeazaMeniuPrincipal();
    }

    UltimaNavigareMeniu = timpCurent;
  }


  bool joyCentral = (joyX > PragSus && joyX < PragJos && joyY > PragSus && joyY < PragJos);

  if (apasatEveniment && joyCentral) {
    sunet.sunetItem();

    switch (IndexMeniuPrincipal) {

      case 0:
        lcd.clear();
        StareCurenta = StareAplicatie::ModJoc;
        joc.startNivel(1);
        break;

      case 1:
        StareCurenta = StareAplicatie::MeniuSetari;
        AfiseazaMeniuSetari();
        break;

      case 2:
        StareCurenta = StareAplicatie::MeniuHighscore;
        ActualizeazaMeniuHighscore();
        break;

      case 3:
        StareCurenta = StareAplicatie::MeniuResetHighscore;
        ActualizeazaMeniuResetHighscore(true);
        break;

      case 4:
        StareCurenta = StareAplicatie::MeniuCumSeJoaca;
        AfiseazaCumSeJoaca();
        break;

      case 5:
        StareCurenta = StareAplicatie::MeniuDespre;
        AfiseazaDespre();
        break;
    }
  }
}


///
void AfiseazaIntro() {

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Undercover");
  lcd.setCursor(0, 1);
  lcd.print("UNIBUC Robotica");
  matrice.afiseazaIconita16(IconitaEvadare);
}

void ActualizeazaMeniuHighscore() {
  lcd.clear();


  lcd.setCursor(0, 0);
  lcd.print("Clasament");


  lcd.setCursor(0, 1);
  lcd.print(">");


  HighscoreEntry entry = hs.get(IndexHighscore);
  lcd.print(entry.nume);
  lcd.print(" ");
  lcd.print(entry.scor);
}


void GestioneazaMeniuHighscore(int joyY) {
  unsigned long t = millis();

  if (t - UltimaNavigareMeniu > TimpNavigareMeniu) {

    if (joyY < PragSus) {
      if (IndexHighscore > 0)
        IndexHighscore--;
      else IndexHighscore = hs.count() - 1;
      ActualizeazaMeniuHighscore();
    }

    if (joyY > PragJos) {
      IndexHighscore = (IndexHighscore + 1) % hs.count();
      ActualizeazaMeniuHighscore();
    }

    UltimaNavigareMeniu = t;
  }
}


void ActualizeazaMeniuResetHighscore(bool daSelectat) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Resetare scoruri?");

  lcd.setCursor(0, 1);

  if (daSelectat) {
    lcd.print(">DA    NU ");
  } else {
    lcd.print(" DA   >NU");
  }
}



void GestioneazaMeniuResetHighscore(int joyX, bool apasare) {
  static bool primaIntrare = true;
  static bool optiuneAnterioara = true;

  if (primaIntrare) {
    ActualizeazaMeniuResetHighscore(optiuneDA);
    optiuneAnterioara = optiuneDA;
    primaIntrare = false;
  }


  if (joyX < PragSus) {
    optiuneDA = true;
  } else if (joyX > PragJos) {
    optiuneDA = false;
  }

  if (optiuneDA != optiuneAnterioara) {
    ActualizeazaMeniuResetHighscore(optiuneDA);
    optiuneAnterioara = optiuneDA;
  }

  if (apasare) {

    if (optiuneDA) {
      hs.reseteaza();
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Clasament");
      lcd.setCursor(0, 1);
      lcd.print("resetat!");
    }
    primaIntrare = true;
    StareCurenta = StareAplicatie::MeniuPrincipal;
    ActualizeazaMeniuPrincipal();
  }
}


void AfiseazaCumSeJoaca() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Colectioneaza &");
  lcd.setCursor(0, 1);
  lcd.print("Fereste-te");
}

void GestioneazaCumSeJoaca(bool apasatEveniment) {
  if (apasatEveniment) {
    StareCurenta = StareAplicatie::MeniuPrincipal;
    ActualizeazaMeniuPrincipal();
  }
}


void AfiseazaDespre() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Undercover 2.1");
  lcd.setCursor(0, 1);
  lcd.print("by Dana");
}

void GestioneazaDespre(bool apasatEveniment) {
  if (apasatEveniment) {
    StareCurenta = StareAplicatie::MeniuPrincipal;
    ActualizeazaMeniuPrincipal();
  }
}


void SalvareSetariEEPROM() {

  EEPROM.update(ErpromLumLcd, NivelLuminozitateLCD);
  EEPROM.update(ErpromLumSunet, SunetActiv ? 1 : 0);
  EEPROM.update(ErpromLumMatrix, NivelLuminozitateMatrix);
}


void CitesteSetariEEPROM() {

  NivelLuminozitateLCD = EEPROM.read(ErpromLumLcd);
  if (NivelLuminozitateLCD > NivelLumin) {
    NivelLuminozitateLCD = NivLumImplicit;
  }

  byte sunetCitit = EEPROM.read(ErpromLumSunet);
  SunetActiv = (sunetCitit == 1);

  NivelLuminozitateMatrix = EEPROM.read(ErpromLumMatrix);
  if (NivelLuminozitateMatrix > NivelLuminMat) {
    NivelLuminozitateMatrix = NivelLuminMat;
  }
}


void AfiseazaMeniuSetari() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Setari");

  lcd.setCursor(0, 1);
  lcd.print("> ");
  lcd.print(ListaSetari[IndexMeniuSetari]);
}



void GestioneazaMeniuSetari(int joyY, bool butonApasat) {

  if (joyY < PragSus && !joyBloc) {
    IndexMeniuSetari--;

    if (IndexMeniuSetari < 0) {
      IndexMeniuSetari = NrSetari - 1;
    }

    sunet.sunetPas();
    AfiseazaMeniuSetari();
    joyBloc = true;
  }

  if (joyY > PragJos && !joyBloc) {
    IndexMeniuSetari++;

    if (IndexMeniuSetari >= NrSetari) {
      IndexMeniuSetari = 0;
    }

    sunet.sunetPas();
    AfiseazaMeniuSetari();
    joyBloc = true;
  }

  if (joyY > PragSus && joyY < PragJos) {
    joyBloc = false;
  }

  if (butonApasat) {
    sunet.sunetItem();
    IntraInSetare();
  }
}



void IntraInSetare() {
  switch (IndexMeniuSetari) {

    case 0:
      StareCurenta = StareAplicatie::SetareLumLCD;
      AfiseazaSetareLuminozLCD();
      break;

    case 1:
      StareCurenta = StareAplicatie::SetareLumMatrix;
      AfiseazaSetareLuminozMatrix();
      break;

    case 2:
      StareCurenta = StareAplicatie::SetareSunet;
      AfiseazaSetareSunet();
      break;

    case 3:
      StareCurenta = StareAplicatie::MeniuPrincipal;
      ActualizeazaMeniuPrincipal();
      break;
  }
}


void AfiseazaSetareLuminozLCD() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Luminoz LCD");

  lcd.setCursor(0, 1);
  lcd.print("Nivel: ");
  lcd.print(NivelLuminozitateLCD + 1);
}


void GestioneazaSetareLumLCD(int joyX, bool inapoi) {

  static bool joyBlocX = false;
  bool valoareSchimbata = false;

  if (joyX < PragSus && !joyBlocX && NivelLuminozitateLCD > 0) {
    NivelLuminozitateLCD--;
    joyBlocX = true;
    valoareSchimbata = true;
  }


  if (joyX > PragJos && !joyBlocX && NivelLuminozitateLCD < (NivelLumin - 1)) {
    NivelLuminozitateLCD++;
    joyBlocX = true;
    valoareSchimbata = true;
  }

  if (joyX > PragSus && joyX < PragJos) joyBlocX = false;


  if (valoareSchimbata) {
    analogWrite(PinLuminaFundal, ValoriLuminozitateLCD[NivelLuminozitateLCD]);
    AfiseazaSetareLuminozLCD();
    EEPROM.update(ErpromLumLcd, NivelLuminozitateLCD);
  }

  if (inapoi) {
    StareCurenta = StareAplicatie::MeniuSetari;
    AfiseazaMeniuSetari();
  }
}





void AfiseazaSetareLuminozMatrix() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Luminoz Matrix");
  lcd.setCursor(0, 1);
  lcd.print("Val: ");
  lcd.print(NivelLuminozitateMatrix);
}



void GestioneazaSetareLumMatrix(int joyX, bool inapoi) {

  static bool joyBlocX = false;
  bool valoareSchimbata = false;

  if (joyX < PragSus && !joyBlocX && NivelLuminozitateMatrix > 0) {
    NivelLuminozitateMatrix--;
    joyBlocX = true;
    valoareSchimbata = true;
  }

  if (joyX > PragJos && !joyBlocX && NivelLuminozitateMatrix < 15) {
    NivelLuminozitateMatrix++;
    joyBlocX = true;
    valoareSchimbata = true;
  }

  if (joyX > PragSus && joyX < PragJos)
    joyBlocX = false;

  if (valoareSchimbata) {
    matrice.setLuminozitate(NivelLuminozitateMatrix);
    AfiseazaSetareLuminozMatrix();
    EEPROM.update(ErpromLumMatrix, NivelLuminozitateMatrix);
  }

  if (inapoi) {
    StareCurenta = StareAplicatie::MeniuSetari;
    AfiseazaMeniuSetari();
  }
}





void GestioneazaSetareSunet(int joyX, bool inapoi) {

  static bool joyBlocX = false;
  bool valoareSchimbata = false;

  if ((joyX < PragSus || joyX > PragJos) && !joyBlocX) {
    SunetActiv = !SunetActiv;
    joyBlocX = true;
    valoareSchimbata = true;
  }

  if (joyX > PragSus && joyX < PragJos)
    joyBlocX = false;

  if (valoareSchimbata) {
    EEPROM.update(ErpromLumSunet, SunetActiv ? 1 : 0);
    AfiseazaSetareSunet();
  }

  if (inapoi) {
    StareCurenta = StareAplicatie::MeniuSetari;
    AfiseazaMeniuSetari();
  }
}



void AfiseazaSetareSunet() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Sunet");

  lcd.setCursor(0, 1);
  if (SunetActiv) {
    lcd.print("ON");
  } else {
    lcd.print("OFF");
  }
}


void afiseazaScorNivel() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("SCOR: ");
  lcd.print(joc.getScor());

  lcd.setCursor(0, 1);
  lcd.print("NIVEL: ");
  lcd.print(joc.getNivel());
}
