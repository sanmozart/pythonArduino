#include "Wire.h"

enum Devices
{
  MPU_9150 = 0x68,  // I2C адрес датчика MPU_9150
  ADXL345 = 0x53,   // I2C адрес датчика ADXL345
  LIS3DH = 0x19     // I2C адрес датчика LIS3DH
};
// массив данных MPU_9150
// [accX, accY, accZ, temp, gyrX, gyrY, gyrZ]
// [ 3B    3D    3F    41    43    45    47 ]

// массив данных ADXL345
// [accX, accY, accZ]
// [ 32    34    36 ]

// массив данных LIS3DH
// [accX, accY, accZ]
// [ 28    2A    2C ]

int16_t data[7];

const char* toString(const Devices& deviceId);

void setup() 
{
  // инициализация
  Serial.begin(9600);
  Wire.begin();

  getID(MPU_9150);
  // getID(ADXL345);
  // getID_LIS3DH(LIS3DH);

  // инициализация MPU_9150
  Wire.beginTransmission(MPU_9150);
  Wire.write(0x6B);   // PWR_MGMT_1 register
  Wire.write(0);      // set to zero (wakes up the MPU_9150)
  Wire.endTransmission(true);
}

void loop()
{
  getData();// получаем
  // выводим
  for (byte i = 0; i < 7; i++)
  {
    switch (i) {
      case 0:
      case 1:
      case 2:
        Serial.println(data[i] / 16384.f, 5); // пересчет в g и вывод с точностью 5 знаков после запятой
        break;
        
      case 3:
        Serial.println (data[i] / 340.f + 35, 5); // пересчет в градусы и вывод с точностью 5 знаков после запятой
        break;
        
      case 4:
      case 5:
      case 6:
        Serial.println (data[i] / 32768.f * 250, 5); // пересчет в м/с и вывод с точностью 5 знаков после запятой
        break;
      
      default:
        Serial.println ("null");
    }
  }
}

void getData()
{
  Wire.beginTransmission(MPU_9150);
  Wire.write(0x3B); // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_9150, 14, true); // request a total of 14 registers
  for (byte i = 0; i < 7; i++)
  {
    data[i] = Wire.read() << 8 | Wire.read();
  }
}

void getID (int DEV_NAME)
{
  // запишем адрес регистра DEVID
  Wire.beginTransmission(DEV_NAME);
  Wire.write(byte(0x00));
  Wire.endTransmission();

  // прочитаем регистр DEVID:
  Wire.requestFrom(DEV_NAME, 1);
  while (Wire.available()) 
    {
      byte c = Wire.read();
      Serial.print("ID_");
      Serial.print(toString(DEV_NAME));
      Serial.print(" = ");
      Serial.println(c, HEX);
    }
} 

void getID_LIS3DH (int DEV_NAME)
{
  // запишем адрес регистра WHO_AM_I
  Wire.beginTransmission(DEV_NAME);
  Wire.write(byte(0x0F));
  Wire.endTransmission();

  // прочитаем регистр WHO_AM_I
  Wire.requestFrom(DEV_NAME, 1);
  while (Wire.available()) 
    {
      byte c = Wire.read();
      Serial.print("ID_");
      Serial.print(toString(DEV_NAME));
      Serial.print(" = ");
      Serial.println(c, HEX);
    }
} 


const char* toString(const Devices& deviceId)
{
  switch(deviceId)
  {
    case MPU_9150:
      return "MPU_9150";
    case ADXL345:
      return "ADXL345";
    case LIS3DH:
      return "LIS3DH";  
    default:
      return "UNKNOWN ID";
  }
}
