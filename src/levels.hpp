#ifndef LEVELS_HPP
#define LEVELS_HPP

// size of the current level. it is always a square in my book
extern int currentLevelSize;
extern int currentLevelIndex;

extern char currentLevel[100][100];

extern char level1[100][100];

extern char level2[100][100];

void changeCurrentLevel(char newLevel[100][100]);

#endif
