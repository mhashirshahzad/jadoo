#ifndef LEVELS_HPP
#define LEVELS_HPP

// size of the current level. it is always a square in my book
extern int currentLevelSize;
extern int currentLevelIndex;

extern char currentLevel[100][100];

extern char level1[100][100];

extern char level2[100][100];

extern char level3[100][100];

// changes currentLevel and updates CurrentLevelSize
void changeCurrentLevel(char newLevel[100][100]);


// updates currentLevel to be the next Level
void levelComplete();
void levelRestart();


// deletes a char, i.e sending g deletes all G and g blocks from level;
void deleteCharFromCurrentLevel(char ch, int x, int y);

#endif
