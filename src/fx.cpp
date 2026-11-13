// #include <iostream>
// using namespace std;

// #include "fx.hpp"
// #include "lib/myconsole.h"

// char fxBuffer[500][500] = {{'\0'}};



// void renderFx(int i, int j) {
//   PlaceCursor(j + 1, i + 1);
//   cout << fxBuffer[i][j];

//   if (i < 499 && j < 499)
//     fxBuffer[i + 1][j + 1] = fxBuffer[i][j];
  
//   fxBuffer[i][j] = '\0';
// }
//  static void dirty(char prevBuffer[500][500], int x, int y) {
//    if (x >= 0 && y >= 0 && x < 500 && y < 500)
//      prevBuffer[y][x] = '\0';
//  }

//  void updateFx(char prevBuffer[500][500]) {
//    int cols = 0, lines = 0;
//    GetMaxWindowSize(cols, lines);

//    for (int k = 0; k < MAX_PARTICLES; k++) {
//      Particle &p = particles[k];
//      if (!p.active) continue;

//      // old cell gets repainted by renderBuffer on the next frame
//      dirty(prevBuffer, p.oldX, p.oldY);

//      p.x += p.vx;
//      p.y += p.vy;
//      p.vx *= 0.85f;
//      p.vy *= 0.85f;
//      p.life--;

//      int ix = (int)(p.x + 0.5f);
//      int iy = (int)(p.y + 0.5f);

//      if (p.life <= 0 || ix < 0 || iy < 0 || ix >= cols || iy >= lines) {
//        p.active = false;
//        continue;
//      }

//      float t = p.life / (float)p.maxLife;
//      char draw = t > 0.75f ? p.ch : t > 0.5f ? '*' : t > 0.25f ? '+' : '.';

//      PlaceCursor(ix + 1, iy + 1);
//      cout << draw;
//      p.oldX = ix;
//      p.oldY = iy;
//    }
//    cout.flush();
//  }
