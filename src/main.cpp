#include <Arduino.h>
#include <HCSR04.h>

#define TRIGGER 2
#define ECHO 3
#define BOMBA 12
#define SENSOR_TEMP 11


const int LED_VERDE = 6; //acende quando a bomba estiver desligada
const int LED_VERMELHO = 5;//acende quando a bomba estiver ligada
const int LED_AMARELO = 7;//acende quando o botão de emergência for pressionado

const int LIGA = 9;
const int DESLIGA = 10;

float nivel_tanque = 38.7; // capacidade maxima do tanque
float nivel_baixo = 5; // nivel minimo do tanque;
float nivel_amarelo = 6; // nivel para ligar o amarelo
float nivel_alto = 8; //nivel maximo do tanque
bool flag = false;
bool flag2;
float nivel;

void ligarPinos(bool bomba, bool led_verm, bool led_verd, bool led_amar) {
  digitalWrite(BOMBA, bomba);
  digitalWrite(LED_VERDE, led_verd);
  digitalWrite(LED_VERMELHO, led_verm);
  digitalWrite(LED_AMARELO, led_amar);
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

  digitalWrite(BOMBA,HIGH);
  digitalWrite(LED_VERMELHO,LOW);
  digitalWrite(LED_VERDE,LOW);
  digitalWrite(LED_AMARELO,LOW);

  Serial.begin(9600);

}
void loop() {

  nivel = nivel_tanque - distanceSensor.measureDistanceCm();
  Serial.print("Nivel: ");
  Serial.println(nivel);
  Serial.print("Medida sensor: ");
  Serial.println(distanceSensor.measureDistanceCm());
  delay(200);

  if(digitalRead(DESLIGA)){
    flag = false;
    flag2 = false;
    ligarPinos(1,0,0,0);
  }

  else if((nivel >= nivel_alto)  && (digitalRead(LIGA) || flag)){
    flag = true;
    flag2 = false;
    ligarPinos(1,0,1,0);
  }

  else if((digitalRead(LIGA) || flag) && (nivel <= nivel_baixo || flag2)){
    if(nivel >= nivel_amarelo){
      ligarPinos(0,1,0,1);
      delay(200);
      ligarPinos(0,1,0,0);
      delay(200);

    } else{
      ligarPinos(0,1,0,0);
    }
    flag = true;
    flag2 = true;
  }

  delay(500);

}