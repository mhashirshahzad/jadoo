#include <asm-generic/ioctls.h>
#include <iostream>
#include <unistd.h>
#include <sys/ioctl.h>

#include "levels.hpp"
#include "game.hpp"

#include "lib/myconsole.h"
#include "lib/mygraphics.h"

using namespace std;

void gameLoop() {
    int xPos = 0, yPos = 0;
    ClearScreen();

    while (true) {
        PlaceCursor(0, 0);
        movePlayer(xPos, yPos);
        render();
        renderText();
        ConsoleSleep(16.67);
    }
}



void renderText() {
    int cols = 0, lines = 0;
    GetMaxWindowSize(cols, lines);

    int top = lines - 2;
    int bottom = lines;

    // Top border
    PlaceCursor(1, top);
    cout << "+";
    for(int i = 0; i < cols - 2; i++)
        cout << "-";
    cout << "+";

    // Side borders
    PlaceCursor(1, top + 1);
    cout << "\033[2K"; // erase entire line before writing stuff
    
    cout << "|";

    PlaceCursor(cols, top + 1);
    cout << "|";

    // Text
    PlaceCursor(2, top + 1);
    cout << " Columns: " << cols << " | Rows: " << lines;
    int k = CheckKeyPressed(10);
    cout << " | KeyPressed: " << k;

    // Bottom border
    PlaceCursor(1, bottom);
    cout << "+";
    for(int i = 0; i < cols - 2; i++)
        cout << "-";
    cout << "+";

    cout.flush();
}


void render() {
    for (int i = 0; i < 30; i++) {
        cout << level1[i];
        cout << "\033[K";
        cout << "\r\n";
    }

    cout.flush();
}

void movePlayer(int &xPos, int &yPos) {
    //TODO this :3
}
