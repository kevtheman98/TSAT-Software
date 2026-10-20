#ifndef CAMERA_H
#define CAMERA_H

#include <Arduino.h>

// Initializes the SPI/I2C buses, resets the CPLD, and configures the OV5642
bool setupCamera();

// Flushes the FIFO buffer, triggers a capture, and streams the JPEG over Serial
void captureAndStream();

#endif