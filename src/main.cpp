//codigo de teste

#include <Arduino.h>
#include <HCSR04.h>

#define TRIGGER 2
#define ECHO 3
#define BOMBA 10



const int LED_VERDE = 5; //acende quando a bomba estiver desligada
const int LED_VERMELHO = 6;//acende quando a bomba estiver ligada
const int LED_AMARELO = 7;//acende quando o botão de emergência for pressionado
const int LIGA = 8;
const int DESLIGA = 9;

float nivel_tanque = 38; // capacidade maxima do tanque
float nivel_baixo = 5; // nivel minimo do tanque;
float nivel_alto = 10; //nivel maximo do tanque
float nivel_alarme = 8;
bool flag = false;
float nivel;

void ligaPinos(bool bomba, bool verm, bool verd, bool amarelo){
  digitalWrite(BOMBA,bomba);
  digitalWrite(LED_VERMELHO,verm);
  digitalWrite(LED_VERDE,verd);
  digitalWrite(LED_AMARELO,amarelo);
}

UltraSonicDistanceSensor distanceSensor(TRIGGER,ECHO); //função para calcular a distância entre o nível da água e o sensor

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_VERDE,OUTPUT);
  pinMode(LED_VERMELHO,OUTPUT);
  pinMode(LED_AMARELO,OUTPUT);
  pinMode(TRIGGER,OUTPUT);
  pinMode(BOMBA,OUTPUT);

  pinMode(ECHO,INPUT);
  pinMode(LIGA,INPUT);
  pinMode(DESLIGA,INPUT);

  ligaPinos(1,0,0,0);

  Serial.begin(9600);

}
void loop() {
  nivel = nivel_tanque - distanceSensor.measureDistanceCm();
  Serial.print("Nivel: ");
  Serial.println(nivel);
  Serial.print("Medicao do sensor: ");
  Serial.println(distanceSensor.measureDistanceCm());

  if(digitalRead(DESLIGA)){
    flag = false;
    ligaPinos(1,0,0,0);
    Serial.println("DESLIGOU");
  } else {
    if(digitalRead(LIGA) || flag){
      flag = true;
      if(nivel >= nivel_alto){
        ligaPinos(1,0,1,0);
      } else{
        if(nivel >= nivel_alarme){
          ligaPinos(0,1,0,1);
          delay(200);
          ligaPinos(0,1,0,0);
          delay(200);
        } else{
          ligaPinos(0,1,0,0);
        }
      }
    }
  }
}