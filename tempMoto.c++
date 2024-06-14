#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Define pinos
#define DHTPIN 2
#define DHTTYPE DHT22

// Inicializa o sensor DHT
DHT dht(DHTPIN, DHTTYPE);

// Inicializa o display LCD 20x4
LiquidCrystal_I2C lcd(0x27, 20, 4);

// Define pinos dos LEDs e do buzzer
const int ledVerde = 4;
const int ledAmarelo = 5;
const int ledVermelho = 6;
const int buzzer = 7;

// Define caracteres personalizados
byte skull[8] = {
  0b00000,
  0b01110,
  0b10101,
  0b11111,
  0b11111,
  0b01110,
  0b01010,
  0b00000
};

byte warning[8] = {
  0b00100,
  0b01110,
  0b10101,
  0b10101,
  0b11111,
  0b11111,
  0b00100,
  0b00000
};

void setup() {
  // Inicializa o monitor serial
  Serial.begin(9600);

  // Inicializa o sensor DHT
  dht.begin();

  // Inicializa o display LCD
  lcd.init();
  lcd.backlight();

  // Define os pinos dos LEDs e do buzzer como saída
  pinMode(ledVerde, OUTPUT);
  pinMode(ledAmarelo, OUTPUT);
  pinMode(ledVermelho, OUTPUT);
  pinMode(buzzer, OUTPUT);

  // Inicia o estado dos LEDs e do buzzer
  digitalWrite(ledVerde, LOW);
  digitalWrite(ledAmarelo, LOW);
  digitalWrite(ledVermelho, LOW);
  digitalWrite(buzzer, LOW);

  // Cria caracteres personalizados
  lcd.createChar(0, skull);
  lcd.createChar(1, warning);
}

void loop() {
  // Lê a temperatura do sensor DHT22
  float temperatura = dht.readTemperature();

  // Verifica se a leitura falhou e se sim, tenta novamente
  if (isnan(temperatura)) {
    Serial.println("Falha ao ler do sensor DHT!");
    return;
  }

  // Imprime a temperatura no monitor serial
  Serial.print("Temperatura: ");
  Serial.print(temperatura);
  Serial.println(" *C");

  // Limpa o display LCD
  lcd.clear();

  // Lógica de controle dos LEDs e do buzzer
  if (temperatura < 30) {
    digitalWrite(ledVerde, HIGH);
    digitalWrite(ledAmarelo, LOW);
    digitalWrite(ledVermelho, LOW);
    lcd.setCursor(0, 0);
    lcd.print("Temperatura: ");
    lcd.print(temperatura);
    lcd.setCursor(0, 1);
    lcd.print("Normal");
  } 
  else if (temperatura >= 30 && temperatura <= 60) {
    digitalWrite(ledVerde, LOW);
    digitalWrite(ledAmarelo, HIGH);
    digitalWrite(ledVermelho, LOW);
    lcd.setCursor(0, 0);
    lcd.print("Temperatura: ");
    lcd.print(temperatura);
    lcd.setCursor(0, 1);
    lcd.print("Quente");
    lcd.setCursor(19, 1);
    lcd.write(byte(1)); // Warning
  } 
  else if (temperatura > 60) {
    digitalWrite(ledVerde, LOW);
    digitalWrite(ledAmarelo, LOW);
    digitalWrite(ledVermelho, HIGH);
     for (int freq = 800; freq <= 1200; freq += 100) {
      tone(buzzer, freq);
      delay(500); // espera 500ms para cada frequência
    }
    noTone(buzzer);
    lcd.setCursor(0, 0);
    lcd.print("Temperatura: ");
    lcd.print(temperatura);
    lcd.setCursor(0, 1);
    lcd.print("Pare pelo amor");
    lcd.setCursor(0, 2);
    lcd.print("de Deus");
    lcd.setCursor(19, 2);
    lcd.write(byte(0)); // Skull
  }

  // Aguarda 2 segundos antes da próxima leitura
  delay(2000);
}
