#include <QTRSensors.h>
#include <SparkFun_TB6612.h>

#define AIN1 5
#define BIN1 7
#define AIN2 4
#define BIN2 8
#define PWMA 3
#define PWMB 9
#define STBY 2 // no se utiliza, está conectado a la fuente directamente

int pushButton = 2;
int currentState;
int lastState = HIGH;
boolean onoff = true;

const int offsetA = 1;
const int offsetB = 1;

// Initializing motors.  The library will allow you to initialize as many
// motors as you have memory for.  If you are using functions like forward
// that take 2 motors as arguements you can either write new functions or
// call the function more than once.
Motor motor1 = Motor(AIN1, AIN2, PWMA, offsetA, STBY);
Motor motor2 = Motor(BIN1, BIN2, PWMB, offsetB, STBY);

QTRSensors qtr;

const uint8_t SensorCount = 8;
uint16_t sensorValues[SensorCount];
int threshold[SensorCount];

float Kp = 0.07, Ki = 0.0, Kd = 0.17;
float Pvalue, Ivalue, Dvalue;

uint16_t position;
int P, D, I, previousError, PIDvalue, error;
int lsp, rsp;
int lfspeed = 150 ;

// ------------------------------------------------------------
  
void setup()
{
  // configure the sensors
  Serial.begin(9600);
  qtr.setTypeAnalog();
  qtr.setSensorPins((const uint8_t[]){A0, A1, A2, A3, A4, A5, A6, A7}, SensorCount);
  qtr.setEmitterPin(13);
  delay(500);

  pinMode(pushButton, INPUT_PULLUP); 
  
  digitalWrite(LED_BUILTIN, HIGH); // turn on Arduino's LED to indicate we are in calibration mode
   
  // analogRead() takes about 0.1 ms on an AVR.
  // 0.1 ms per sensor * 4 samples per sensor read (default) * 6 sensors
  // * 10 reads per calibrate() call = ~24 ms per calibrate() call.
  // Call calibrate() 400 times to make calibration take about 10 seconds.
  for (uint16_t i = 0; i < 400; i++)
  {
    qtr.calibrate();
  }
  digitalWrite(LED_BUILTIN, LOW); // turn off Arduino's LED to indicate we are through with calibration
  
  // print the calibration minimum values measured when emitters were on
  for (uint8_t i = 0; i < SensorCount; i++)
  {
    Serial.print(qtr.calibrationOn.minimum[i]);
    Serial.print(' ');
  }
  Serial.println();

  // print the calibration maximum values measured when emitters were on
  for (uint8_t i = 0; i < SensorCount; i++)
  {
    Serial.print(qtr.calibrationOn.maximum[i]);
    Serial.print(' ');
  }

  delay(1000);
}

// ------------------------------------------------------------

void loop()
{

  currentState = digitalRead(pushButton);

  Serial.println(onoff);
  // Detectar flanco de bajada (botón presionado)
  if (lastState == HIGH && currentState == LOW) {
    onoff = !onoff; 
//    delay(50); // debounce simple
  }

  if(onoff)
  {
    robot_control();
    Serial.println("SYSTEM ON");
  }
  else
  {
    brake(motor1,motor2);
  }

  lastState = currentState;
   
}

// ------------------------------------------------------------

    // Hay que definir los valores del sensor 7000 

void robot_control()
{
  // read calibrated sensor values and obtain a measure of the line position
  // from 0 to 7000 (for a white line, use readLineWhite() instead)
  position = qtr.readLineBlack(sensorValues);
  error = 3500 - position;
  
//  int promMax = 0, promMin = 0; 
//  for(int i = 0; i < SensorCount; i++)
//  {
//    promMax += qtr.calibration.maximum[i]; 
//    promMin += qtr.calibration.minimum[i]; 
//  }
//  promMax = promMax/SensorCount;
//  promMin = promMin/SensorCount;

  bool allSensorsWhite = true;
  for(int i = 0; i < SensorCount; i++)
  {
    if(sensorValues[i] < qtr.calibrationOn.maximum[i] - 100)
    {
      allSensorsWhite = false;
      break;
    }
  }
  
  if(allSensorsWhite)
  { 
    if(previousError>0)
    {      
      motor_drive(-50,50); //Turn left if the line was to the left before
    }
    else
    {
      motor_drive(50,-50); // Else turn right
    }
    return;
  }
  
  PID_Linefollow(error);
}

// ------------------------------------------------------------

void PID_Linefollow(int error)
{
    // términos PID
    P = error;
    I = I + error;
    D = error - previousError;

    // anti-windup para I 
    if (I > 1000) I = 1000;
    if (I < -1000) I = -1000;

    // output PID
    Pvalue = Kp*P;
    Ivalue = Ki*I;
    Dvalue = Kd*D; 

    float PIDvalue = Pvalue + Ivalue + Dvalue;
    
    previousError = error;

    lsp = lfspeed - PIDvalue;
    rsp = lfspeed + PIDvalue;

    lsp = constrain(lsp, -150, 150);
    rsp = constrain(rsp, -150, 150);
  
    motor_drive(lsp,rsp);
}

// ------------------------------------------------------------

void motor_drive(int left, int right){
    motor1.drive(right);
    motor2.drive(left);
}
