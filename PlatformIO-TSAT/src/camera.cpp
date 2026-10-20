#include "camera.h"
#include <SPI.h>
#include "Arducam_Mega.h"

// ESP32 default SPI pins, with CS on 17
const int PIN_SCK  = 18;
const int PIN_MISO = 19;
const int PIN_MOSI = 23;
const int PIN_CS   = 17;

Arducam_Mega myCAM(PIN_CS);

static const uint8_t FRAME_MARKER[4] = {0xFF, 0xAA, 0x55, 0xFF};

bool setupCamera() {
  Serial.println(F("SPI begin..."));
  SPI.begin(PIN_SCK, PIN_MISO, PIN_MOSI, PIN_CS);

  Serial.println(F("Camera begin..."));
  CamStatus status = myCAM.begin();
  Serial.println(F("Camera begin returned."));

  if (status != CAM_ERR_SUCCESS) {
    Serial.println(F("Can't find Arducam Mega!"));
    return false;
  }
  Serial.println(F("Arducam Mega detected."));
  return true;
}

void captureAndStream() {
  // 1. Capture a JPEG (blocks until the frame is stored in the camera's buffer)
  CamStatus status = myCAM.takePicture(CAM_IMAGE_MODE_QVGA, CAM_IMAGE_PIX_FMT_JPG);
  if (status != CAM_ERR_SUCCESS) {
    return; // capture failed; skip this frame
  }

  // 2. Verify image size
  uint32_t length = myCAM.getTotalLength();
  if (length == 0) {
    return;
  }

  // 3. Send header: [4-byte marker][4-byte little-endian length]
  Serial.write(FRAME_MARKER, 4);
  Serial.write((uint8_t*)&length, 4);

  // 4. Read the image out of the camera in chunks and stream it
  const uint8_t CHUNK = 255; // readBuff takes a uint8_t length, so 255 is the max
  static uint8_t buf[CHUNK];
  uint32_t remaining = length;

  while (remaining > 0) {
    uint8_t want = (remaining > CHUNK) ? CHUNK : (uint8_t)remaining;
    uint8_t got = myCAM.readBuff(buf, want);
    if (got == 0) {
      break; // camera stopped returning data; avoid hanging
    }
    Serial.write(buf, got);
    remaining -= got;
  }
}