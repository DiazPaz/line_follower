// motores
int PWMA = 3; 
int INA2 = 5;
int INA1 = 4;

int PWMB = 9;
int INB1 = 7; 
int INB2 = 8; 

void setup() {
  Serial.begin(9600);
  
  pinMode(PWMA, OUTPUT); pinMode(INA2, OUTPUT); pinMode(INA1, OUTPUT);
  pinMode(PWMB, OUTPUT); pinMode(INB1, OUTPUT); pinMode(INB2, OUTPUT);
  digitalWrite(PWMA, HIGH);
  digitalWrite(PWMB, HIGH);
}

void loop() {
  digitalWrite(INA1, HIGH); digitalWrite(INA2, LOW);
  digitalWrite(INB1, HIGH); digitalWrite(INB2, LOW);
  delay(3000);
  digitalWrite(INA1, LOW); digitalWrite(INA2, HIGH);
  digitalWrite(INB1, LOW); digitalWrite(INB2, HIGH);
  delay(3000);
}
