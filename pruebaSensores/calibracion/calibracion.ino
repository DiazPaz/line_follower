int pushButton = 2;
int ledPin = LED_BUILTIN;

bool ledState = true;
int currentState;
int lastState = HIGH;

void setup() {
  Serial.begin(9600);
  pinMode(pushButton, INPUT_PULLUP);  // Usa resistencia interna
  pinMode(ledPin, OUTPUT);
}

void loop() {
  currentState = digitalRead(pushButton);

  // Detectar flanco de bajada (botón presionado)
  if (lastState == HIGH && currentState == LOW) {
    ledState = !ledState;  // Toggle del LED
    digitalWrite(ledPin, ledState);
    Serial.println(ledState ? "LED encendido" : "LED apagado");
    delay(50); // debounce simple
  }

  lastState = currentState;
}
