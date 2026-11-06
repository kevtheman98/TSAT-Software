// radio.h = the "menu" for the radio code. It lists what exists (settings,
// the packet layout, and the functions) without showing how they work.
// Other files #include this to use the radio.

// Without this line, including the file twice by accident causes errors.
#pragma once

// Gives us Arduino types and functions (uint8_t, millis(), pinMode, etc.)
#include <Arduino.h>

// ---------- PINS: change these to match your wiring ----------
// #define makes a nickname. Anywhere the code says RADIO_CS, it means 5.
// If you rewire, you only change the number here.
#define RADIO_CS    5    // CS (chip select): the ESP32 pulls this low to say "talking to the radio now"
#define RADIO_RST   4    // RST (reset): lets the code restart the radio chip
#define RADIO_IRQ   27   // G0 (interrupt): the radio taps the ESP32 on the shoulder
                         //   through this pin when it finishes something.
                         //   Must be a pin that supports interrupts.
// SCK, MISO, MOSI (the other SPI wires) use the ESP32's built-in
// default pins (18, 19, 23), so we don't set them here.

// ---------- NETWORK SETTINGS ----------
// Radios only accept packets from the same network ID.
// Both the T-SAT and the ground station must use the same NETWORK_ID.
#define NETWORK_ID  100
#define TX_NODE_ID  1    // this board's address (the T-SAT transmitter)
#define RX_NODE_ID  2    // the ground station's address (the receiver)

// ---------- PACKET LAYOUT ----------
// A struct groups several variables into one block of memory. We send this
// whole block through the air as raw bytes, which is much smaller than text.
// "packed" tells the compiler to add no padding between fields, so the
// sender and receiver agree exactly on where each field sits.
// Total size: 2 + 4 + 2 + 2 + 1 = 11 bytes (the radio allows up to 61).
struct __attribute__((packed)) Telemetry {
  uint16_t seq;        // packet number (0 to 65535); the receiver sees gaps if
                       //   packets were lost. Filled in automatically.
  uint32_t t_ms;       // time since the board booted, in milliseconds (millis())
  int16_t  alt_dm;     // altitude in decimeters (tenths of a meter); 123 = 12.3 m.
                       //   Integers are smaller than floats, so we scale instead.
  int16_t  accel_mg;   // acceleration magnitude in milli-g; 1000 = 1.0 g
  uint8_t  phase;      // which mission phase we're in (a small number, 0-255)
};

// Function declarations: these promise the functions exist in radio.cpp.
bool radioInit();                       // call once at startup; returns true if the radio started
void radioSendTelemetry(Telemetry &t);  // call each time you want to send a packet