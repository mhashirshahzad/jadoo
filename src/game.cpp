#include <iostream>
#include <chrono>
#include <unistd.h>
#include "levels.hpp"
#include "game.hpp"
using namespace std;

void gameLoop() {
    clearScreen();
    using clock = std::chrono::steady_clock;
    constexpr auto frame_time = std::chrono::microseconds(16667);
    
    while(true) {
        auto start = clock::now();
        moveCursor(0,0);
        render();
        renderText();

        auto elapsed = clock::now() - start;

        if (elapsed < frame_time)
            usleep(std::chrono::duration_cast<std::chrono::microseconds>(frame_time - elapsed).count());
    }
}

void moveCursor(int row = 0, int col = 0) {
    cout << "\033[" << row << ";" << col << "H";
}

void clearScreen() {
    std::cout << "\033[2J\033[H";
}

void renderText() {
    moveCursor(999, 1);
    cout << "Frame: 12, Size: 13";
}

void render() {
    for (int i = 0; i < 100; i++) {
        cout << level1[i];
        cout << "\n";
    }

    cout.flush();
}

