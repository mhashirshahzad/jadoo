#include "audio.hpp"
#include "lib/myconsole.h"

#include <thread>

// music from https://github.com/ShaneMcC/beeps/blob/master/monkeyisland.sh

//  melody[freq][dur]
int melodyStart[100][2] = {
  {300, 150},
  {500, 150},
  {700, 150},
  {300, 150},
  {500, 150},
  {700, 150},
  {300, 150},
  {500, 150},
  {700, 150},
  {300, 150},
  {500, 150},
  {700, 150},
  {300, 150},
  {500, 150},
  {700, 150},
};
int melodyEnd[100][2] = {
  {800, 150},
  {650, 150},
  {500, 150},
  {800, 150},
  {650, 150},
  {500, 150},
  {800, 150},
  {650, 150},
  {500, 150},
  {800, 150},
  {650, 150},
  {500, 150},
  {800, 150},
  {650, 150},
  {500, 150},
};


void playInBG(int melody[100][2], int size) {
  for (int i = 0; i < size; i++) {
    ConsoleBeep(melody[i][0], melody[i][1]);
    ConsoleSleep(melody[i][1]);
  }
}


void startPlayingBGMusic() {
    std::thread worker(playInBG, melodyStart, 14);
    worker.detach();
}
void startPlayingEndMusic() {
    std::thread worker(playInBG, melodyEnd, 14);
    worker.detach();
}



