#include <QTRSensors.h>
#include <SparkFun_TB6612.h>

#define AIN1 4
#define BIN1 7
#define AIN2 5
#define BIN2 8
#define PWMA 3
#define PWMB 9
#define STBY 9

const int offsetA = 1;
const int offsetB = 1;

Motor motor1 = Motor(AIN1, AIN2, PWMA, offsetA, STBY);
Motor motor2 = Motor(BIN1, BIN2, PWMB, offsetB, STBY);

QTRSensors qtr;

const uint8_t SensorCount = 8;
uint16_t sensorValues[SensorCount];

void setup()
{
  Serial.begin(9600);
  qtr.setTypeAnalog();
  qtr.setSensorPins((const uint8_t[]){A0, A1, A2, A3, A4, A5, A6, A7}, SensorCount);
  qtr.setEmitterPin(13);

  // analogRead() takes about 0.1 ms on an AVR.
  // 0.1 ms per sensor * 4 samples per sensor read (default) * 6 sensors
  // * 10 reads per calibrate() call = ~24 ms per calibrate() call.
  // Call calibrate() 400 times to make calibration take about 10 seconds.
  for (uint16_t i = 0; i < 400; i++)
  {
    qtr.calibrate();
  }

  // ------------------- CALIBRATION 1 --------------------------
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

  // ------------------- CALIBRATION 2 --------------------------
  // print the calibration minimum values measured when emitters were on
//  for (uint8_t i = 0; i < SensorCount; i++)
//  {
//    threshold[i] = (qtr.calibrationOn.minimum[i] + qtr.calibrationOn.maximum[i])/2;
//    Serial.print(threshold[i]);
//    Serial.print("  ");
//  }
  
  Serial.println();
  Serial.println();
  Serial.println();
  
  delay(1000);
}

void loop()
{
//   motor1.drive(255,1000);
//   motor1.drive(-255,1000);
//   motor1.brake();
//   delay(1000);
//
//   motor2.drive(255,1000);
//   motor2.drive(-255,1000);
//   motor2.brake();
//   delay(1000);
//
//   forward(motor1, motor2, 150);
//   delay(1000);
//
//   back(motor1, motor2, -150);
//   delay(1000);
//
//   brake(motor1, motor2);
//   delay(1000);
//
//   left(motor1, motor2, 100);
//   delay(1000);
//   right(motor1, motor2, 100);
//   delay(1000);
//   
//   brake(motor1, motor2);
//   delay(1000);
   
  // read calibrated sensor values and obtain a measure of the line position
  // from 0 to 5000 (for a white line, use readLineWhite() instead)
  uint16_t position = qtr.readLineBlack(sensorValues);
  
  // print the sensor values as numbers from 0 to 1000, where 0 means maximum
  // reflectance and 1000 means minimum reflectance, followed by the line
  // position
  for (uint8_t i = 0; i < SensorCount; i++)
  {
    Serial.print(sensorValues[i]);
    Serial.print('\t');
  }
  Serial.println(position);

  delay(250);
}
