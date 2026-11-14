#include <iostream>
#include <cstdlib>
using namespace std;

#include "fx.hpp"
#include "lib/myconsole.h"


//  @TODO FIX THIS SO HARD PLZ
// works but i am not happy :SOB
// we need coin up/ key up
// die and door open and fall plz
// also all of these look so awful :sob:
int fxBuffer[500][500][8] = {{{0}}};
int fxTick = 0;
int fxFrame = 0;

void spawnExplosion(int row, int col, char ch, MYCOLORREF color) {
  int count = 3;
  int posRandomness = 2;
  int velRandomness = 3;
  int acclRandomness = 5;

  for (int n = 0; n < count; n++) {
    int x = col + rand() % posRandomness - 1;
    int y = row + rand() % posRandomness - 1;

    if (!(x < 0 || y < 0 || x >= 500 || y>= 500)) {
      int vx = rand() % velRandomness - 3;
      int vy = rand() % velRandomness -2;

      int ax = rand() % acclRandomness - 5;
      int ay = rand() % acclRandomness - 2;
      if (vx == 0 && vy == 0) vx = 1;
      
      fxBuffer[y][x][0] = ch;
      fxBuffer[y][x][1] = vx;
      fxBuffer[y][x][2] = vy;
      fxBuffer[y][x][3] = ax;
      fxBuffer[y][x][4] = ay;
      fxBuffer[y][x][5] = 4 + rand() % 5;
      fxBuffer[y][x][6] = fxTick;
      fxBuffer[y][x][7] = color;
    }
  }
}

void spawnCircular(int row, int col, char ch, MYCOLORREF color) {
  int count = 10;
  int posRandomness = 2;
  int velRandomness = 5;
  int acclRandomness = 5;

  for (int n = 0; n < count; n++) {
    int x = col + rand() % posRandomness - 1;
    int y = row + rand() % posRandomness - 1;

    if (!(x < 0 || y < 0 || x >= 500 || y>= 500)) {
      int vx = rand() % velRandomness - 3;
      int vy = rand() % velRandomness -2;

      int ax = rand() % acclRandomness - 5;
      int ay = rand() % acclRandomness - 2;
      if (vx == 0 && vy == 0) vx = 1;

      
      fxBuffer[y][x][0] = ch;
      fxBuffer[y][x][1] = vx;
      fxBuffer[y][x][2] = vy;
      fxBuffer[y][x][3] = ax;
      fxBuffer[y][x][4] = ay;
      fxBuffer[y][x][5] = 4 + rand() % 5;
      fxBuffer[y][x][6] = fxTick;
      fxBuffer[y][x][7] = color;
    }
  }
}
void spawnCollected(int row, int col, char ch, MYCOLORREF color) {
  int count = 1;

  for (int n = 0; n < count; n++) {
    int x = col;
    int y = row - 1;

    if (!(x < 0 || y < 0 || x >= 500 || y>= 500)) {
      int vx = 0;
      int vy = -2;

      if (vx == 0 && vy == 0) vx = 1;
      fxBuffer[y][x][0] = ch;
      fxBuffer[y][x][1] = vx;
      fxBuffer[y][x][2] = vy;
      fxBuffer[y][x][3] = 0;
      fxBuffer[y][x][4] = 2;
      fxBuffer[y][x][5] = 2 + rand() % 3;
      fxBuffer[y][x][6] = fxTick;
      fxBuffer[y][x][7] = color;

    }
  }
}

void spawnOpened(int row, int col, char ch, MYCOLORREF color) {
  int count = 1;

  for (int n = 0; n < count; n++) {
    int x = col;
    int y = row - 2;

    if (!(x < 0 || y < 0 || x >= 500 || y>= 500)) {
      int vx = rand() % 1 - 1; // 1, 0 or -1
      int vy = -1;

      if (vx == 0 && vy == 0) vx = 1;
      fxBuffer[y][x][0] = ch;
      fxBuffer[y][x][1] = vx;
      fxBuffer[y][x][2] = vy;
      fxBuffer[y][x][3] = 0;
      fxBuffer[y][x][4] = 1;
      fxBuffer[y][x][5] = 10;
      fxBuffer[y][x][6] = fxTick;
      fxBuffer[y][x][7] = color;

    }
  }
}

void updateFx(char prevBuffer[500][500]) {
  // only mv every third frame
  if(fxFrame++ % 3 != 0) return;
  
  int cols = 0, lines = 0;
  GetMaxWindowCoordinates(cols, lines);
  fxTick++;

  for (int i = 0; i < lines && i < 500; i++) {
    for (int j = 0; j < cols && j < 500; j++) {

      // LOOK WHAT not having CONTINUES makes me do
      if(fxBuffer[i][j][0] == 0 || fxBuffer[i][j][6] == fxTick)
        ; // skip
      else {
        char ch = fxBuffer[i][j][0];       
        int velX = fxBuffer[i][j][1];       
        int velY = fxBuffer[i][j][2];       
        int acclX = fxBuffer[i][j][3];       
        int acclY = fxBuffer[i][j][4];       
        int life = fxBuffer[i][j][5] - 1; // decreasing life  
        MYCOLORREF color = fxBuffer[i][j][7];

        fxBuffer[i][j][0] = 0;
        prevBuffer[i][j] = '\1';

        // TERMINAL VELOCITY
        if (velX > 3) velX = 3;
        else if (velX < -3) velX = -3;
        
        if (velY > 3) velY = 3;
        else if (velY < -3) velY = -3;

        
        int newX = j + velX;
        int newY = i + velY;

        // accleration adds into velocity
        velX += acclX;
        velY += acclY;
        if (life <= 0 || newX < 0 || newY < 0 || newX >= cols || newY >= lines)
          ;
        else {
          
          fxBuffer[newY][newX][0] = ch;
          fxBuffer[newY][newX][1] = velX;
          fxBuffer[newY][newX][2] = velY;
          fxBuffer[newY][newX][3] = acclX;
          fxBuffer[newY][newX][4] = acclY;
          fxBuffer[newY][newX][5] = life;
          fxBuffer[newY][newX][6] = fxTick;
          fxBuffer[newY][newX][7] = color;

          char draw = '.';

          if (life > 4) draw = ch;
          else if (life > 2) draw = '*';
          else if (life == 0) draw = ' ';

          PrintColoredCharAtPoint(newX + 1, newY + 1, draw, color);
        }
        
      }
    } 
  }

  cout.flush();
}
