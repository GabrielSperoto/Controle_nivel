#include <Arduino.h>

#define LED_VERMELHO 4
#define LED_VERDE 3
#define LED_AMARELO 2
#define LIGA 5
#define DESLIGA 6
#define POTENCIOMETRO A0

const float nivel_maximo = 30;
const float nivel_alto = 25;
const float nivel_baixo = 10;

void setup(){
    pinMode(LED_VERMELHO,OUTPUT);
    pinMode(LED_VERDE,OUTPUT);
    pinMode(LED_AMARELO,OUTPUT);

    pinMode(LIGA,INPUT);
    pinMode(DESLIGA,INPUT);
    pinMode(POTENCIOMETRO,INPUT);

    Serial.begin(9600);
}

void loop(){
    float valor_sensor = map(analogRead(POTENCIOMETRO),0,1023,0,30);
    float nivel_atual = nivel_maximo - valor_sensor;

    Serial.print("Nivel atual: ");
    Serial.println(nivel_atual);

    if(nivel_atual <= nivel_baixo){
        digitalWrite(LED_VERMELHO,HIGH);
        digitalWrite(LED_VERDE,LOW);
        digitalWrite(LED_AMARELO,LOW);
    } else if(nivel_atual >= nivel_alto){
        digitalWrite(LED_VERMELHO,LOW);
        digitalWrite(LED_VERDE,HIGH);
        digitalWrite(LED_AMARELO,LOW);
    }
}
