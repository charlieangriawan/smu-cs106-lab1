#include "MicroBit.h"
#include <math.h>

MicroBit uBit;

// microbit global variables
int MICROBIT_SLEEP = 10;
int MICROBIT_ORIENTATION = 0;

// microbit global events
void microbit_onTilt(MicroBitEvent);
void microbit_resetState();
void start_verticalParadox();
void start_horizontalParadox();
void _tilting();

// vertical paradox global variables
bool VERTICAL_GAMEOVER = false;
int VERTICAL_ROTATION_ANGLE_PREV = 0;
int VERTICAL_ROTATION_ANGLE_INIT = 0;
int VERTICAL_ROTATION_DIRECTION = 0;

// horizontal tilting global variables
int HORIZONTAL_TILT_LED = 0;
int HORIZONTAL_TILT_PREV_X = -1;
int HORIZONTAL_TILT_PREV_Y = -1;
int HORIZONTAL_TILT_BLINK_INTERVAL = 0;
int HORIZONTAL_TILT_BLINKING_INTERVAL = 800;
int HORIZONTAL_TILT_BLINKOFF_INTERVAL = HORIZONTAL_TILT_BLINKING_INTERVAL * 3/4;

// horizontal paradox global variables
int HORIZONTAL_ROTATION_DIRECTION = 0;
int HORIZONTAL_ROTATION_ANGLE_PREV = 0;
int HORIZONTAL_ROTATION_ANGLE_INIT = 0;
int HORIZONTAL_ROTATION_ANGLE_TOTAL = 0;

int main()
{
    uBit.init();
    uBit.messageBus.listen(MICROBIT_ID_GESTURE, MICROBIT_EVT_ANY, microbit_onTilt);
    while (true) {
      if (MICROBIT_ORIENTATION == 1) start_verticalParadox();
      else if (MICROBIT_ORIENTATION == -1) start_horizontalParadox();
      uBit.sleep(MICROBIT_SLEEP);
    }
}

void microbit_onTilt(MicroBitEvent e) {
  if (e.value == MICROBIT_ACCELEROMETER_EVT_NONE) {
    return;
  } else if (e.value == MICROBIT_ACCELEROMETER_EVT_FACE_UP || e.value == MICROBIT_ACCELEROMETER_EVT_FACE_DOWN) {
    if (MICROBIT_ORIENTATION == 1) microbit_resetState();
    MICROBIT_ORIENTATION = -1;
  } else if (e.value == MICROBIT_ACCELEROMETER_EVT_TILT_UP || e.value == MICROBIT_ACCELEROMETER_EVT_TILT_DOWN ||
             e.value == MICROBIT_ACCELEROMETER_EVT_TILT_LEFT || e.value == MICROBIT_ACCELEROMETER_EVT_TILT_RIGHT) {
    if (MICROBIT_ORIENTATION == -1) microbit_resetState();
    MICROBIT_ORIENTATION = 1;
  }
}

void microbit_resetState() {
  uBit.display.clear();
  VERTICAL_ROTATION_ANGLE_PREV = 0;
  VERTICAL_ROTATION_ANGLE_INIT = atan2 (uBit.accelerometer.getY(), uBit.accelerometer.getX()) * 180 / 3.14159265;
  VERTICAL_GAMEOVER = false;
  VERTICAL_ROTATION_DIRECTION = 0;
  HORIZONTAL_TILT_LED = 0;
  HORIZONTAL_TILT_PREV_X = 0;
  HORIZONTAL_TILT_PREV_Y = 0;
  HORIZONTAL_TILT_BLINK_INTERVAL = 0;
  HORIZONTAL_ROTATION_DIRECTION = 0;
  HORIZONTAL_ROTATION_ANGLE_TOTAL = 0;
  HORIZONTAL_ROTATION_ANGLE_PREV = 0;
  HORIZONTAL_ROTATION_ANGLE_INIT = uBit.compass.heading();
}

void _lightUp(int x, int y) {
  if (VERTICAL_ROTATION_DIRECTION == 1) uBit.display.image.setPixelValue(x, y, 255);
  else uBit.display.image.setPixelValue(y, x, 255);
}

void _showBox(int angle) {
  uBit.display.clear();

  if (VERTICAL_ROTATION_DIRECTION == -1) angle *= -1;
  if (angle < 0) angle += 360;

  if (VERTICAL_ROTATION_ANGLE_PREV == 0) VERTICAL_ROTATION_ANGLE_PREV = angle;
  int offset = angle - VERTICAL_ROTATION_ANGLE_PREV;
  if (offset > 50 || offset < -50) return microbit_resetState();
  VERTICAL_ROTATION_ANGLE_PREV = angle;

  int p = angle/20, x = 0, y = 0;

  if (p >=  1) _lightUp(  x,   y);
  if (p >=  2) _lightUp(++x,   y);
  if (p >=  3) _lightUp(++x,   y);
  if (p >=  4) _lightUp(++x,   y);
  if (p >=  5) _lightUp(++x,   y);
  if (p >=  6) _lightUp(  x, ++y);
  if (p >=  7) _lightUp(  x, ++y);
  if (p >=  8) _lightUp(  x, ++y);
  if (p >=  9) _lightUp(  x, ++y);
  if (p >= 10) _lightUp(--x,   y);
  if (p >= 11) _lightUp(--x,   y);
  if (p >= 12) _lightUp(--x,   y);
  if (p >= 13) _lightUp(--x,   y);
  if (p >= 14) _lightUp(  x, --y);
  if (p >= 15) _lightUp(  x, --y);

  if (p == 16) {
    _lightUp(x, --y);
    _lightUp(2, 2);
    VERTICAL_GAMEOVER = true;
  }
}

void start_verticalParadox() {
  int x = uBit.accelerometer.getX();
  int y = uBit.accelerometer.getY();
  int angle = (atan2 (y,x) * 180 / PI) - VERTICAL_ROTATION_ANGLE_INIT;

  if (VERTICAL_GAMEOVER) return;

  if (0 < angle && angle < 18) VERTICAL_ROTATION_DIRECTION = 1;
  else if (-18 < angle && angle < 0) VERTICAL_ROTATION_DIRECTION = -1;

  if (VERTICAL_ROTATION_DIRECTION == 1) _showBox(angle);
  else if (VERTICAL_ROTATION_DIRECTION == -1) _showBox(angle);
}


int _pixel_from_g(int value)
{
    int z = 0;
    if (value > -400) z++;
    if (value > -200) z++;
    if (value > 200) z++;
    if (value > 400) z++;
    return z;
}

void _blink(int x, int y) {
    if (HORIZONTAL_TILT_BLINK_INTERVAL < HORIZONTAL_TILT_BLINKING_INTERVAL) HORIZONTAL_TILT_BLINK_INTERVAL += MICROBIT_SLEEP;
    else HORIZONTAL_TILT_BLINK_INTERVAL = 0;

    if (HORIZONTAL_TILT_BLINK_INTERVAL < HORIZONTAL_TILT_BLINKOFF_INTERVAL) uBit.display.image.setPixelValue(x, y, 0);
    else if (HORIZONTAL_TILT_BLINK_INTERVAL >= HORIZONTAL_TILT_BLINKOFF_INTERVAL) uBit.display.image.setPixelValue(x, y, 255);
}

void _startTiltBlinker() {
    int x = _pixel_from_g(uBit.accelerometer.getX());
    int y = _pixel_from_g(uBit.accelerometer.getY());
    if (HORIZONTAL_TILT_PREV_X != x || HORIZONTAL_TILT_PREV_Y != y) {
        if (HORIZONTAL_TILT_LED == 0) uBit.display.image.setPixelValue(HORIZONTAL_TILT_PREV_X, HORIZONTAL_TILT_PREV_Y, 0);
        else uBit.display.image.setPixelValue(HORIZONTAL_TILT_PREV_X, HORIZONTAL_TILT_PREV_Y, 255);
        HORIZONTAL_TILT_LED = uBit.display.image.getPixelValue(x, y);
        HORIZONTAL_TILT_PREV_X = x;
        HORIZONTAL_TILT_PREV_Y = y;
    }
    _blink(x, y);
}

void start_horizontalParadox() {
  int angle = uBit.compass.heading() - HORIZONTAL_ROTATION_ANGLE_INIT;
  if (angle < 0) angle += 360;
  if (HORIZONTAL_ROTATION_ANGLE_PREV == 0) HORIZONTAL_ROTATION_ANGLE_PREV = angle;
  int changes = angle - HORIZONTAL_ROTATION_ANGLE_PREV;
  if (changes > 300) changes -= 360;
  else if (changes < -300) changes += 360;

  HORIZONTAL_ROTATION_ANGLE_TOTAL += changes;
  if (HORIZONTAL_ROTATION_ANGLE_TOTAL < 0 || HORIZONTAL_ROTATION_ANGLE_TOTAL > 360 * 10) HORIZONTAL_ROTATION_ANGLE_TOTAL = 0;

  uBit.display.print(HORIZONTAL_ROTATION_ANGLE_TOTAL / 360);
  HORIZONTAL_ROTATION_ANGLE_PREV = angle;
  _startTiltBlinker();
}
