#include <iostream>


#include "levels.hpp"
#include "game.hpp"

#include "lib/myconsole.h"
#include "lib/mygraphics.h"

using namespace std;

void gameLoop() {
    bool shouldRun = true;

    // buffer system to prevent flickering
    // [y][x] is the system blame @hashir not me 
    char prevBuffer[1000][1000] = {{'\0'}};
    char newBuffer[1000][1000] = {{'\0'}};

    // hides terminal cursor
    // check on windows
    cout << "\033[?25l";
    
    int playerXPos = 1, playerYPos = 1;
    ClearScreen();

    while (shouldRun) {
        int cols = 0;
        int lines = 0;
        PlaceCursor(1, 1); // was (0,0) test changed it 
        int keyCode = handleInput(playerXPos, playerYPos, shouldRun);

        paintMap(playerXPos, playerYPos, newBuffer);
        paintUI(keyCode, newBuffer);

        renderBuffer(prevBuffer, newBuffer);
        // render(playerXPos, playerYPos);
        // renderText(keyCode);
        cout.flush();
        ConsoleSleep(16.67f);
    }
}

void paintMap(int &playerXPos, int &playerYPos, char newBuffer[1000][1000]) {
    for (int i = 0; i < 30; i++) {
        for (int j = 0; j < 30; j++) {
            newBuffer[i][j] = level1[i][j];
        }
    }    
    newBuffer[playerYPos][playerXPos] = 'P';
    
}

void paintUI(int keyCode, char newBuffer[1000][1000]) {
    int cols = 0, lines = 0;
    GetMaxWindowSize(cols, lines);

    // renaming for ease
    int top = lines - 3;
    int mid = lines - 2;
    int bottom = lines - 1;

    // Top border
    newBuffer[top][0] = '+';
    newBuffer[top][cols - 1] = '+';

    // Bottom border
    newBuffer[bottom][0] = '+';
    newBuffer[bottom][cols - 1] = '+';

    // Side borders
    newBuffer[mid][0] = '|';
    newBuffer[mid][cols - 1] = '|';

    // fill bottom, top and mid
    for(int c = 1; c <= cols - 2; c++) {
        // top and bottom need -
        newBuffer[top][c] = '-';
        newBuffer[bottom][c] = '-';
        // middle will need text
        newBuffer[mid][c] = ' ';
    }


    // TODO TEXT
    // PlaceCursor(2, top + 1);
    // newBuffer[2][top+1] = 'C';
    // cout << " Columns: " << cols << " | Rows: " << lines;
    // cout << " | KeyPressed: " << (char)keyCode;

}

void renderBuffer(char prevBuffer[1000][1000], char newBuffer[1000][1000]) {
    int cols = 0;
    int lines = 0;
    GetMaxWindowSize(cols, lines);

    for (int i = 0; i < lines && i < 1000; i++) {
        for (int j = 0; j < cols && j < 1000; j++) {
            
            if (newBuffer[i][j] == 'P') {
                SetColorAtPoint(j + 1, i + 1, CONSOLE_RED);
                PlaceCursor(j + 1, i + 1);
                cout << 'P' << "\033[0m";
            } else if (newBuffer[i][j] != prevBuffer[i][j]) {
                PlaceCursor(j + 1, i + 1);
                cout << newBuffer[i][j];
            }
            prevBuffer[i][j] = newBuffer[i][j];
        }
    }
}

int handleInput(int &xPos, int &yPos, bool &shouldRun) {

    int key = CheckKeyPressed(10);

    if (key == ESCKEY) {
        shouldRun = false;
        return key;
    }
    // Player movement done by @ayaan @uzair
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
