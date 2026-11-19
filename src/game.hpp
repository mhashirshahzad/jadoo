#ifndef GAME_HPP
#define GAME_HPP
void gameLoop();

// paints the map onto the buffer including the player :O
// doesnt drawn on screen just adds to buffer
void paintMap(int &playerXPos, int &playerYPos, char newBuffer[500][500]);

// same as paintMap but for the UI
// doesn't draw on screen just adds to buffer
void paintUI(int keyCode, char newBuffer[500][500], int &keysObtained);


// handles input, both player and exiting
// also returns a keyCode
int handleInput(int &xPos, int &yPos, int &keys, bool &shouldRun, int &currentLevelNumber);

// renders the difference btwn buffers and updates prevBuffer
void renderBuffer(char prevBuffer[500][500], char newBuffer[500][500], int &playerXPos, int &playerYPos);

// replaces every element in buffer with resetWith
void resetBuffer(char buffer[500][500], int newLines, int newCols, char resetWith = '\0');

// draws special characters UNICODE instead of ascii stuff
// does j + 1 and i + 1 itself
void specialDraw(char newChar, int j, int i);
#endif
