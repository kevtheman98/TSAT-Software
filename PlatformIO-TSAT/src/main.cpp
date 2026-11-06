#include <Arduino.h>
#include "radio.h"   // lets main.cpp use radioInit() and radioSendTelemetry()

void setup() {
  // runs once at startup
  Serial.begin(115200);  // start the Serial Monitor; must match monitor_speed
  radioInit();           // start the radio (prints success or failure)
}

void loop() {
  // runs over and over

  // "static" keeps this variable's value between loops instead of resetting it.
  // It remembers when we last transmitted.
  static uint32_t lastTx = 0;

  // A non-blocking timer: instead of delay(100), which would freeze everything
  // else (sensors, state machine), we check the clock and only act every 100 ms.
  if (millis() - lastTx >= 100) {   // 100 ms = 10 packets per second
    lastTx = millis();              // remember when we sent

    Telemetry t = {};               // = {} sets every field to 0 before we fill it in
    t.alt_dm = 0;                   // dummy altitude for testing; replace with real data later
    t.accel_mg = 1000;              // dummy acceleration (1 g, i.e. sitting still)
    t.phase = 0;                    // dummy phase
    radioSendTelemetry(t);          // seq and time are filled in inside this function
  }
}