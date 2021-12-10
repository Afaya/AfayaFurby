#include <DHT.h>
#include "Wire.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

const int ledPinTurnOn =  2;
int ledRed= 5;
int ledGreen = 4;
int ledBlue = 3;
int ledStateOn = HIGH;   
int backSwitchPin = 6;
int mouthSwitchPin = 7;
int readingBack;
int readingMouth; 
double previous_ax = 0;
double previous_ay = 0;
double previous_az = 0;
double previous_gx = 0;
double previous_gy = 0;
double previous_gz = 0;
bool isCold = false;
bool isHot = false;

//sensor DHT11
#define DHTPIN 8 // Definimos el pin digital donde se conecta el sensor
#define DHTTYPE DHT11 // Dependiendo del tipo de sensor
DHT dht(DHTPIN, DHTTYPE);// Inicializamos el sensor DHT11

//GY-521 giroscope MPU-6050
const int MPU_ADDR = 0x68; // I2C address of the MPU-6050. If AD0 pin is set to HIGH, the I2C address will be 0x69.
int16_t accelerometer_x, accelerometer_y, accelerometer_z; // variables for accelerometer raw data
int16_t gyro_x, gyro_y, gyro_z; // variables for gyro raw data
int16_t temperature; // variables for temperature data
char tmp_str[7]; // temporary variable used in convert function
char* convert_int16_to_str(int16_t i) { // converts int16 to string. Moreover, resulting strings will have the same length in the debug monitor.
  sprintf(tmp_str, "%6d", i);
  return tmp_str;
}

//mp3 player
SoftwareSerial mySoftwareSerial(10, 11);
DFRobotDFPlayerMini myDFPlayer;

void setup() {
    Serial.begin(9600);
    
    pinMode(ledPinTurnOn, OUTPUT);
    pinMode(ledRed, OUTPUT);
    pinMode(ledGreen, OUTPUT);
    pinMode(ledBlue, OUTPUT);
    pinMode(backSwitchPin, INPUT);
    pinMode(mouthSwitchPin, INPUT);
    
    dht.begin();
    
    Wire.begin();
    Wire.beginTransmission(MPU_ADDR); // Begins a transmission to the I2C slave (GY-521 board)
    Wire.write(0x6B); // PWR_MGMT_1 register
    Wire.write(0); // set to zero (wakes up the MPU-6050)
    Wire.endTransmission(true);

    mySoftwareSerial.begin(9600);
    Serial.println("Initializing DFPlayer");

     while(!myDFPlayer.begin(mySoftwareSerial)){ Serial.println("Not Connected!"); }
    Serial.println("Player online!");

    myDFPlayer.setTimeOut(500);
    myDFPlayer.volume(20);
    myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);
    myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);
    
}

void loop() {
 turnOnLed();
 readingBack = digitalRead(backSwitchPin);
 readingMouth = digitalRead(mouthSwitchPin);
 if(readingBack == HIGH){
    party();
 } else if(readingMouth == HIGH){
    eating();
 }  else {
    temperatureHumidityListen();
 }
}

void turnOnLed() {
  digitalWrite(ledPinTurnOn, ledStateOn);
}

void party(){
  Serial.println("Entro en fiesta");
  speakText(1);
  RGB_color(255, 0, 0); 
  delay(1000);
  RGB_color(0, 255, 0); 
  delay(1000);
  RGB_color(0, 0, 255); 
  delay(1000);
  RGB_color(0, 0, 0);
}

void RGB_color(int redValue, int greenValue, int blueValue){
  analogWrite(ledRed, redValue);
  analogWrite(ledGreen, greenValue);
  analogWrite(ledBlue, blueValue);
}

void speakText(int audioToPlay){
  myDFPlayer.playMp3Folder(audioToPlay);
  delay(6000);
  myDFPlayer.stop();
}

void eating(){
  Serial.println("Entro en comiendo");
  long randomNumber = random(1,4);
  if(randomNumber == 3 ){
     speakText(2);
     Serial.println("¡Ag no gusta!");
   }
  else {
     speakText(3);
     Serial.println("¡umm, que rico!");
  }  
}


void temperatureHumidityListen(){
  Serial.println("Entro en DHT11");
  delay(500);
 
  // Leemos la humedad relativa
  float h = dht.readHumidity();
  // Leemos la temperatura en grados centígrados (por defecto)
  float t = dht.readTemperature();
  // Leemos la temperatura en grados Fahrenheit
  float f = dht.readTemperature(true);
 
  // Comprobamos si ha habido algún error en la lectura
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Error obteniendo los datos del sensor DHT11");
    return;
  }
 
  // Calcular el índice de calor en grados centígrados
  float hic = dht.computeHeatIndex(t, h, false);
  
  if((t < 18 || hic < 19.6) && !isCold)
  {
    isHot = false;
    isCold = true;
    speakText(4);
    Serial.println("¡Abrazame, que hace frio!");
    
  } 
  else if ((t > 25 || hic > 24) && !isHot)
  {
    isHot = true;
    isCold = false;
    
    speakText(5);
    Serial.println("¡Que calor! Abanícame");
  } 
  else
  {
    claimAttention();
  }
  
}

void claimAttention(){
    long randomNumber = random(1,20);
    switch (randomNumber) {
    case 1:
      speakText(6);
      Serial.println("Guau hambre Guau");
      break;
    case 2:
      speakText(7);
      Serial.println("Guau aburrido Guau");
      break;
    default:
      giroscopeListen();
    }   
}

void giroscopeListen(){
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B); // starting with register 0x3B (ACCEL_XOUT_H) [MPU-6000 and MPU-6050 Register Map and Descriptions Revision 4.2, p.40]
  Wire.endTransmission(false); // the parameter indicates that the Arduino will send a restart. As a result, the connection is kept active.
  Wire.requestFrom(MPU_ADDR, 7*2, true); // request a total of 7*2=14 registers
  
  accelerometer_x = Wire.read()<<8 | Wire.read(); // reading registers: 0x3B (ACCEL_XOUT_H) and 0x3C (ACCEL_XOUT_L)
  accelerometer_y = Wire.read()<<8 | Wire.read(); // reading registers: 0x3D (ACCEL_YOUT_H) and 0x3E (ACCEL_YOUT_L)
  accelerometer_z = Wire.read()<<8 | Wire.read(); // reading registers: 0x3F (ACCEL_ZOUT_H) and 0x40 (ACCEL_ZOUT_L)
  gyro_x = Wire.read()<<8 | Wire.read(); // reading registers: 0x43 (GYRO_XOUT_H) and 0x44 (GYRO_XOUT_L)
  gyro_y = Wire.read()<<8 | Wire.read(); // reading registers: 0x45 (GYRO_YOUT_H) and 0x46 (GYRO_YOUT_L)
  gyro_z = Wire.read()<<8 | Wire.read(); // reading registers: 0x47 (GYRO_ZOUT_H) and 0x48 (GYRO_ZOUT_L)

  int differenceGx = previous_gx - (int) gyro_x;
  bool isGxdifferent = previous_gx != 0 && previous_gx != (int) gyro_x && differenceGx  > 100; 
  int differenceGy = previous_gy - (int) gyro_y;  
  bool isGydifferent = previous_gy != 0 && previous_gy != (int) gyro_y && differenceGy  > 100;
  int differenceGz = previous_gz - (int) gyro_z; 
  bool isGzdifferent = previous_gz != 0 && previous_gz != (int) gyro_z && differenceGz  > 100;

  if(isGxdifferent || isGydifferent || isGzdifferent){
     speakText(8);
     Serial.println("Divertido, ¡otra vez!");
  } 
  previous_gx = (int) gyro_x;
  previous_gy = (int) gyro_y;
  previous_gz = (int) gyro_z;
 
  delay(1000);
}
