#ifndef GAME_HPP
#define GAME_HPP
void gameLoop();

// paints the map onto the buffer including the player :O
void paintMap(int &playerXPos, int &playerYPos, char newBuffer[500][500]);

// same as paintMap but for the UI
void paintUI(int keyCode, char newBuffer[500][500]);

// handles input, both player and exiting
// also returns a keyCode
int handleInput(int &xPos, int &yPos, int &keys, bool &shouldRun);

// renders the difference btwn buffers and updates prevBuffer
void renderBuffer(char prevBuffer[500][500], char newBuffer[500][500]);

// replaces every element in buffer with resetWith
void resetBuffer(char buffer[500][500], int newLines, int newCols, char resetWith = '\0');

#endif
