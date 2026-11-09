
void gameLoop();

// renders the ui... on top of the game
void renderText(int keyCode);
// renders the game
void render(int xPos, int yPos);
// handles input, both player and exiting
// also returns a keyCode
int handleInput(int &xPos, int &yPos, bool &shouldRun);
