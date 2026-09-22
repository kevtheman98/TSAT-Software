#include "sensors.h" // Pulls in our own declarations above, so this file knows what it's implementing
#include <Wire.h> // I2C library

Adafruit_LSM9DS1 lsm = Adafruit_LSM9DS1(); 
// Create "lsm" object to represent physical sensor in code.
// All sensor interactions will go through this object.

bool initSensor() { // Runs the sensor setup. Returns true/false so main.cpp knows if it worked.
    if (!lsm.begin()) { // Initialize the sensor over I2C. If it fails, return false.
        return false;
    }

    // At this point, the sensor is successfully initialized and ready for use.

    lsm.setupAccel(lsm.LSM9DS1_ACCELRANGE_2G); 
    // Sets accelerometer's measurement range to ±2g. This is the default setting, but it's good practice to set it explicitly.
    // Smaller range (2G) provides higher precision for small movements, while larger ranges (4G, 8G, 16G) allow for measuring more extreme accelerations but with less precision.
    lsm.setupMag(lsm.LSM9DS1_MAGGAIN_4GAUSS); 
    // Sets the magnetometer's gain to ±4 gauss. 
    // Lower gauss range gives higher sensitivity for detecting small magnetic fields, while higher ranges (8G, 12G, 16G) allow for measuring stronger magnetic fields but with less precision.
    lsm.setupGyro(lsm.LSM9DS1_GYROSCALE_245DPS); 
    // Sets the gyroscope's range to ±245 degrees per second. (How fast it can measure rotation). Smaller range (245DPS) provides higher precision for slow rotations, while larger ranges (500DPS, 2000DPS) allow for measuring faster rotations but with less precision.

    return true; // Everything succeeded
}

void readSensor(sensors_event_t &a, sensors_event_t &m, sensors_event_t &g, sensors_event_t &temp) {
    lsm.read(); // pulls fresh accel/gyro/mag data into the sensor object (all 3 sub sensors)
    lsm.getEvent(&a, &m, &g, &temp);
    // Fills in all four containers above with the latest readings from lsm.read().
    // The "&" passes each variable by reference, so this function can write directly into them.
}