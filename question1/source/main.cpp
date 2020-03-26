#include "MicroBit.h"

MicroBit uBit;

// microbit global variables
int MICROBIT_SLEEP = 50;

// q1 global variables
int X = 5;
bool PROGRAM_END = false;

// q1 button hold variables
int MICROBIT_BUTTON_BEING_HOLD = 0;
int MICROBIT_BUTTON_HOLD_SLEEP = 250;
bool MICROBIT_BUTTON_IS_HOLD = false;

// microbit global events
void button_onHold (MicroBitEvent);
void button_onClick (MicroBitEvent);
void button_onHoldLoop();

int main()
{
    uBit.init();
    uBit.messageBus.listen(MICROBIT_ID_ANY, MICROBIT_EVT_ANY, button_onHold);
    uBit.messageBus.listen(MICROBIT_ID_ANY, MICROBIT_EVT_ANY, button_onClick);
    while (!PROGRAM_END) {
        button_onHoldLoop();
        uBit.display.print(X);
        uBit.sleep(MICROBIT_SLEEP);
    }
}

void _decrementX() {
    if (X != 1) X--;
}

void _incrementX() {
    if (X != 9) X++;
}

void _countdownX() {
    uBit.messageBus.ignore(MICROBIT_ID_ANY, MICROBIT_EVT_ANY, button_onHold);
    uBit.messageBus.ignore(MICROBIT_ID_ANY, MICROBIT_EVT_ANY, button_onClick);
    do uBit.sleep(1000);
    while (X-- != 0);
    PROGRAM_END = true;
}

void button_handlers (int button) {
    if (button == MICROBIT_ID_BUTTON_A) _decrementX();
    else if (button == MICROBIT_ID_BUTTON_B) _incrementX();
}

void button_onHold (MicroBitEvent e) {
    if (e.value == MICROBIT_BUTTON_EVT_HOLD) MICROBIT_BUTTON_IS_HOLD = true;
    else if (e.value == MICROBIT_BUTTON_EVT_UP) MICROBIT_BUTTON_IS_HOLD = false;
}

void button_onClick (MicroBitEvent e) {
    MICROBIT_BUTTON_BEING_HOLD = e.source;
    if (MICROBIT_BUTTON_BEING_HOLD == MICROBIT_ID_BUTTON_AB) _countdownX();
    if (e.value == MICROBIT_BUTTON_EVT_CLICK) button_handlers(MICROBIT_BUTTON_BEING_HOLD);
}

void button_onHoldLoop() {
    if (MICROBIT_BUTTON_IS_HOLD) {
        button_handlers (MICROBIT_BUTTON_BEING_HOLD);
        uBit.sleep(MICROBIT_BUTTON_HOLD_SLEEP);
    }
}
