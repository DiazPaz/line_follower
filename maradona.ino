// === Configuración global ===
int sensorPins[8] = {A0, A1, A2, A3, A4, A5, A6, A7};
int sensorMin[8];
int sensorMax[8];
int setPoint = 3500; // Centro (de 0 a 7000)
int velocidadBase = 120;

// Pines de motor
const int ENIzq = 5; 
const int IN1   = 7;
const int IN2   = 8;

const int ENDer = 6;
const int IN3   = 9;
const int IN4   = 10;

// Variables PID
float Kp = 0.05;
float Ki = 0.0005;
float Kd = 0.5;

int errorAnterior = 0;
float errorAcumulado = 0;

void setup() {
  Serial.begin(9600);

  for (int i = 0; i < 8; i++) {
    pinMode(sensorPins[i], INPUT);
    sensorMin[i] = 1023;
    sensorMax[i] = 0;
  }

  pinMode(ENIzq, OUTPUT); pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(ENDer, OUTPUT); pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);

  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);

  Serial.println("=== CALIBRANDO SENSORES ===");
  delay(2000);
  unsigned long tInicio = millis();
  while (millis() - tInicio < 5000) {
    for (int i = 0; i < 8; i++) {
      int lectura = analogRead(sensorPins[i]);
      if (lectura < sensorMin[i]) sensorMin[i] = lectura;
      if (lectura > sensorMax[i]) sensorMax[i] = lectura;
    }
    delay(5);
  }

  for (int i = 0; i < 8; i++) {
    Serial.print("Sensor "); Serial.print(i);
    Serial.print(" - Min: "); Serial.print(sensorMin[i]);
    Serial.print(" | Max: "); Serial.println(sensorMax[i]);
  }
}

void loop() {
  int posicion = calcularPosicionLinea();
  int error = posicion - setPoint;

  // PID
  errorAcumulado += error;
  int derivada = error - errorAnterior;
  errorAnterior = error;

  float ajuste = Kp * error + Ki * errorAcumulado + Kd * derivada;

  int velIzq = velocidadBase + ajuste;
  int velDer = velocidadBase - ajuste;

  velIzq = constrain(velIzq, 0, 255);
  velDer = constrain(velDer, 0, 255);

  analogWrite(ENIzq, velIzq);
  analogWrite(ENDer, velDer);
}

// === Cálculo de posición ===
int calcularPosicionLinea() {
  long sumaPonderada = 0;
  int sumaTotal = 0;

  for (int i = 0; i < 8; i++) {
    int lectura = analogRead(sensorPins[i]);
    int normalizado = map(lectura, sensorMin[i], sensorMax[i], 1000, 0);
    normalizado = constrain(normalizado, 0, 1000);
    sumaPonderada += (long)normalizado * (i * 1000);
    sumaTotal += normalizado;
  }

  if (sumaTotal == 0) return setPoint; // No hay línea
  return sumaPonderada / sumaTotal;
}
