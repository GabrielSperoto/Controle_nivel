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

void ligarPinos(bool bomba, bool led_verm, bool led_verd, bool led_amar) {
  digitalWrite(BOMBA, bomba);
  digitalWrite(LED_VERDE, led_verd);
  digitalWrite(LED_VERMELHO, led_verm);
  digitalWrite(LED_AMARELO, led_amar);
}

void pisca_amarelo(){
  digitalWrite(LED_AMARELO,HIGH);
  delay(200);
  digitalWrite(LED_AMARELO,LOW);
  delay(200);
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

  float nivel = nivel_tanque - distanceSensor.measureDistanceCm();
  Serial.print("Nivel: ");
  Serial.println(nivel);
  Serial.println(distanceSensor.measureDistanceCm());
  delay(200);

  // obtem o nível atual da água no tanque
  
  if(digitalRead(LIGA)){
    Serial.println("BOTAO FUNCIONOU. GRACAS A DEUS");
  }

  if(digitalRead(DESLIGA)){
    flag = false;
    ligarPinos(1,0,0,0);
  }

  else if((nivel >= nivel_alto)  && (digitalRead(LIGA) || flag)){
    //desliga o sistema caso o nível da água esteja alto
    flag = true;
    ligarPinos(1,0,1,0);
    Serial.println("ENTROU ALTO!");
  }
  //caso o nível da água esteja baixo e flag seja true ou o botão de ligar seja pressionado, ligar a bomba
  else if((digitalRead(LIGA) || flag)){
    //liga o sistema quando o nível está baixo
    if(nivel >= nivel_amarelo){
      ligarPinos(0,1,0,0);
      pisca_amarelo();

    } else{
      ligarPinos(0,1,0,0);
    }
    flag = true;
    Serial.println("ENTROU!");
  }

}