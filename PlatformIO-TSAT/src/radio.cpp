// radio.cpp = the actual radio code (the "kitchen" behind the menu in radio.h).

#include "radio.h"   // our own settings and packet layout
#include <SPI.h>     // SPI = the 4-wire protocol the ESP32 uses to talk to the radio
#include <RFM69.h>   // the library that handles the radio chip's registers for us

// Create the radio object. This does not start it yet; it just stores our
// pin choices. "true" means "this is the HCW (high power) version".
// "static" means this variable is private to this file.
static RFM69 radio(RADIO_CS, RADIO_IRQ, true);

// Counts packets sent. Static so it remembers its value between function calls.
static uint16_t seq = 0;

// Starts the radio. Call this once in setup().
// Returns true on success and false if the radio didn't respond.
bool radioInit() {

  // --- Hardware reset ---
  // Putting the chip in a known clean state before use is good practice.
  // On the RFM69, a HIGH pulse on RST resets it, and LOW lets it run.
  pinMode(RADIO_RST, OUTPUT);     // we'll drive this pin, so make it an output
  digitalWrite(RADIO_RST, HIGH);  // reset on
  delay(10);                      // hold for 10 ms
  digitalWrite(RADIO_RST, LOW);   // reset off
  delay(10);                      // give the chip time to wake up

  // --- Start the radio ---
  // Arguments: frequency band (915 MHz for the US), this board's address,
  // and the network ID. It returns false if it can't talk to the chip,
  // which usually means a wiring problem.
  if (!radio.initialize(RF69_915MHZ, TX_NODE_ID, NETWORK_ID)) {
    Serial.println("Radio init FAILED - check wiring/antenna/power");
    return false;
  }

  // The HCW is the high-power version (up to +20 dBm). This must be called,
  // or the radio will transmit weakly or not at all.
  radio.setHighPower();

  Serial.println("Radio ready @ 915 MHz");
  return true;
}

// Fills in the automatic fields and sends one packet.
// The "&" means we edit the caller's struct directly instead of copying it,
// so the caller can see the seq number we set.
void radioSendTelemetry(Telemetry &t) {

  t.seq = seq++;       // use the current count, then add 1 for next time
  t.t_ms = millis();   // millis() = milliseconds since the board powered on

  // Send the struct as raw bytes to the ground station.
  //   RX_NODE_ID   = who it's for
  //   &t           = where the data starts in memory
  //   sizeof(t)    = how many bytes to send (11)
  // We don't ask for an acknowledgement (ACK). For a live stream, the next
  // reading arrives in 100 ms anyway, so a retry of an old one isn't worth it.
  radio.send(RX_NODE_ID, &t, sizeof(t));
}