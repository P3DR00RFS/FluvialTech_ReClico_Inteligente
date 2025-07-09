#include "HX711.h"

// === HX711 ===
#define DT A1
#define SCK A0
HX711 balanca;

// === Motores ===
#define ENA 10 // Velocidade - Motor esquerdo
#define IN1 4
#define IN2 5

#define ENB 11 // Velocidade - Motor direito
#define IN3 6
#define IN4 7

// === Sensor Ultrassônico ===
#define TRIG 8
#define ECHO 9

// === Parâmetros ===
#define PESO_LIMITE 10000
#define DISTANCIA_DESTINO 20 // cm
#define VELOCIDADE_ESQUERDA 180 // 0-255 (ajuste se desalinhar)
#define VELOCIDADE_DIREITA 180

void setup() {
  Serial.begin(9600);

  // Inicializa HX711
  balanca.begin(DT, SCK);
  balanca.set_scale();
  balanca.tare();

  // Motores
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  // Ultrassônico
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  Serial.println("Sistema pronto. Aguardando peso...");
}

void loop() {
  long peso = balanca.get_units(5);
  Serial.print("Peso detectado: ");
  Serial.println(peso);

  if (peso > PESO_LIMITE) {
    Serial.println("Peso suficiente. Esperando 3 segundos...");
    delay(3000);

    // Anda para frente até detectar o destino
    Serial.println("Avançando...");
    while (medirDistancia() > DISTANCIA_DESTINO) {
      andarFrente();
    }
    pararMotores();

    Serial.println("Chegou ao destino. Esperando 10 segundos...");
    delay(10000);

    // Anda de ré por tempo estimado (ou outro sensor se preferir)
    Serial.println("Voltando...");
    andarRe();
    delay(3000); // Ajuste o tempo de retorno conforme distância real
    pararMotores();

    Serial.println("Retornou ao ponto inicial.");
    Serial.println("------------------------------");

    delay(3000); // Espera antes de próxima detecção
  }

  delay(1000);
}

// === Controle de movimento ===
void andarFrente() {
  analogWrite(ENA, VELOCIDADE_ESQUERDA);
  analogWrite(ENB, VELOCIDADE_DIREITA);

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void andarRe() {
  analogWrite(ENA, VELOCIDADE_ESQUERDA);
  analogWrite(ENB, VELOCIDADE_DIREITA);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void pararMotores() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

// === Medição com HC-SR04 ===
long medirDistancia() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  long duracao = pulseIn(ECHO, HIGH);
  long distancia = duracao * 0.034 / 2;

  return distancia;
}
