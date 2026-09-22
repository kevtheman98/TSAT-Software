#ifndef SENSORS_H // Include guard: prevents this file from being loaded more than once
#define SENSORS_H // if multiple files include "sensors.h"

#include <Adafruit_LSM9DS1.h> // Library for LSM9DS1 IMU sensor

extern Adafruit_LSM9DS1 lsm; 
// Tells other files "lsm" exists and what type it is, without creating it again.
// The actual object is created once, in sensors.cpp.

bool initSensor();
// Declares a function that sets up the sensor. Returns true if successful, false if not.
// The actual code for this function lives in sensors.cpp.

void readSensor(sensors_event_t &a, sensors_event_t &m, sensors_event_t &g, sensors_event_t &temp);
// Declares a function that fills in the four event containers with fresh sensor readings.
// The "&" means these are passed by reference, so the function can write directly into them.

#endif