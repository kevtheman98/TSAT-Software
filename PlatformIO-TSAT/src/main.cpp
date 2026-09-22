#include <Arduino.h> // Arduino core library
#include "sensors.h" // Our own header, gives access to initSensor() and readSensor()

void setup(){ // setup() runs ONCE, when ESP32 is powered on or reset
    Serial.begin(115200); // Start serial communication at 115200 baud (bits per second)
    delay(1000); // Wait for serial port to connect (for native USB devices)

    if (!initSensor()) { // Calls our sensor setup function. If it fails, print error and halt execution.
        Serial.println("Failed to communicate with LSM9DS1 sensor, check wiring!");
        while (1) delay(10); // Infinite loop to halt execution if sensor initialization fails.
    }

    Serial.println("LSM9DS1 sensor initialized successfully!");
}

void loop() { // loop() runs repeatedly for as long as ESP32 is powered on
  sensors_event_t a, m, g, temp;
  // Creates four separate "event" containers to hold the results
  // accelerometer (a), magnetometer (m), gyroscope (g), and temperature (temp).
  // sensors_event_t is a standard Adafruit struct type used across their sensor libraries.
  readSensor(a, m, g, temp);
  // Calls our function to pull fresh readings into the four containers above.

  Serial.print("Accel X: "); Serial.print(a.acceleration.x);
  // a.acceleration.x/y/z holds accelerometer readings in m/s^2 for each axis.
  Serial.print("  Y: "); Serial.print(a.acceleration.y);
  Serial.print("  Z: "); Serial.println(a.acceleration.z);
  // println on the last one adds a line break after all three values print on one line.

  Serial.print("Mag X: "); Serial.print(m.magnetic.x);
  // m.magnetic.x/y/z holds magnetometer readings in micro-Tesla (uT) for each axis.
  Serial.print("  Y: "); Serial.print(m.magnetic.y);
  Serial.print("  Z: "); Serial.println(m.magnetic.z);

  Serial.print("Gyro X: "); Serial.print(g.gyro.x);
  // g.gyro.x/y/z holds gyroscope readings in rad/s for each axis.
  Serial.print("  Y: "); Serial.print(g.gyro.y);
  Serial.print("  Z: "); Serial.println(g.gyro.z);

  Serial.println();
  // Print a blank line to separate each loop iteration's output for readability.
  delay(500);
  // Wait 500 milliseconds before the next loop iteration. This controls how often we read and print sensor data.
}