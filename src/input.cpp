#include <iostream>

using namespace std;

#include "lib/myconsole.h"
#include "levels.hpp"
#include "fx.hpp"
#include "input.hpp"



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
    else if  (key == 'R' || key == 'r') {
        levelRestart();
        spawnCircular(newY, newX, '-', CONSOLE_BG_RED);
        newX = 1;
        newY = 1;
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
            
            replaceEveryCharFromCurrentLevel('.', '*');
            deleteCharFromCurrentLevel(newY, newX, 'T');
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
            spawnCurtain(currentLevelSize, currentLevelSize, '#');
            xPos = 1;
            yPos = 1;
            keys = 0;
            return -200;
        }

    }
    return key; 
}

