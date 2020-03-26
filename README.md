# SMU CS106 Embedded Systems Lab 1

  

#### Question 1. A Time For Everything

- When the micro:bit is switched on, the LEDs display a numeric value, X
- Initially, X == 5
- Pressing A decrements X; pressing B increments X
- X is limited to the range 1 to 9 only; both 1 and 9 inclusive
- Pressing both buttons causes X to count down to zero, in  approximately  1s intervals
- When X reaches 0, the display stays at 0



#### Question 2. It’s the paradox that drives us all

- When the micro:bit is in a vertical position, a single LED lights up (LED X), on any corner of
  the micro:bit display. When the micro:bit is rotated clockwise, the LEDs along the edges
  light up, starting from LED X, X+1, X+2, etc. When the micro:bit is rotated anti-clockwise,
  the lighted LEDs will be unlit, from X+2, X+1, etc. When one full circle has been made, the
  display clears. If the micro:bit continues to be rotated clockwise, LED X lights up again, and
  the pattern repeats. 
- When the micro:bit is in a horizontal position, a blinking LED lights up in the center of the
  screen. By tilting the micro:bit, the blinking LED moves in the direction of tilt (in 4 directions
  only). The display initially shows a number 0. When the micro:bit is rotated clockwise, the
  number increments. When the micro:bit is rotated anti-clockwise, the number decrements.
  The number must remain in the range [0, 9]. If the blinking LED hits any of the edges, the
  number is reset to 0



#### How to Run

1. Install [`yotta`](http://docs.yottabuild.org/#installing) and `srecord` [[windows]](http://srecord.sourceforge.net/) [[ubuntu]](https://packages.ubuntu.com/bionic/srecord)

2. Build project dependencies:

```
> cd PROJECT_FOLDER
> yt target bbc-microbit-classic-gcc
> yt build
```

3. Copy hex file to micro:bit:

```
> cp build/bbc-microbit-classic-gcc/source/[PROJECT_FOLDER]-combined.hex [MICROBIT_PATH]
```

Example (Ubuntu):

```
> cp build/bbc-microbit-classic-gcc/source/cs102-lab1-question1-combined.hex /media/charlie/MICROBIT1
```