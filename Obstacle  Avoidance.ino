#include <Adafruit_VL53L0X.h>


#include <Wire.h>


int xshut[5] = {2, 3, 4, 5, 6};  // GPIO pins for XSHUT

Adafruit_VL53L0X  sensor1= Adafruit_VL53L0X();
Adafruit_VL53L0X  sensor2= Adafruit_VL53L0X();
Adafruit_VL53L0X  sensor3= Adafruit_VL53L0X();
Adafruit_VL53L0X  sensor4= Adafruit_VL53L0X();
Adafruit_VL53L0X  sensor5= Adafruit_VL53L0X();

void setup() {
  Serial.begin(115200);
  Wire.begin();  // Ensure I2C is initialized

  // Turn off all sensors before configuring addresses
  for (int i = 0; i < 5; i++) {
    pinMode(xshut[i], OUTPUT);
    digitalWrite(xshut[i], LOW);
  }

  // Initialize each VL53L0X sensor
  init_VL53L0X();
}

void loop() {
  // Print distance measurements from all sensors
  Serial.print("Sensor 1: "); Serial.print(sensor1.readRange()); Serial.print(" mm\t");
  Serial.print("Sensor 2: "); Serial.print(sensor2.readRange()); Serial.print(" mm\t");
  Serial.print("Sensor 3: "); Serial.print(sensor3.readRange()); Serial.print(" mm\t");
  Serial.print("Sensor 4: "); Serial.print(sensor4.readRange()); Serial.print(" mm\t");
  Serial.print("Sensor 5: "); Serial.print(sensor5.readRange()); Serial.println(" mm");
  
  delay(100);
}

void init_VL53L0X() {
  Serial.print("Initializing VL53L0X sensors...");

  // Initialize Sensor 1
  digitalWrite(xshut[0], HIGH);
  delay(100);
  sensor1.begin(1);
  sensor1.setAddress((uint8_t)01);
  delay(100);

  // Initialize Sensor 2
  digitalWrite(xshut[1], HIGH);
  delay(100);
  sensor2.begin(1);
  sensor2.setAddress((uint8_t)02);
  delay(100);

  // Initialize Sensor 3
  digitalWrite(xshut[2], HIGH);
  delay(100);
  sensor3.begin(1);
  sensor3.setAddress((uint8_t)03);
  delay(100);

  // Initialize Sensor 4
  digitalWrite(xshut[3], HIGH);
  delay(100);
  sensor4.begin(1);
  sensor4.setAddress((uint8_t)04);
  delay(100);

  // Initialize Sensor 5
  digitalWrite(xshut[4], HIGH);
  delay(100);
  sensor5.begin(1);
  sensor5.setAddress((uint8_t)05);
  delay(100);

  Serial.println("Done!");

  // Start continuous mode for all sensors
  sensor1.startRangeContinuous();
  sensor2.startRangeContinuous();
  sensor3.startRangeContinuous();
  sensor4.startRangeContinuous();
  sensor5.startRangeContinuous();
}
