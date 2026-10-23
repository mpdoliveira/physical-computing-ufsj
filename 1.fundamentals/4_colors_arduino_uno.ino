//Saidas
#define BLUE 9
#define GREEN 10
#define RED 11

//Config
#define DELAY 1000

void setup() {
  pinMode(BLUE, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(RED, OUTPUT);
}

void loop() {
  //Yellow
  digitalWrite(RED, HIGH);
  digitalWrite(GREEN, HIGH);
  delay(DELAY);
  digitalWrite(GREEN, LOW);

  //Pink
  digitalWrite(BLUE, HIGH);
  delay(DELAY);
  digitalWrite(RED, LOW);

  //Light blue
  digitalWrite(GREEN, HIGH);
  delay(DELAY);

  //White
  digitalWrite(RED, HIGH);
  delay(DELAY);
  digitalWrite(BLUE, LOW);
}
