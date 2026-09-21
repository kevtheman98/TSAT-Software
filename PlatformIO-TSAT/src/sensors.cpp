#include <Arduino.h> // Arduino core library
#include <Wire.h> // I2C library
#include <Adafruit_LSM9DS1.h> // Library for LSM9DS1 IMU sensor

Adafruit_LSM9DS1 lsm = Adafruit_LSM9DS1(); 
// Create "lsm" object to represent physical sensor in code.
// All sensor interactions will go through this object.

void setup(){ // setup() runs ONCE, when ESP32 is powered on or reset
    Serial.begin(115200); // Start serial communication at 115200 baud (bits per second)
    delay(1000); // Wait for serial port to connect (for native USB devices)
    
    if (!lsm.begin()) { // Initialize the sensor over I2C. If it fails, print error and halt execution.
        Serial.println("Failed to communicate with LSM9DS1 sensor, check wiring!");
        while (1) delay(10); // Infinite loop to halt execution if sensor initialization fails.
    }
    
    // At this point, the sensor is successfully initialized and ready for use.

    lsm.setupAccel(lsm.LSM9DS1_ACCELRANGE_2G); 
    // Sets accelerometer's measurement rnage to ±2g. This is the default setting, but it's good practice to set it explicitly.
    // Smaller range (2G) provides higher precision for small movements, while larger ranges (4G, 8G, 16G) allow for measuring more extreme accelerations but with less precision.
    lsm.setupMag(lsm.LSM9DS1_MAGGAIN_4GAUSS); 
    // Sets the magnetometer's gain to ±4 gauss. 
    // Lower gauss range gives higher sensitivity for detecting small magnetic fields, while higher ranges (8G, 12G, 16G) allow for measuring stronger magnetic fields but with less precision.
    lsm.setupGyro(lsm.LSM9DS1_GYROSCALE_245DPS); 
    // Sets the gyroscope's range to ±245 degrees per second. (How fast it can measure rotation). Smaller range (245DPS) provides higher precision for slow rotations, while larger ranges (500DPS, 2000DPS) allow for measuring faster rotations but with less precision.

    Serial.println("LSM9DS1 sensor initialized successfully!");
}

void loop() { // loop() runs repeatedly for as long as ESP32 is powered on
  lsm.read();  // pulls fresh accel/gyro/mag data into the sensor object (all 3 sub sensors)

  sensors_event_t a, m, g, temp;
  // Creates four separate "event" containers to hold the results
  // accelerometer (a), magnetometer (m), gyroscope (g), and temperature (temp).
  // sensors_event_t is a standard Adafruit struct type used across their sensor libraries.
  lsm.getEvent(&a, &m, &g, &temp);
  // Fills in all four containers above with the latest readings from lsm.read().
  // The "&" passes each variable by reference, so this function can write directly into them.

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

