#include <iostream>


#include "levels.hpp"
#include "game.hpp"

#include "lib/myconsole.h"
#include "lib/mygraphics.h"

using namespace std;

void gameLoop() {
    int xPos = 1, yPos = 1;
    ClearScreen();

    while (true) {


        ClearScreen();
        PlaceCursor(1, 1); // was (0,0) test changed it 
        movePlayer(xPos, yPos);
        render(xPos, yPos);
        renderText();
        ConsoleSleep(16.67f);
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


void render(int xPos,int yPos) {
    //for (int i = 0; i < 30; i++) {
    //    cout << level1[i];
    //    cout << "\033[K";
    //    cout << "\r\n";
    //}
    for (int i = 0; i < 30; i++) {
        for (int j = 0; j < 30; j++) {

            if (i == yPos && j == xPos) {
                cout << 'P';
            }
            else {
                cout << level1[i][j];
            }
        }

        cout << "\n";
    }


    cout.flush();

}

void movePlayer(int &xPos, int &yPos) {

    //TODO this :3
    int key = CheckKeyPressed(10);
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

        if (level1[newY][newX] == ' ') {
            xPos = newX;
            yPos = newY;
        }
    }

}
