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

constexpr unsigned long TimpNavigareMeniu = 180;
constexpr unsigned long TimpApasareLunga = 800;

constexpr unsigned long BlinkLentMs = 500;
constexpr unsigned long BlinkRapidMs = 150;

LiquidCrystal lcd(PinLcdRs, PinLcdEn, PinLcdD4, PinLcdD5, PinLcdD6, PinLcdD7);

byte NivelLuminozitate = 2;
byte ValoriLuminozitate[5] = { 50, 100, 150, 200, 255 };

const byte dinPin = 12;
const byte clockPin = 11;
const byte loadPin = 10;

LedControl matrice(dinPin, clockPin, loadPin, 1);

enum class StareLedMatrice {
  Oprit,
  Fix,
  BlinkLent,
  BlinkRapid
};

StareLedMatrice StareMatrice[8][8];

unsigned long UltimBlinkLent = 0;
unsigned long UltimBlinkRapid = 0;
bool FazaBlinkLent = false;
bool FazaBlinkRapid = false;

int CursorX = 0;
int CursorY = 0;
unsigned long UltimaMiscareJoy = 0;

enum class StareAplicatie {

  MeniuPrincipal,
  MeniuLuminozitate,
  ModJoc
};

StareAplicatie StareCurenta = StareAplicatie::MeniuPrincipal;

int IndexMeniuPrincipal = 0;
int NivelLuminozitateTemporar;

unsigned long UltimaNavigareMeniu = 0;

const int NivelLumin = 5;
const int NivLumImplicit = 2;


bool prevButonDebounced = false;
unsigned long ultimSchimbareButon = 0;
const unsigned long DebounceButonMs = 50;
bool apasatEveniment = false;

void setup() {

  lcd.begin(16, 2);
  pinMode(PinLuminaFundal, OUTPUT);
  pinMode(PinButonJoy, INPUT_PULLUP);

  matrice.shutdown(0, false);
  matrice.setIntensity(0, 1);
  matrice.clearDisplay(0);

  NivelLuminozitate = EEPROM.read(0);

  if (NivelLuminozitate > NivelLumin)
    NivelLuminozitate = NivLumImplicit;

  analogWrite(PinLuminaFundal, ValoriLuminozitate[NivelLuminozitate]);

  ActualizeazaMeniuPrincipal();
}


void loop() {
  
  unsigned long timpCurent = millis();

  int ValJoyY = analogRead(PinJoyY);
  int ValJoyX = analogRead(PinJoyX);

  bool butonApasat;
  bool apasareDetectata;
  CitesteButon(timpCurent, butonApasat, apasareDetectata);

  switch (StareCurenta) {

    case StareAplicatie::MeniuPrincipal:
      GestioneazaMeniuPrincipal(ValJoyY, ValJoyX, apasareDetectata);
      break;

    case StareAplicatie::MeniuLuminozitate:
      GestioneazaMeniuLuminozitate(ValJoyY, ValJoyX, apasareDetectata);
      break;

    case StareAplicatie::ModJoc:
      GestioneazaModJoc(ValJoyY, butonApasat);
      break;
  }

  ActualizeazaBlink();
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


void SchimbaStareLedMatrice(int x, int y) {
  StareMatrice[y][x] =
    static_cast<StareLedMatrice >((static_cast<int>(StareMatrice[y][x]) + 1) % 4);
}

void GestioneazaModJoc(int joyY, bool apasat) {

  static unsigned long momentApasare = 0;
  unsigned long t = millis();

  if (apasat && momentApasare == 0)
    momentApasare = t;

  if (!apasat && momentApasare != 0) {

    unsigned long durata = t - momentApasare;
    momentApasare = 0;

    if (durata > TimpApasareLunga) {

      ReseteazaJocSiArataZambet();
      StareCurenta = StareAplicatie::MeniuPrincipal;
      ActualizeazaMeniuPrincipal();
      return;
    }

    else {
      SchimbaStareLedMatrice(CursorX, CursorY);
    }
  }

  if (t - UltimaMiscareJoy > 150) {

    int joyX = analogRead(PinJoyX);

    if (joyY < PragSus && CursorY > 0)
      CursorY--;

    if (joyY > PragJos && CursorY < 7)
      CursorY++;

    if (joyX < PragSus && CursorX > 0)
      CursorX--;

    if (joyX > PragJos && CursorX < 7)
      CursorX++;

    UltimaMiscareJoy = t;
  }

  lcd.setCursor(0, 0);
  lcd.print("X:");
  lcd.print(CursorX);
  lcd.print(" Y:");
  lcd.print(CursorY);

  lcd.setCursor(0, 1);
  lcd.print("Stare:");
  lcd.print(static_cast<int>(StareMatrice[CursorY][CursorX]));
}

void ReseteazaJocSiArataZambet() {

  for (auto &lin : StareMatrice)
    for (auto &p : lin)
      p = StareLedMatrice ::Oprit;

  matrice.clearDisplay(0);

  static byte Zambet[8] = {
    B00111100, B01000010, B10100101, B10000001,
    B10100101, B10011001, B01000010, B00111100
  };


  unsigned long start = millis();

  while (millis() - start < 800) {

    for (int i = 0; i < 8; i++)
      matrice.setRow(0, i, Zambet[i]);
  }

  matrice.clearDisplay(0);
}

void ActualizeazaBlink() {

  unsigned long t = millis();

  if (t - UltimBlinkLent > BlinkLentMs) {

    FazaBlinkLent = !FazaBlinkLent;
    UltimBlinkLent = t;
  }


  if (t - UltimBlinkRapid > BlinkRapidMs) {

    FazaBlinkRapid = !FazaBlinkRapid;
    UltimBlinkRapid = t;
  }
}

void DeseneazaMatrice() {

  for (int y = 0; y < 8; y++) {
    for (int x = 0; x < 8; x++) {

      bool valoare = false;

      switch (StareMatrice[y][x]) {

        case StareLedMatrice ::Oprit:
          valoare = false;
          break;
        case StareLedMatrice ::Fix:
          valoare = true;
          break;
        case StareLedMatrice ::BlinkLent:
          valoare = FazaBlinkLent;
          break;
        case StareLedMatrice ::BlinkRapid:
          valoare = FazaBlinkRapid;
          break;
      }

      matrice.setLed(0, y, x, valoare);
    }
  }

  matrice.setLed(0, CursorY, CursorX, true);
}
