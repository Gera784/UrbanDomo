#include <DHT.h>
#include <Servo.h>
// Configuración del DHT11
#define DHTPIN 2       // Pin donde conectas el DHT11
#define DHTTYPE DHT11 // Tipo de sensor
DHT dht(DHTPIN, DHTTYPE);
// Configuración del relé
#define RELE_PIN 8
#define TEMP_UMBRAL 25   // Temperatura en °C para activar el relé
//Configuracion Sensor ultrasonico y servo
#define TRIG 9
#define ECHO 10
#define SERVO_PIN 3
Servo servo;
//configuracion LDR
const int sensorLuz = A0;
int valorLuz;
const int LED1 = 12;
const int LED2 = 11; 
unsigned long ultima_medicion_dht;

void setup() {

  Serial.begin(9600);
  dht.begin();
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(LED1,OUTPUT);
  pinMode(LED2,OUTPUT);
  pinMode(RELE_PIN, OUTPUT);
  servo.attach(SERVO_PIN);
  servo.write(0); // posición inicial servo
  digitalWrite(RELE_PIN, HIGH); // Relé apagado al inicio
  ultima_medicion_dht = millis();
}

void loop() {
//
//RELÉ SENSOR ULTRASONICO
delay(50);
  long duracion;
  int distancia;
  // Enviar pulso ultrasónico
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  // Medir tiempo de retorno
  duracion = pulseIn(ECHO, HIGH);
  //Serial.println(duracion);
  distancia = duracion * 0.0332 / 2; // cm
  //Serial.print("Distancia: ");
  //Serial.print(distancia);
  //Serial.println(" cm");
  // Si un objeto está cerca (menos de 15 cm), mover el servo
  if (distancia < 15) {
    servo.write(90); // mover a 90 grados
    delay(2000);
  } else {
    servo.write(0);  // volver a 0 grados
  }
//
//ILUMINACION
//Lectura de Nivel de Iluminacion
  valorLuz = analogRead(sensorLuz);
  //Serial.print("Nivel de luz: ");
  //Serial.println(valorLuz);
//Control de Iluminacion
 if (valorLuz < 500){
  digitalWrite(LED1, HIGH);
  digitalWrite(LED2,HIGH);
 }else{
  digitalWrite(LED1, LOW);
  digitalWrite(LED2,LOW);
 }
//
//TEMPERATURA
//Lecturas de temperatura
  if (ultima_medicion_dht + 2000 < millis()) // Tiempo recomendado entre lecturas
  {
    ultima_medicion_dht = millis();
    float t = dht.readTemperature(); // Lee temperatura en °C
    Serial.print("Temperatura: ");
    Serial.print(t);
    Serial.println(" °C");
  // Control del relé
    if (t > TEMP_UMBRAL) {
      digitalWrite(RELE_PIN, HIGH); // Activa relé
      //Serial.println("Relé ACTIVADO");
    } else {
      digitalWrite(RELE_PIN, LOW);  // Desactiva relé
      //Serial.println("Relé DESACTIVADO");
    }
  }
}
