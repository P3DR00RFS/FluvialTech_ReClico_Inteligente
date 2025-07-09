#include <Servo.h>

// --- Definições dos pinos dos servos ---
#define PINO_BASE     2
#define PINO_OMBRO    3
#define PINO_COTOVELO 4
#define PINO_GARRA    5

// --- Sensores ---
#define SENSOR_UMIDADE A0
#define TRIG_PIN       6
#define ECHO_PIN       7

// --- LEDs ---
#define LED_VERMELHO   8
#define LED_VERDE      9

// --- Limites ---
#define LIMITE_UMIDADE 500
#define DISTANCIA_MINIMA 20  // centímetros

// --- Objetos Servo ---
Servo servoBase;
Servo servoOmbro;
Servo servoCotovelo;
Servo servoGarra;

// --- Posições dos servos ---
#define GARRA_ABERTA    90
#define GARRA_FECHADA   20

#define ALTURA_BAIXA    70
#define ALTURA_ALTA     20

#define BASE_INICIAL    90
#define BASE_VIRADA     180

#define COTOVELO_PADRAO 90

void setup() {
  // Inicializa os servos
  servoBase.attach(PINO_BASE);
  servoOmbro.attach(PINO_OMBRO);
  servoCotovelo.attach(PINO_COTOVELO);
  servoGarra.attach(PINO_GARRA);

  // LEDs
  pinMode(LED_VERMELHO, OUTPUT);
  pinMode(LED_VERDE, OUTPUT);

  // Sensor de distância
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Monitor serial
  Serial.begin(9600);

  // Posição inicial
  servoBase.write(BASE_INICIAL);
  servoOmbro.write(ALTURA_BAIXA);
  servoCotovelo.write(COTOVELO_PADRAO);
  servoGarra.write(GARRA_ABERTA);

  delay(1000);
}

void loop() {
  int umidade = analogRead(SENSOR_UMIDADE);
  Serial.print("Umidade: ");
  Serial.println(umidade);

  long distancia = medirDistancia();
  Serial.print("Distância: ");
  Serial.print(distancia);
  Serial.println(" cm");

  if (umidade < LIMITE_UMIDADE) {
    // Sem água: LED vermelho desligado
    digitalWrite(LED_VERMELHO, LOW);
    return;
  } else {
    // Com água: LED vermelho ligado
    digitalWrite(LED_VERMELHO, HIGH);
  }

  if (distancia > 0 && distancia < DISTANCIA_MINIMA) {
    digitalWrite(LED_VERMELHO, LOW);
    digitalWrite(LED_VERDE, HIGH);

    executarSequencia();

    digitalWrite(LED_VERDE, LOW);
    digitalWrite(LED_VERMELHO, HIGH);

    delay(3000); // Pequena pausa para evitar repetições rápidas
  }

  delay(500);
}

// --- Medição com o HC-SR04 ---
long medirDistancia() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duracao = pulseIn(ECHO_PIN, HIGH);
  long distancia = duracao * 0.034 / 2;

  return distancia;
}

// --- Sequência do braço ---
void executarSequencia() {
  Serial.println("Executando sequência...");

  servoGarra.write(GARRA_FECHADA);
  delay(700);

  servoOmbro.write(ALTURA_ALTA);
  delay(700);

  servoBase.write(BASE_VIRADA);
  delay(700);

  servoOmbro.write(ALTURA_BAIXA);
  delay(700);

  servoGarra.write(GARRA_ABERTA);
  delay(700);

  servoOmbro.write(ALTURA_ALTA);
  delay(700);

  servoBase.write(BASE_INICIAL);
  delay(700);

  servoOmbro.write(ALTURA_BAIXA);
  delay(700);

  Serial.println("Sequência finalizada.");
}
