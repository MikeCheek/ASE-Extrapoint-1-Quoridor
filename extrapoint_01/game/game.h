
void initGame(void);
void startGame(void);
void movePlayerTo(int playerId, int x, int y);
void changePlayer(void);
void changeMode(void);
//direction-> 0:su, 1:destra, 2:giu, 3:sinistra
void moveWall(int direction);
void rotateWall(void);
int confirmWall(void);
int checkWallSovrapposition(void);
void drawPlacedWalls(void);
