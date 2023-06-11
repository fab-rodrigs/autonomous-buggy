#include "Ultrassom.h"
#include "Motores.h"
#include "Encoder.h"
#include <math.h>
#include <SoftwareSerial.h>

#define tempo 10

SoftwareSerial bluetooth(2, 3);
Ultrassom ultrassom_1(Ultrassom::ECHO1, Ultrassom::TRIG1);
Motores motores;
Encoder encoder_0 (Encoder::D0);
Encoder encoder_1 (Encoder::D1);
int tempo, distancia, pulsos_encoder0, pulsos_encoder1, dadoBluetooth = 0;
float trajeto_encoder0 = 0, trajeto_encoder1 = 0;
Linha seguidor_linha;

void setup() {
   Serial.begin(9600);
}

void loop() {

  // /* ---> BLUETOOTH <---
  if(bluetooth.available()){
       dadoBluetooth = bluetooth.read();
       Serial.println(dadoBluetooth);
  }
  // */

  // /* ---> ULTRASSONICO E ENCODER <---
  encoder_0.atualizar();
  encoder_1.atualizar();
  ultrassom_1.atualizar();

  tempo = ultrassom_1.obter_distancia();
  distancia = (tempo/2) * 0.0343;

  // fazer contador para printar em tempo diferente
  Serial.print("Distancia = ");
  Serial.println(distancia);
  Serial.print("\n");

  if (distancia > 30){
    motores.frente(100);
    pulsos_encoder0 = encoder_0.obter_pulsos();
    trajeto_encoder0 += (pulsos_encoder0/20) * 0.225; // A cada 20 pulsos, da 1 volta. 22.5 cm de circunferência. 
    pulsos_encoder1 = encoder_1.obter_pulsos();
    trajeto_encoder1 += (pulsos_encoder1/20) * 0.225;
  }
  else{
    motores.direita(200);
    pulsos_encoder0 = -encoder_0.obter_pulsos(); // lado direito
    trajeto_encoder0 += (pulsos_encoder0/20) * 0.225;
    pulsos_encoder1 = encoder_1.obter_pulsos(); // lado esquerdo
    trajeto_encoder1 += (pulsos_encoder1/20) * 0.225;
  }
  
  Serial.print("Encoder0 = "); // lado direito
  Serial.println(encoder_0.obter_pulsos());
  Serial.print("Encoder1 = "); // lado esquerdo
  Serial.println(encoder_1.obter_pulsos());
  Serial.print("\n");
  
  Serial.print("Trajeto Encoder0 = ");
  Serial.println(trajeto_encoder0);
  Serial.print("Trajeto Encoder1 = ");
  Serial.println(trajeto_encoder1);
  Serial.print("\n"); 
  // */
  
  // /* ---> POSIÇÂO X e Y <---
  float posicao_x = 0;
  float posicao_y = 0;

  v1 = trajeto_encoder0 / tempo;
  v2 = trajeto_encoder1 / tempo;

  float v_linear = v1 + v2;
  float orientacao = v2 - v1;
  
  float delta_x = v_linear * tempo;
  float delta_y = orientacao * tempo;
  
  posicao_x += delta_x;
  posicao_y += delta_y;

  Serial.print("Posicao X = ");
  Serial.println(posicao_x);
  Serial.print("Posicao Y = ");
  Serial.println(posicao_y);
  Serial.print("\n");
  // */

  // /* ---> SEGUIDOR DE LINHA <---
  //      preto = 1   branco = 0
  if(seguidor_linha.obter_esquerda() == 1 && seguidor_linha.obter_direita() == 1)
    motores.parar();
  else if(seguidor_linha.obter_esquerda() == 0 && seguidor_linha.obter_direita() == 0)
    motores.frente(60);
  else if(seguidor_linha.obter_esquerda() == 0)
    motores.esquerda(60);
  else if(seguidor_linha.obter_direita() == 0)
    motores.direita(60);
 
  Serial.print("Linha esquerda = ");
  Serial.println(seguidor_linha.obter_esquerda());
  Serial.print("Linha direita = ");
  Serial.println(seguidor_linha.obter_direita());
  Serial.print("\n");
  // */
  
  delay(tempo);
}
