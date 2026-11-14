#include <iostream>


#include "levels.hpp"
#include "game.hpp"
#include "fx.hpp"

#include "lib/myconsole.h"

using namespace std;

void gameLoop() {

    #ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    #endif
    bool shouldRun = true;

    // buffer system to prevent flickering
    // [y][x] is the system blame @hashir not me 
    char prevBuffer[500][500] = {{'\0'}}; // edited it to 500 by 500
    char newBuffer[500][500] = {{'\0'}};

    int prevCols = 0, prevLines = 0;
    int newCols = 0, newLines = 0;
    int keysObtained = 0;
    
    // hides terminal cursor
    cout << "\033[?25l";
    
    int playerXPos = 1, playerYPos = 1;
    changeCurrentLevel(levels[currentLevelIndex]);
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
        int keyCode = handleInput(playerXPos, playerYPos, keysObtained, shouldRun, currentLevelIndex);

        if (keyCode == -200) {
            ClearScreen();
            resetBuffer(prevBuffer,newLines, newCols);
            resetBuffer(newBuffer,newLines, newCols);
        }

        paintMap(playerXPos, playerYPos, newBuffer);
        paintUI(keyCode, newBuffer, keysObtained);

        renderBuffer(prevBuffer, newBuffer);
        updateFx(prevBuffer);
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
    newBuffer[playerYPos][playerXPos] = '@';
    
}

void paintUI(int keyCode, char newBuffer[500][500], int &keysObtained) {
    int cols = 0, lines = 0;
    GetMaxWindowSize(cols, lines);

    // renaming for ease
    int top = lines - 3;
    int mid = lines - 2;
    int bottom = lines - 1;

    // Top border
    newBuffer[top][0] = '\1';
    newBuffer[top][cols - 1] = '\2';

    // Bottom border
    newBuffer[bottom][0] = '\3';
    newBuffer[bottom][cols - 1] = '\4';

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

    char str[100] = "Keys Obtained: b / Key pressed: c";

    for (int c = 1; c <= cols - 2 && str[c - 1] != '\0'; c++) {
        newBuffer[mid][c] = str[c - 1];
    }
    newBuffer[mid][16] = keysObtained;
    // TODO Show arrow keys instead
    newBuffer[mid][33] = keyCode;
    
    
}

void renderBuffer(char prevBuffer[500][500], char newBuffer[500][500]) {
    int cols = 0, lines = 0;
    GetMaxWindowSize(cols, lines);
    // renaming for ease
    int top = lines - 3, mid = lines - 2, bottom = lines - 1;

    for (int i = 0; i < lines && i < 500; i++) {
        for (int j = 0; j < cols && j < 500; j++) {

            char newChar = newBuffer[i][j];
            char prevChar = prevBuffer[i][j];

            if (newChar == prevChar)
                ; // skip drawing
            // make this switch TODO
            else switch (newChar) {
                case '@':
                    PrintColoredCharAtPoint(j + 1, i + 1, newChar, CONSOLE_RED);
                    break;
                case 'k':
                    PrintColoredCharAtPoint(j + 1, i + 1, newChar, CONSOLE_GREEN);
                    break;
                case 'D':
                    PrintColoredCharAtPoint(j + 1, i + 1, newChar, CONSOLE_RED);
                    break;
                case 'b':
                    PrintColoredCharAtPoint(j + 1, i + 1, newChar, CONSOLE_BLUE);
                    break;
                case 'g':
                    PrintColoredCharAtPoint(j + 1, i + 1, newChar, CONSOLE_GREEN);
                    break;
                case 'v':
                    PrintColoredCharAtPoint(j + 1, i + 1, newChar, CONSOLE_RED | CONSOLE_BLUE);
                    break;

                // ASCII blocks and walls ▲ ▼ ▶ ◀ 
                case '*':
                    PlaceCursor(j + 1, i + 1);
                    cout << "█";
                    break;
                case '^':
                    PlaceCursor(j + 1, i + 1);
                    cout << "ఠ";
                    break;
                case 'G':
                    SetColorAtPoint(j + 1, i + 1, CONSOLE_GREEN);
                    PlaceCursor(j + 1, i + 1);
                    cout << "▒";
                    SetColorAtPoint(j + 1, i + 1, CONSOLE_GREEN);
                    cout << "\033[0m";
                    break;
                case 'B':
                    SetColorAtPoint(j + 1, i + 1, CONSOLE_BLUE);
                    PlaceCursor(j + 1, i + 1);
                    cout << "▒";
                    SetColorAtPoint(j + 1, i + 1, CONSOLE_BLUE);
                    cout << "\033[0m";
                    break;
                case 'V':
                    SetColorAtPoint(j + 1, i + 1, CONSOLE_RED | CONSOLE_BLUE);
                    PlaceCursor(j + 1, i + 1);
                    cout << "▒";
                    SetColorAtPoint(j + 1, i + 1, CONSOLE_RED | CONSOLE_BLUE);
                    cout << "\033[0m";
                    break;

                // UI
                case '-':
                    PlaceCursor(j + 1, i + 1);
                    cout << "─";
                    break;
                case '\1':
                    PlaceCursor(j + 1, i + 1);
                    cout << "┌";
                    break;
                case '\2':
                    PlaceCursor(j + 1, i + 1);
                    cout << "┐";
                    break;
                case '\3':
                    PlaceCursor(j + 1, i + 1);
                    cout << "└";
                    break;
                case '\4':
                    PlaceCursor(j + 1, i + 1);
                    cout << "┘";
                    break;
                case '|':
                    PlaceCursor(j + 1, i + 1);
                    cout << "│";
                    break;
                default:
                    PlaceCursor(j + 1, i + 1);
                    cout << newChar;
                    break;
            }

            // fixes the UI to render an integer instead of the character :o
            if (i == mid && j == 16) {
                PlaceCursor(j + 1, i + 1);
                cout << (int)newChar;
            }

            prevBuffer[i][j] = newBuffer[i][j];
        }
    }
}

int handleInput(int& xPos, int& yPos, int& keys, bool& shouldRun, int &currentLevelNumber) {

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
        if (des == ' ' || des == '.') {
            xPos = newX;
            yPos = newY;
        }
        else if (des == 'k') {
            keys++;
            ConsoleBeep(1000, 100);
            currentLevel[newY][newX] = ' ';
            
            spawnCollected(newY, newX, 'k');
            xPos = newX;
            yPos = newY;
        }
        else if (des == 'D' && keys > 0) {
            keys--;
            ConsoleBeep(300, 10);
            spawnOpened(newY, newX, 'D');
            deleteCharFromCurrentLevel(newY, newX, des);
            xPos = newX;
            yPos = newY;
        }
        else if (des == 'T') {
            ConsoleBeep(1000, 10);
            
            for (int i = 0; i < currentLevelSize; i++)
            {
                for (int j = 0; j < currentLevelSize; j++)
                {
                    if (currentLevel[i][j] == '.') {
                        currentLevel[i][j] = '*';
                        currentLevel[yPos][xPos] = ' ';
                    }
                }
            }
        }

        // Spike elimination
        else if (des == '^') {
            levelRestart();
            xPos = 1; yPos = 1;
            spawnCircular(newY, newX, '-', CONSOLE_BG_RED);
            ConsoleBeep(2000, 200);

        }

        else if (des == 'g' || des == 'v' || des == 'b') {
            ConsoleBeep(300, 10);

            xPos = newX;
            yPos = newY;
            deleteEveryCharFromCurrentLevel(des);

        }

        else if (des == 'E') {


            //Now the beep is synchronized better :)
            cout.flush();

            ConsoleBeep(1200, 300);

            levelComplete();

            xPos = 1;
            yPos = 1;
            keys = 0;
            return -200;
        }

    }
    return key; 
}

void resetBuffer(char buffer[500][500], int newLines, int newCols, char resetWith) {
    for (int i = 0; i < newLines; i++)
        for (int j = 0; j < newCols; j++)
            buffer[i][j] = resetWith;
}
