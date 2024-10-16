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
bool estado_bomba;
bool shutdown;

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
  //caso não haja parada total, execute o código normalmente
  if (not shutdown){

    if(digitalRead(DESLIGA)){
      flag = false;
    }

    // obtem o nível atual da água no tanque
    int nivel = nivel_tanque - distanceSensor.measureDistanceCm();

    //caso o nível da água esteja baixo e flag seja true ou o botão de ligar seja pressionado, ligar a bomba
    if((nivel < nivel_baixo) && (digitalRead(LIGA) || flag)){
      //liga o sistema quando o nível está baixo
      estado_bomba = true;
      flag = true;
      digitalWrite(BOMBA,HIGH);
      digitalWrite(LED_VERMELHO,HIGH);
      digitalWrite(LED_VERDE,LOW);
      digitalWrite(LED_AMARELO,LOW);
    }

    else if((nivel >= nivel_alto) && (digitalRead(LIGA) || flag)){
      //desliga o sistema caso o nível da água esteja alto
      estado_bomba = false;
      flag = false;
      digitalWrite(BOMBA,LOW);
      digitalWrite(LED_VERDE,HIGH);
      digitalWrite(LED_VERMELHO,LOW);
      digitalWrite(LED_AMARELO,LOW);
    }

    else{
      digitalWrite(BOMBA,LOW);
      digitalWrite(LED_VERDE,LOW);
      digitalWrite(LED_VERMELHO,LOW);
      digitalWrite(LED_AMARELO,LOW);

    //parar tudo caso haja alguma anormalidade no funcionamento da bomba
    if ((nivel < nivel_baixo && estado_bomba == false) || (nivel >= nivel_alto && estado_bomba == true)){ 
      digitalWrite(BOMBA,LOW);
      digitalWrite(LED_VERDE,LOW);
      digitalWrite(LED_VERMELHO,LOW);
      digitalWrite(LED_AMARELO,LOW);
      shutdown = true;
    }
     }
      }
  //No caso de shutdown == true, acenda o led amarelo e faça-o piscar
  else{
      digitalWrite(LED_AMARELO, HIGH);
      delay(400);
      digitalWrite(LED_AMARELO, LOW);
      delay(400);
      
      //caso o botão de ligar seja acionado, shutdown será dado como false, reiniciando o sistema
      if (digitalRead(LIGA)){
        shutdown = false;
      }
      //caso o botão de desligar seja acionado, desligar todo o sistema de vez
      else if (digitalRead(DESLIGA)){
        digitalWrite(BOMBA,LOW);
        digitalWrite(LED_VERDE,LOW);
        digitalWrite(LED_VERMELHO,LOW);
        digitalWrite(LED_AMARELO,LOW);
      }
    }


}