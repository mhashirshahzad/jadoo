
void gameLoop();

// paints the map onto the buffer including the player :O
void paintMap(int &playerXPos, int &playerYPos, char newBuffer[1000][1000]);

// same as paintMap but for the UI
void paintUI(int keyCode, char newBuffer[1000][1000]);

// handles input, both player and exiting
// also returns a keyCode
int handleInput(int &xPos, int &yPos, bool &shouldRun);

// renders the difference btwn buffers and updates prevBuffer
void renderBuffer(char prevBuffer[1000][1000], char newBuffer[1000][1000]);
