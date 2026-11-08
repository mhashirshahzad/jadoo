#include <asm-generic/ioctls.h>
#include <iostream>
#include <chrono>
#include <unistd.h>
#include <sys/ioctl.h>

#include "levels.hpp"
#include "game.hpp"

#include "lib/myconsole.h"
#include "lib/mygraphics.h"

using namespace std;

void gameLoop() {
    ClearScreen();
    using clock = std::chrono::steady_clock;
    constexpr auto frame_time = std::chrono::microseconds(16667);
    
    while(true) {
        auto start = clock::now();
        ClearScreen();
        PlaceCursor(0, 0);
        render();
        renderText();

        auto elapsed = clock::now() - start;

        if (elapsed < frame_time)
            usleep(std::chrono::duration_cast<std::chrono::microseconds>(frame_time - elapsed).count());
    }
}


void renderText() {
    int cols = 0, lines = 0;
    GetMaxWindowSize(cols, lines);

    int top = lines - 3;
    int bottom = lines - 1;

    // Top border
    PlaceCursor(1, top);
    cout << "+";
    for(int i = 0; i < cols - 2; i++)
        cout << "-";
    cout << "+";

    // Side borders
    PlaceCursor(1, top + 1);
    cout << "|";

    PlaceCursor(cols, top + 1);
    cout << "|";

    // Text
    PlaceCursor(2, top + 1);
    cout << " Columns: " << cols << " | Rows: " << lines;

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

