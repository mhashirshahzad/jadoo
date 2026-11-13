#ifndef FX_HPP
#define FX_HPP


// fxBuffer[xPos][yPos][index of value]
// 0 -> char (if 0 is unactive)
// 1 -> velocityX
// 2 -> velocityY
// 3 -> life
// 4 -> frame stamp
extern int fxBuffer[500][500][6];

void spawnExplosion(int row, int col, char ch);
void spawnCircular(int row, int col, char ch);

void updateFx(char prevBuffer[500][500]);
#endif
