#include <iostream>
#include <cstdlib>
using namespace std;

#include "fx.hpp"
#include "lib/myconsole.h"


//  @TODO FIX THIS SO HARD PLZ
// works but i am not happy :SOB:
// gotta find online better effects
// also COLORS PLZ
int fxBuffer[500][500][6] = {{{0}}};
int fxTick = 0;
int fxFrame = 0;

void spawnExplosion(int row, int col, char ch) {
  int count = 3;
  int posRandomness = 2;
  int velRandomness = 5;

  for (int n = 0; n < count; n++) {
    int x = col + rand() % posRandomness - 1;
    int y = row + rand() % posRandomness - 1;

    if (!(x < 0 || y < 0 || x >= 500 || y>= 500)) {
      int vx = rand() % velRandomness - 3;
      int vy = rand() % velRandomness -2;

      if (vx == 0 && vy == 0) vx = 1;
      fxBuffer[y][x][0] = ch;
      fxBuffer[y][x][1] = vx;
      fxBuffer[y][x][2] = vy;
      fxBuffer[y][x][3] = 4 + rand() % 5;
      fxBuffer[y][x][4] = fxTick;
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
      if(fxBuffer[i][j][0] == 0 || fxBuffer[i][j][4] == fxTick)
        ; // skip
      else {
        char ch = fxBuffer[i][j][0];       
        int velX = fxBuffer[i][j][1];       
        int velY = fxBuffer[i][j][2];       
        int life = fxBuffer[i][j][3] - 1; // decreasing life  
        

        fxBuffer[i][j][0] = 0;
        prevBuffer[i][j] = '\1';

        int newX = j + velX;
        int newY = i + velY;

        if (life <= 0 || newX < 0 || newY < 0 || newX >= cols || newY >= lines)
          ;
        else {
          
          fxBuffer[newY][newX][0] = ch;
          fxBuffer[newY][newX][1] = velX;
          fxBuffer[newY][newX][2] = velY;
          fxBuffer[newY][newX][3] = life;
          fxBuffer[newY][newX][4] = fxTick;

          char draw = '.';

          if (life > 4) draw = ch;
          else if (life > 2) draw = '*';
          else if (life == 0) draw = ' ';

          PlaceCursor(newX + 1, newY + 1);
          cout << draw;
        }
        
      }
    } 
  }

  cout.flush();
}
