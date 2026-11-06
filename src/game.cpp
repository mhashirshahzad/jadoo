#include <asm-generic/ioctls.h>
#include <iostream>
#include <chrono>
#include <unistd.h>
#include <sys/ioctl.h>

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

void moveCursor(int line = 0, int col = 0) {
    cout << "\033[" << line << ";" << col << "H";
}

void clearScreen() {
    std::cout << "\033[2J\033[H";
}

void renderText() {
    int cols = 0, lines = 0;
    getTerminalSize(cols, lines);

    int top = lines - 3;
    int bottom = lines - 1;

    // Top border
    moveCursor(top, 1);
    cout << "+" << string(cols - 2, '-') << "+";

    // Side borders
    moveCursor(top + 1, 1);
    cout << "|";

    moveCursor(top + 1, cols);
    cout << "|";

    // Text
    moveCursor(top + 1, 2);
    cout << " Columns: " << cols << " | Rows: " << lines;

    // Bottom border
    moveCursor(bottom, 1);
    cout << "+" << string(cols - 2, '-') << "+";

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

int getTerminalSize(int& cols, int& lines) {
    struct winsize ws;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 )
        return -1;

    cols = ws.ws_col;
    lines = ws.ws_row;
    return 0;
}
