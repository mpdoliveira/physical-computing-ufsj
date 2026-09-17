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
  //Karina's Lilac
  analogWrite(RED, 200);
  analogWrite(GREEN, 50);
  analogWrite(BLUE, 200);
  delay(DELAY);

  //Marcos' Yellow
  analogWrite(RED, 255);
  analogWrite(GREEN, 150);
  analogWrite(BLUE, 15);
  delay(DELAY);

  //Group's Lime-blue
  analogWrite(RED, 0);
  analogWrite(GREEN, 255);
  analogWrite(BLUE, 60);
  delay(DELAY);

  //Someone's Brown????
  analogWrite(RED, 165);
  analogWrite(GREEN, 100);
  analogWrite(BLUE, 42);
  delay(DELAY);
  
  //God's Orange
  analogWrite(RED, 255);
  analogWrite(GREEN, 100);
  analogWrite(BLUE, 7);
  delay(DELAY);

}
