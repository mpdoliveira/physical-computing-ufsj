#define BUTTON_PIN 12
#define POTENTIOMETER_PIN A0
#define GREEN_PIN 2
#define BLUE_PIN 4
#define RED_PIN 7
#define GREEN_RGB_PIN 3
#define BLUE_RGB_PIN 5
#define RED_RGB_PIN 6

int state;
byte last;
byte last_potence;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(BUTTON_PIN, INPUT);
  pinMode(POTENTIOMETER_PIN, INPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_RGB_PIN, OUTPUT);
  pinMode(BLUE_RGB_PIN, OUTPUT);
  pinMode(RED_RGB_PIN, OUTPUT);
  state = 0;
  last = LOW;
  last_potence = analogRead(POTENTIOMETER_PIN);
}



void loop() {
    
  byte button = digitalRead(BUTTON_PIN);
  int potence = analogRead(POTENTIOMETER_PIN);

  bool change = false;
  if (last_potence - potence < -5 || last_potence > 5) {
    change = true;
  }
  
  Serial.println(potence / 4);
  
  if (button == HIGH){
    if (last == LOW) {
      state = (state + 1) % 3;
    }
  }

  if (state == 0 && change) {
    digitalWrite(GREEN_PIN, HIGH);
    digitalWrite(BLUE_PIN, LOW);
    digitalWrite(RED_PIN, LOW);
    analogWrite(GREEN_RGB_PIN, potence / 4);
  }
  else if (state == 1 && change) {
    digitalWrite(GREEN_PIN, LOW);
    digitalWrite(BLUE_PIN, HIGH);
    digitalWrite(RED_PIN, LOW);
    analogWrite(BLUE_RGB_PIN, potence / 4);
  }
  else if (state == 2 && change) {
    digitalWrite(GREEN_PIN, LOW);
    digitalWrite(BLUE_PIN, LOW);
    digitalWrite(RED_PIN, HIGH);
    analogWrite(RED_RGB_PIN, potence / 4);
  }

  last = button;
  last_potence = potence;
  delay(1);
}
