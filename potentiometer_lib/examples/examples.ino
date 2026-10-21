
#include "potentiometer.h"
#define POTENTIOMETER_PIN A0
Potentiometer poten = Potentiometer(POTENTIOMETER_PIN);  // Define o poten como do tipo potentiometer 
int i = 0;

void setup() {
  Serial.begin(9600);  // Inicializa o Serial Begin para monitoramento
}

void espacoEsquerda(String texto, int largura) {
  int tamanho = texto.length();
  int totalEspacos = largura - tamanho;
  int espacosEsquerda = totalEspacos / 2;
  // Adiciona o preenchimento à esquerda
  for (int i = 0; i < espacosEsquerda; i++) {
    Serial.print(" ");
  }
}

void espacoDireita(String texto, int largura){
  int tamanho = texto.length();
  int totalEspacos = largura - tamanho;
  int espacosDireita = totalEspacos - (totalEspacos / 2);
  for (int i = 0; i <= espacosDireita; i++) {
    Serial.print(" ");
  }
}

void loop() {
  if (i == 0){
    Serial.println("----------------------------------------------------------------------------------------------------------------------------");
    Serial.println("Leitura do potênciometro - Leitura da Tensão - Leitura considerando intervalo dado - Leitura de tensão considerando a Serial");
    Serial.println("----------------------------------------------------------------------------------------------------------------------------");
    i = 1;
  }
  
  espacoEsquerda(String(poten.read()), 25);
  Serial.print(poten.read());         // Realiza a leitura e apresenta na tela o valor lido (0-1023).
  espacoDireita(String(poten.read()), 25);
  
  espacoEsquerda(String(poten.readTension()), 19);
  Serial.print(poten.readTension());  // Realiza a leitura do potenciometro e converte para a tensão associada (0-5).
  float tensaoParametro = pegarUltimoElemento();
  espacoDireita(String(poten.readTension()), 19);

  espacoEsquerda(String(poten.readInRange()), 37);
  Serial.print(poten.readInRange());  // Realiza a leitura de acordo a um intervalo, caso não digite um valor o default é de 0 a 255.
  espacoDireita(String(poten.readInRange()), 37);
  
  espacoEsquerda(String(poten.read()),25);      
  poten.readSerial(Serial);
}
