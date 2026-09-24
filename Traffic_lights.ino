//Saidas
#define BLUE 9
#define GREEN 10
#define RED 11

//Config
#define DELAY 100

void setup() {
  pinMode(BLUE, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(RED, OUTPUT);
}

void loop() {
  digitalWrite(RED, HIGH);
  delay(1000);
  
  analogWrite(GREEN, 100);
  delay(2000);
  digitalWrite(RED, LOW);
  
  digitalWrite(GREEN, HIGH);
  delay(8000);
  digitalWrite(GREEN, LOW);
}
