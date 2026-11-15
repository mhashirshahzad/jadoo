#ifndef FX_HPP
#define FX_HPP
#include "lib/myconsole.h"

// fxBuffer[xPos][yPos][index of value]
// 0 -> char (if 0 is unactive)
// 1 -> velocityX
// 2 -> velocityY
// 3 -> accelerationX
// 4 -> accelerationY
// 5 -> life
// 6 -> frame stamp
// 7 -> color
extern int fxBuffer[500][500][8];

// A circular explosion type thingi :o
void spawnExplosion(int row, int col, char ch, MYCOLORREF color = CONSOLE_INTENSITY);
// an alternative to spawnExplosion (used for player death :c)
void spawnCircular(int row, int col, char ch, MYCOLORREF color = CONSOLE_INTENSITY);
// a curtain which falls from up to down for fun :o
void spawnCurtain(int hLength, int vLength, char ch, MYCOLORREF color = CONSOLE_BG_INTENSITY);

// the item appears to move up as if its collected :O
void spawnCollected(int row, int col, char ch, MYCOLORREF color = CONSOLE_BG_GREEN);
// the item appears to move up and then down as seen in SMM2
void spawnOpened(int row, int col, char ch, MYCOLORREF color = CONSOLE_BG_RED);


// update the vfx
void updateFx(char prevBuffer[500][500]);

// checks if buffers are equal :sob:
bool areFxBuffersEqual(int i, int j);
void updatePrevBuffer(int i, int j);
#endif
