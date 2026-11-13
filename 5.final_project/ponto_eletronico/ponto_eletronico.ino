
#include <Keypad.h>
#include <LiquidCrystal.h>

#define MAXIN 10

#define ROWS 4
#define COLS 4

#define RS_PIN 12
#define E_PIN 11
#define D4_PIN 5
#define D5_PIN 4
#define D6_PIN 3
#define D7_PIN 2

LiquidCrystal lcd(
  RS_PIN,
  E_PIN,
  D4_PIN,
  D5_PIN,
  D6_PIN,
  D7_PIN
);

char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

const byte rowPins[ROWS] = {22, 24, 26, 28};
const byte colPins[COLS] = {23, 25, 27, 29};

Keypad keypad = Keypad(
  makeKeymap(hexaKeys),
  rowPins,
  colPins,
  ROWS,
  COLS
);

char inputWord[MAXIN + 1] = "";
byte currIndex = 0;

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.print("Trabalha,Karina!");
}

void resetWord() {
  currIndex = 0;
  inputWord[0] = '\0';
}

void deleteKey() {
  if (currIndex > 0) {
    currIndex--;
    inputWord[currIndex] = '\0';
  }
}

void addKey(char key) {
  if (currIndex < MAXIN) {
    inputWord[currIndex] = key;
    currIndex++;
    inputWord[currIndex] = '\0';
  }
}

void loop() {
  char key = keypad.getKey();

  if (key == '*') {
    deleteKey();
  }
  else if (key == '#') {
    resetWord();
  }
  else if (key) {
    addKey(key);
  }
}
