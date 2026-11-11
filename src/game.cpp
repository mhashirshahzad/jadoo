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
    char prevBuffer[500][500] = {{'\0'}}; // edited it to 500 by 500
    char newBuffer[500][500] = {{'\0'}};
    // draws on top of the buffer to make it ultra ultra cool
    // todo by @hashir
    char fxBuffer[500][500] = { {'\0'}};

    int prevCols = 0, prevLines = 0;
    int newCols = 0, newLines = 0;
    int keys = 0;
    
    // hides terminal cursor
    cout << "\033[?25l";
    
    int playerXPos = 1, playerYPos = 1;
    changeCurrentLevel(level3);
    ClearScreen();

    while (shouldRun) {
        GetMaxWindowSize(newCols, newLines);
        
        // if window is resized we have to do this :/
        if (newCols != prevCols || newLines != prevLines) {
            ClearScreen();

            resetBuffer(prevBuffer, newLines, newCols);        
            
            prevCols = newCols;
            prevLines = newLines;
        }

        resetBuffer(newBuffer, newLines, newCols);        
        PlaceCursor(1, 1); // was (0,0) test changed it 
        int keyCode = handleInput(playerXPos, playerYPos, keys, shouldRun);

        paintMap(playerXPos, playerYPos, newBuffer);
        paintUI(keyCode, newBuffer);

        renderBuffer(prevBuffer, newBuffer);
        cout.flush();
        ConsoleSleep(16.67f);
    }
}

void paintMap(int &playerXPos, int &playerYPos, char newBuffer[500][500]) {
    for (int i = 0; i < currentLevelSize; i++) {
        for (int j = 0; j < currentLevelSize; j++) {
            newBuffer[i][j] = currentLevel[i][j];
        }
    }    
    newBuffer[playerYPos][playerXPos] = 'P';
    
}

void paintUI(int keyCode, char newBuffer[500][500]) {
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
        // middle will need text later
        newBuffer[mid][c] = ' ';
    }

    // TEXT -> a is 10, b is 20 & c is 37
    char str[100] = "Columns: a / Rows: b / Key pressed: c";

    for (int c = 1; c <= cols - 2 && str[c - 1] != '\0'; c++) {
        newBuffer[mid][c] = str[c - 1];
    }
    newBuffer[mid][10] = cols;
    newBuffer[mid][20] = lines;
    newBuffer[mid][37] = keyCode;
    
}

void renderBuffer(char prevBuffer[500][500], char newBuffer[500][500]) {
    int cols = 0, lines = 0;
    GetMaxWindowSize(cols, lines);
    // renaming for ease
    int top = lines - 3, mid = lines - 2, bottom = lines - 1;

    for (int i = 0; i < lines && i < 500; i++) {
        for (int j = 0; j < cols && j < 500; j++) {

            if (newBuffer[i][j] == prevBuffer[i][j])
                ; // skip drawing
            else if (newBuffer[i][j] == 'P') {
                PrintColoredCharAtPoint(j + 1, i + 1, 'P', CONSOLE_RED);
            }
            else if (newBuffer[i][j] == 'k') {
                PrintColoredCharAtPoint(j + 1, i + 1, 'k', CONSOLE_GREEN);
            }
            else if (newBuffer[i][j] == 'D') {
                PrintColoredCharAtPoint(j + 1, i + 1, 'D', CONSOLE_GREEN | CONSOLE_RED);
            }
            else if (newBuffer[i][j] == '+') {
                PrintColoredCharAtPoint(j + 1, i + 1, '+', CONSOLE_GREEN);
            }
            else {
                PlaceCursor(j + 1, i + 1);
                cout << newBuffer[i][j];
            }
            prevBuffer[i][j] = newBuffer[i][j];
        }
    }
}

int handleInput(int& xPos, int& yPos, int& keys, bool& shouldRun) {

    int key = CheckKeyPressed(10);

    if (key == ESCKEY || key == 'q') {
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
    if (newX >= 0 && newX < currentLevelSize && newY >= 0 && newY < currentLevelSize) {

        // this is if that point is walkable

        char des = currentLevel[newY][newX];
        if (des == ' ') {
            xPos = newX;
            yPos = newY;
        }
        else if (des == 'k') {
            keys++;
            currentLevel[newY][newX] = ' ';
            
            xPos = newX;
            yPos = newY;
        }
        else if (des == 'D' && keys > 0) {
            keys--;
            currentLevel[newY][newX] = ' ';

            xPos = newX;
            yPos = newY;
        }
    }
    return key; 
}

void resetBuffer(char buffer[500][500], int newLines, int newCols, char resetWith) {
    for (int i = 0; i < newLines; i++)
        for (int j = 0; j < newCols; j++)
            buffer[i][j] = resetWith;
}
