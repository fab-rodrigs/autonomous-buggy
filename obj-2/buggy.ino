#include "Ultrassom.h"
#include "Motores.h"
#include "Encoder.h"
#include <math.h>
//#include <SoftwareSerial.h>

//SoftwareSerial bluetooth(2, 3);

Ultrassom ultrassom_1(Ultrassom::ECHO1, Ultrassom::TRIG1);
Motores motores;
Encoder encoder_0 (Encoder::D0);
Encoder encoder_1 (Encoder::D1);
int tempo, distancia, pulsos_encoder0, pulsos_encoder1; // dadoBluetooth = 0;
float trajeto_encoder0 = 0, trajeto_encoder1 = 0;

void setup() {
   Serial.begin(9600);
}

void loop() {
  //if(bluetooth.available()){
  //     dadoBluetooth = bluetooth.read();
  //     Serial.println(dadoBluetooth);
  //}
  
  encoder_0.atualizar();
  encoder_1.atualizar();
  ultrassom_1.atualizar();

  tempo = ultrassom_1.obter_distancia();
  distancia = (tempo/2) * 0.0343;

  Serial.print("Distancia = ");
  Serial.println(distancia);
  Serial.print("\n");

  if (distancia > 30){
    motores.frente(100);
    pulsos_encoder0 = encoder_0.obter_pulsos();
    trajeto_encoder0 += (pulsos_encoder0/20) * 0.225; // 22.5 cm de circunferência
    pulsos_encoder1 = encoder_1.obter_pulsos();
    trajeto_encoder1 += (pulsos_encoder1/20) * 0.225;
  }
  else{
    motores.direita(200);
    pulsos_encoder0 = encoder_0.obter_pulsos(); // lado direito
    trajeto_encoder0 += (pulsos_encoder0/20) * 0.225;
    pulsos_encoder1 = -encoder_1.obter_pulsos(); // lado esquerdo
    trajeto_encoder1 += (pulsos_encoder1/20) * 0.225;
  }
  
  // a cada 20 pulsos, da 1 volta. 6 cm de diametro, logo pi * 6 = 19 cm de circunferência. Então a cada 20 pulsos, 1 volta, anda 19 cm.
    
  Serial.print("Trajeto Encoder0 = ");
  Serial.println(trajeto_encoder0);
  Serial.print("Trajeto Encoder1 = ");
  Serial.println(trajeto_encoder1);
  Serial.print("\n"); 
  
  Serial.print("Encoder0 = "); // lado direito
  Serial.println(encoder_0.obter_pulsos());
  
  Serial.print("Encoder1 = "); // lado esquerdo
  Serial.println(encoder_1.obter_pulsos());
  Serial.print("\n");

  int delta_angulo = (trajeto_encoder0 - trajeto_encoder1) / (2*0.0675);
  float delta_x = (trajeto_encoder0 + trajeto_encoder1) / (2 * cos(radians(delta_angulo))); 
  float delta_y = (trajeto_encoder0 + trajeto_encoder1) / (2 * sin(radians(delta_angulo)));
  
  Serial.print("Delta_X = ");
  Serial.println(delta_x);
  Serial.print("Delta_Y = ");
  Serial.println(delta_y);
  Serial.print("Delta_Angulo = ");
  Serial.println(delta_angulo);
  Serial.print("\n");
   
  delay(1000);
}
