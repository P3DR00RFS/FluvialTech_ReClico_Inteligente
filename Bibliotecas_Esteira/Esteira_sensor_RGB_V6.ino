#include <Wire.h>
#include "Adafruit_TCS34725.h"
#include <LiquidCrystal_I2C.h>
#include <Servo.h>


// Inicializa o objeto LiquidCrystal_I2C
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Define os pinos dos servo motores
const int servoPin1 = 12;
const int servoPin2 = 13;
const int motorPin = 6;  
int total = 0;
int amarelo = 0;
int azul = 0;
int roxo = 0;

// Cria objetos Servo
Servo servo1;
Servo servo2;

/* Inicializa com os valores padrÃµes(int time = 2.4ms, gain = 1x) */
// Adafruit_TCS34725 tcs = Adafruit_TCS34725();

/* Initialise with specific int time and gain values */
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_154MS, TCS34725_GAIN_1X);

// Define a velocidade do motor (0 a 255)
  int velocidade = 190;


void setup() 
{
  Serial.begin(9600);
  pinMode(motorPin, OUTPUT);

  if (tcs.begin())//Se conseguirmos iniciar o sensor significa que ele esta conectado 
  {
    Serial.println("Sensor encontrado");
  } 
  else//caso contrario ele nao estao conectado 
  {
    Serial.println("Sensor nao encontrado, cheque suas conexoes.");
    while (1);
  }
  // Inicializa o display LCD
     lcd.init();
  
  // Ativa o backlight (iluminação) do display
  lcd.backlight();
  
   // Limpa o display
  lcd.clear();
  
  // Imprime a palavra "Casa" no display
  lcd.setCursor(0, 0);
  lcd.print("  USINAINFO  ");
 
  delay(5000);
  lcd.clear();
  
  
  // Anexa os objetos Servo aos pinos
  servo1.attach(servoPin1);
  servo2.attach(servoPin2);

  // Posiciona os servo motores
  servo1.write(0); //Servo 1 aste elevada
  servo2.write(90);//Servo 2 aste elevada
    
 
}

void loop() 
{
  lcd.setCursor(0, 0);
  lcd.print(" AGUARDANDO...       ");
  lcd.setCursor(0, 1);
  lcd.print("   TOTAL=");
  lcd.print(total);
  lcd.print("       ");
  
 analogWrite(motorPin, velocidade);
  
 
 
  uint16_t r, g, b, c;

  tcs.getRawData(&r, &g, &b, &c);

  // Imprime na serial os valores das cores
  Serial.print("Vermelho: "); Serial.print(r, DEC); Serial.print(" ");
  Serial.print("Verde: "); Serial.print(g, DEC); Serial.print(" ");
  Serial.print("Azul: "); Serial.print(b, DEC); Serial.print(" ");
  Serial.print("Claridade: "); Serial.print(c, DEC); Serial.print(" ");
  Serial.println(" ");
  


// Configuração MANUAL dos valores para reconhecimento de objetos especificos

 // Configuração do reconhecimento da cor ROXA    
  if (c > 150 && r > 100 && r < 170 && g > 60 && g < 130 && b > 60 && b < 130) {
    total++;
    lcd.setCursor(0, 1);
    lcd.print("   TOTAL=");
    lcd.print(total);
    lcd.print("       ");
    
    roxo++;
    lcd.setCursor(0, 0);
    lcd.print("   ROXO=");
    lcd.print(roxo);
    lcd.print("       ");
    servo1.write(90);//Servo 1 aste baixada
    delay(6000);
    servo1.write(0);//Servo 1 aste elevada 
    // lcd.clear();   
    Serial.println("Cor: ROXA");
  }

 // Configuração do reconhecimento da cor AMARELA
  if (c > 150 && r > 340 && r < 500 && g > 200 && g < 280 && b > 90 && b < 200) {
    total++;
    lcd.setCursor(0, 1);
    lcd.print("   TOTAL=");
    lcd.print(total);
    lcd.print("       ");
    
    amarelo++;
    lcd.setCursor(0, 0);
    lcd.print("  AMARELO=");
    lcd.print(amarelo);
    lcd.print("       ");
    servo2.write(0);//Servo 2 aste baixada
    delay(6500);
    servo2.write(90);//Servo 2 aste elevada
    Serial.println("Cor: AMARELO");
  }

// Configuração do reconhecimento da cor AZUL
  if (c > 150 && r > 65 && r < 180 && g > 130 && g < 230 && b > 110 && b < 200) {
    total++;
    lcd.setCursor(0, 1);
    lcd.print("   TOTAL=");
    lcd.print(total);
    lcd.print("       ");
    
    azul++;
    lcd.setCursor(0, 0);
    lcd.print("   AZUL=");
    lcd.print(azul);
    lcd.print("       ");
    Serial.println("Cor: AZUL");
    delay(3000);
    //lcd.clear();
  }
 
  

}
