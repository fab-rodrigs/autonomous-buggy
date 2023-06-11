#define pinMot1A 6 // Horário       (motores da DIREITA)
#define pinMot1B 5 // Anti-Horário
#define pinMot2A 11 // Horário      (motores da ESQUERDA)
#define pinMot2B 3 // Anti-Horário 

void printf(int pwm1, int pwm2, int pwm3, int pwm4){
  Serial.println("pwm1A = ");
  Serial.println(pwm1);
  Serial.println("pwm1B = ");
  Serial.println(pwm2);
  Serial.println("pwm2A = ");
  Serial.println(pwm3);
  Serial.println("pwm2B = ");
  Serial.println(pwm4);
  delay(1000000);
}

void parar(){
  analogWrite(pinMot1A, 0);
  analogWrite(pinMot1B, 0);
  analogWrite(pinMot2A, 0);
  analogWrite(pinMot2B, 0);
  printf(0, 0, 0, 0);
}

void frente(int pwm1, int pwm3){
  analogWrite(pinMot1A, pwm1);
  analogWrite(pinMot1B, 0);                           
  analogWrite(pinMot2A, pwm3);
  analogWrite(pinMot2B, 0);
  printf(pwm1, 0, pwm3, 0);
}

void tras(int pwm2, int pwm4){
  analogWrite(pinMot1A, 0);
  analogWrite(pinMot1B, pwm2);
  analogWrite(pinMot2A, 0);
  analogWrite(pinMot2B, pwm4);
  printf(0, pwm2, 0, pwm4);
}

void direita(int pwm2, int pwm3){
  analogWrite(pinMot1A, 0);
  analogWrite(pinMot1B, pwm2);
  analogWrite(pinMot2A, pwm3);
  analogWrite(pinMot2B, 0);
  printf(0, pwm2, pwm3, 0);
}

void esquerda(int pwm1, int pwm4){
  analogWrite(pinMot1A, pwm1);
  analogWrite(pinMot1B, 0);
  analogWrite(pinMot2A, 0); 
  analogWrite(pinMot2B, pwm4);
  printf(pwm1, 0, 0, pwm4);
}

void setup()
{
 pinMode(pinMot1A, OUTPUT);
 pinMode(pinMot1B, OUTPUT);
 pinMode(pinMot2A, OUTPUT);
 pinMode(pinMot2B, OUTPUT);
 Serial.begin(9600);
}

void loop()
{
    int pwmV1, pwmV2;
    float v, v1, v2, w, l = 0.0675;

    v = 0.5;
    w = 0.34;

    if(v > 1.063)
      v = 1.063;
    else if(v < -1.063)
      v = -1.063;
  
    v1 = v + l*w;
    v2 = v - l*w;

    pwmV1 = v1 * (255/1.063);
    pwmV2 = v2 * (255/1.063);

    if(pwmV1 >= 255 || pwmV1 <= -255)
        pwmV1 = 255;
    else if(pwmV1 < 0)
        pwmV1 = -pwmV1;

    if(pwmV2 >= 255 || pwmV2 <= -255)
        pwmV2 = 255;
    else if(pwmV2 < 0)
        pwmV2 = -pwmV2;

    if (v==0 && w==0)
        parar();

    else if (v==0 || w==0){
        if (v>0 && w==0)
            frente(pwmV1, pwmV2);
        else if (v<0 && w==0)
            tras(pwmV1, pwmV2);
        else if (v==0 && w>0)
            esquerda(pwmV1, pwmV2);
        else if (v==0 && w<0)
            direita(pwmV1, pwmV2);
    }

    else if (v!=0 && w!=0){
        if (v>0 && w>0)
            frente(pwmV1, pwmV2);
        else if (v>0 && w<0)
            frente(pwmV1, pwmV2);
        else if (v<0 && w>0)
            tras(pwmV1, pwmV2);
        else if (v<0 && w<0)
            tras(pwmV1, pwmV2);
    }
}