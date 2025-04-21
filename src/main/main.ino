#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C screen(0x27, 16, 2);

// pins for components
const int btnModePin = 5;
const int btnUpPin = 6;
const int btnDownPin = 7;
const int buzPin = 8;

// current time
int h = 0, m = 0, s = 0;

// alarm
int al_h = 6, al_m = 0;
bool alarmOn = false;

// mode for edit 
// 0 - nothing, 1 - hours for current time, 2 - minuts for current time, 3 - hours for alarm, 4 - mins for alarm 
int mode = 0;

// for counting seconds
unsigned long lastTick = 0;
const unsigned long tickInterval = 1000;

void setup() {
  screen.begin();
  screen.backlight();

  pinMode(btnModePin, INPUT_PULLUP);
  pinMode(btnUpPin, INPUT_PULLUP);
  pinMode(btnDownPin, INPUT_PULLUP);
  pinMode(buzPin, OUTPUT);
}

void loop() {
  updateClock();
  showInfo();
  checkButtons();
  checkAlarm();
}

void updateClock() {
  // count time
  if (millis() - lastTick >= tickInterval) {
    lastTick = millis();
    s++;
    if (s >= 60) {
      s = 0;
      m++;
      if (m >= 60) {
        m = 0;
        h++;
        if (h >= 24) h = 0;
      }
    }
  }
}

void showInfo() {
  screen.setCursor(0, 0);
  screen.print("Time ");
  prnt2dig(h);
  screen.print(":");
  prnt2dig(m);
  screen.print(":");
  prnt2dig(s);

  screen.setCursor(0, 1);
  screen.print("BOMB ");
  prnt2dig(al_h);
  screen.print(":");
  prnt2dig(al_m);
  screen.print(alarmOn ? " ON " : " OFF");
}

void prnt2dig(int n) {
  /* 
    make print numbers with '00' syntax. Exmaple, we have number 7(in display we will see - hh12 : mm 7 (what?) : ss 37 )
    and using this func we will see '07' instead just '7' and in display we will see - hh 12 : mm 07 : ss 37 
  */
  if (n < 10) screen.print("0");
  screen.print(n);
}


// handle button presses
void checkButtons() {
  // last buttons states
  static bool lastSet = HIGH, lastUp = HIGH, lastDown = HIGH;
  static unsigned long lastDebounce = 0;
  
  const unsigned long debounceDelay = 200;  // wait time to avoid bouncing

  // read real states
  bool nowSet = digitalRead(btnModePin);
  bool nowUp = digitalRead(btnUpPin);
  bool nowDown = digitalRead(btnDownPin);

  // Set
  if (nowSet == LOW && lastSet == HIGH && millis() - lastDebounce > debounceDelay) {
    mode = (mode + 1) % 5;
    lastDebounce = millis();
  }

  // Up
  if (nowUp == LOW && lastUp == HIGH && millis() - lastDebounce > debounceDelay) {
    changeValue(1);
    lastDebounce = millis();
  }

  // Down
  if (nowDown == LOW && lastDown == HIGH && millis() - lastDebounce > debounceDelay) {
    changeValue(-1);
    lastDebounce = millis();
  }

  // save button states for next loop
  lastSet = nowSet;
  lastUp = nowUp;
  lastDown = nowDown;
}

// handler for setting mode. Delta means something like 'change' value after buttons presses 
void changeValue(int delta) {
  switch (mode) {
    case 1: h = (h + delta + 24) % 24; break;
    case 2: m = (m + delta + 60) % 60; break;
    case 3: al_h = (al_h + delta + 24) % 24; alarmOn = true; break;
    case 4: al_m = (al_m + delta + 60) % 60; alarmOn = true; break;
  }
}


void checkAlarm() {
  if (alarmOn && h == al_h && m == al_m && s == 0) {
    tone(buzPin, 900);
    delay(5000);
    noTone(buzPin);
    alarmOn = false;
  }
}
