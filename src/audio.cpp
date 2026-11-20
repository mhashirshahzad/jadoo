#include "audio.hpp"
#include "lib/myconsole.h"

#include <thread>

// music from https://github.com/ShaneMcC/beeps/blob/master/monkeyisland.sh

//  melody[freq][dur]
int melody[100][2] = {
  {50, 150},
  {30, 150},
  {500, 150},
  {200, 150},
  {10, 150},
  {1000, 150},
  {50, 150},
  {30, 150},
  {500, 150},
  {200, 150},
  {10, 150},
  {1000, 150},
  {50, 150},
  {30, 150},
  {500, 150},
  {200, 150},
  {10, 150},
  {1000, 150},
  {50, 150},
  {30, 150},
  {500, 150},
  {200, 150},
  {10, 150},
  {1000, 150},
};


void playInBG() {
  for (int i = 0; i < 23; i++) {
    ConsoleBeep(melody[i][0], melody[i][1]);
    ConsoleSleep(melody[i][1]);
  }
}


void startPlayingBGMusic() {
    std::thread worker(playInBG);
    worker.detach();
}



