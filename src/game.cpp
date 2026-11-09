#include <iostream>


#include "levels.hpp"
#include "game.hpp"

#include "lib/myconsole.h"
#include "lib/mygraphics.h"

using namespace std;

void gameLoop() {
    bool shouldRun = true;

    // buffer system to prevent flickering
    char prevBuffer[1000][1000] = {{'\0'}};
    char newBuffer[1000][1000] = {{'\0'}};

    // hides terminal cursor
    // check on windows
    cout << "\033[?25l";
    
    int playerXPos = 1, playerYPos = 1;
    ClearScreen();

    while (shouldRun) {
        ClearScreen();
        PlaceCursor(1, 1); // was (0,0) test changed it 
        int keyCode = handleInput(playerXPos, playerYPos, shouldRun);
        render(playerXPos, playerYPos);
        renderText(keyCode);
        cout.flush();
        ConsoleSleep(16.67f);
    }
}



void renderText(int keyCode = 0) {
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
    cout << " | KeyPressed: " << (char)keyCode;

    // Bottom border
    PlaceCursor(1, bottom);
    cout << "+";
    for(int i = 0; i < cols - 2; i++)
        cout << "-";
    cout << "+";

}


void render(int xPos,int yPos) {

    for (int i = 0; i < 30; i++) {
        for (int j = 0; j < 30; j++) {

            if (i == yPos && j == xPos) {

                SetColorAtPoint(xPos + 1, yPos + 1, CONSOLE_RED);
                cout << 'P';
                cout << "\033[0m";
            }
            else {
                cout << level1[i][j];
            }
        }

        cout << "\n";
    }
}

int handleInput(int &xPos, int &yPos, bool &shouldRun) {

    int key = CheckKeyPressed(10);

    if (key == ESCKEY) {
        shouldRun = false;
        return -1;
    }
    // Player movement
    int newX = xPos;
    int newY = yPos;

    if (key == 'W' || key == 'w' || key == UPKEY) {
        newY--;
    }
    else if (key == 'S' || key == 's' || key == DOWNKEY) {
        newY++;
    }
    else if (key == 'A' || key == 'a' || key == LEFTKEY) {
        newX--;
    }
    else if (key == 'D' || key == 'd' || key == RIGHTKEY) {
        newX++;
    }

    // This is for boundary colliding :|
    if (newX >= 0 && newX < 30 && newY >= 0 && newY < 30) {

        // this is if that point is walkable
        if (level1[newY][newX] == ' ') {
            xPos = newX;
            yPos = newY;
        }
    }

    return key;
}
