#include <Arduino.h>
#include <HCSR04.h>

#define TRIGGER 2
#define ECHO 3
#define BOMBA 10
#define SENSOR_TEMP 11


const int LED_VERDE = 5; //acende quando a bomba estiver desligada
const int LED_VERMELHO = 6;//acende quando a bomba estiver ligada
const int LED_AMARELO = 7;//acende quando o botão de emergência for pressionado
const int BUZZER = 8;//soa quando o botão de emergência for pressionado
const int LIGA = 8;
const int DESLIGA = 9;

float nivel_tanque = 0; // capacidade maxima do tanque
float nivel_baixo = 0; // nivel minimo do tanque;
float nivel_alto = 0; //nivel maximo do tanque
bool flag;

//xxxxx

UltraSonicDistanceSensor distanceSensor(TRIGGER,ECHO); //função para calcular a distância entre o nível da água e o sensor

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_VERDE,OUTPUT);
  pinMode(LED_VERMELHO,OUTPUT);
  pinMode(LED_AMARELO,OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(TRIGGER,OUTPUT);
  pinMode(BOMBA,OUTPUT);

  pinMode(ECHO,INPUT);
  pinMode(SENSOR_TEMP,INPUT);
  pinMode(LIGA,INPUT);
  pinMode(DESLIGA,INPUT);

  Serial.begin(9600);

}
void loop() {

    if(digitalRead(DESLIGA)){
      flag = false;
    }

    // obtem o nível atual da água no tanque
    int nivel = nivel_tanque - distanceSensor.measureDistanceCm();

    //caso o nível da água esteja baixo e flag seja true ou o botão de ligar seja pressionado, ligar a bomba
    if((nivel < nivel_baixo) && (digitalRead(LIGA) || flag)){
      //liga o sistema quando o nível está baixo
      flag = true;
      digitalWrite(BOMBA,HIGH);
      digitalWrite(LED_VERMELHO,HIGH);
      digitalWrite(LED_VERDE,LOW);
      digitalWrite(LED_AMARELO,LOW);
    }

    else if((nivel >= nivel_alto) && (digitalRead(LIGA) || flag)){
      //desliga o sistema caso o nível da água esteja alto
      digitalWrite(BOMBA,LOW);
      digitalWrite(LED_VERDE,HIGH);
      digitalWrite(LED_VERMELHO,LOW);
      digitalWrite(LED_AMARELO,LOW);
    }

    else if(digitalRead(DESLIGA)){
      flag = false;
    }

    else{
      digitalWrite(BOMBA,LOW);
      digitalWrite(LED_VERDE,LOW);
      digitalWrite(LED_VERMELHO,LOW);
      digitalWrite(LED_AMARELO,LOW);
     }
    


}