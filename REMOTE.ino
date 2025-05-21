#include <WiFi.h>
#include "Wire.h"

#define I2C_DEV_ADDR 0x55
int p, r, t, y, s1, s2;
void onReceive(int len) {
  if (len < 6 * sizeof(int)) return;  // Ensure full packet
  Wire.readBytes((uint8_t*)&r, sizeof(r));
  Wire.readBytes((uint8_t*)&p, sizeof(p));
  Wire.readBytes((uint8_t*)&y, sizeof(y));
  Wire.readBytes((uint8_t*)&t, sizeof(t));
  Wire.readBytes((uint8_t*)&s1, sizeof(s1));
  Wire.readBytes((uint8_t*)&s2, sizeof(s2));
}

// Pin definitions
const int CS0_PIN = 13;//pitch
const int CS1_PIN = 15;//roll
const int CS2_PIN = 14;//yaw
const int CS3_PIN = 33;//thro

const int INC_PIN = 23;
const int UD_PIN = 18;
const float MAX_VOLTAGE = 3.3;
const int STEPS = 99;

// PID Controller variables
float Kp = 2.0;    // Proportional constant
float Ki = 0.5;    // Integral constant
float Kd = 1.0;    // Derivative constant

float prevError = 0;
float integral = 0;

// Low-pass filter variables
float smoothedVoltage = 0; // Filtered voltage
const float alpha = 0.4;   // Smoothing factor (0.0 to 1.0)

// Function to apply low-pass filter to analog readings
float lowPassFilter(int rawAnalogValue) {
    float rawVoltage = rawAnalogValue * MAX_VOLTAGE / 4095;
    smoothedVoltage = alpha * rawVoltage + (1 - alpha) * smoothedVoltage;
    return smoothedVoltage;
}

// Function to perform an increment pulse
void pulseIncrement(int cs_pin) {
    digitalWrite(cs_pin, LOW);
    delayMicroseconds(10);
    digitalWrite(INC_PIN, LOW);
    delayMicroseconds(1);
    digitalWrite(INC_PIN, HIGH);
    delayMicroseconds(1);
    digitalWrite(cs_pin, HIGH);
}

// Adjust the throttle voltage
void adjustVoltage(int cs_pin, int analogPin, float targetVoltage) {
    float currentVoltage = lowPassFilter(analogRead(analogPin)); // Use low-pass filter for smoother reading

    for (int i = 0; i <= STEPS; i++) {
        // Calculate error
        float error = targetVoltage - currentVoltage;

        // Determine direction based on error
        digitalWrite(UD_PIN, error > 0 ? HIGH : LOW);

        // Perform increment/decrement pulse
        pulseIncrement(cs_pin);

        // Update current voltage with the low-pass filter
        currentVoltage = lowPassFilter(analogRead(analogPin));

        // Debugging output
/*Serial.print("Step: ");
        Serial.print(i);
        Serial.print(" | Target Voltage: ");
        Serial.print(targetVoltage, 3);
        Serial.print(" | Current Voltage: ");
        Serial.println(currentVoltage, 3);
*/
        // Break if within the desired range
        if (abs(currentVoltage - targetVoltage) <= 0.01) {
            break;
        }
    }
}

float mapFloat(float x, float in_min, float in_max, float out_min, float out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

float mapAndAdjustJoystickDeadBandValues(float value, bool reverse) {
    if (value >= 2200) {
        value = mapFloat(value, 2200, 4095, 1.69, 3.3);
    } else if (value <= 1800) {
        value = (value == 0 ? 0 : mapFloat(value, 1800, 0, 1.52, 0));  
    } else {
        value = 1.69;
    }

    if (reverse) {
        value = 3.3 - value;
    }

    //Serial.println(value, 3); // Print with 3 decimal places
    return (value);
}

void setup() {
    Serial.begin(115200);
    Wire.onReceive(onReceive);
    Wire.begin(I2C_DEV_ADDR);  // ESP32 as I2C Slave
    // Set pin modes
    pinMode(CS0_PIN, OUTPUT);
    pinMode(CS1_PIN, OUTPUT);
    pinMode(CS2_PIN, OUTPUT);
    pinMode(CS3_PIN, OUTPUT);
    pinMode(INC_PIN, OUTPUT);
    pinMode(UD_PIN, OUTPUT);

    // Initialize digital pins
    digitalWrite(CS0_PIN, HIGH);
    digitalWrite(CS1_PIN, HIGH);
    digitalWrite(CS2_PIN, HIGH);
    digitalWrite(CS3_PIN, HIGH);

    // Set ADC resolution
    analogReadResolution(12);

    // Initialize throttle to 0V
    smoothedVoltage = 0; // Initialize the filter
 ;
    delay(1000);
}

void loop() {
    // Continuously adjust throttle to maximum voltage (3.3V)
    float tt0 = mapAndAdjustJoystickDeadBandValues(analogRead(34), false);//pitch
   // Serial.println(tt0);
    adjustVoltage(13,27,(tt0+mapFloat(p, 0, 80,-1.0, 1.0)));
    // Continuously adjust throttle to maximum voltage (3.3V)
    float tt1 = mapAndAdjustJoystickDeadBandValues(analogRead(35), false);//roll
    adjustVoltage(15,26,(tt1+mapFloat(r, 0, 80,-1.0, 1.0)));
    // Continuously adjust throttle to maximum voltage (3.3V)
    float tt2 = mapAndAdjustJoystickDeadBandValues(analogRead(39), false);//yaw
    adjustVoltage(14,32,(tt2+mapFloat(y, 0, 80,-1.0, 1.0)));
    // Continuously adjust throttle to maximum voltage (3.3V)
    float tt3 = mapAndAdjustJoystickDeadBandValues(analogRead(36),false);//thro
    adjustVoltage(33,25,(tt3+mapFloat(t, 0, 80,-1.0, 1.0)));
    //Serial.printf("Received: p=%d, r=%d, t=%d, y=%d, s1=%d, s2=%d\n", p, r, t, y, s1, s2);
    delay(1); // Small delay to avoid spamming the throttle adjustment
}