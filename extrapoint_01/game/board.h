#ifndef _GLOBALS
#define _GLOBALS
#include "globals.h"
#endif

typedef unsigned short     int uint16_t;

void drawThickLine( int vertical, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1 , uint16_t color );
void drawBoard(void);
void drawPlayer(int playerId, uint16_t x, uint16_t y);
void drawPlayerInPosition(int playerId);
void erasePlayerInPosition(int playerId);
void drawOptionsOnPlayer(int playerId);
void eraseOptionsOnPlayer(int playerId);
//option-> 0:su, 1:destra, 2:giu, 3:sinistra
//return-> 0:move invalid, 1:move valid
int chooseOption(int x, int y, int option, int draw);
void drawPlayer0Walls(void);
void drawPlayer1Walls(void);
void drawTimeLeft(void);
void drawPlayerTurn(void);
void drawWallLeftMessage(void);
void eraseWallLeftMessage(void);
void drawWallPlaceErrorMessage(void);
void eraseWallPlaceErrorMessage(void);
void drawWallTrapMessage(void);
void eraseWallTrapMessage(void);
void placeWall(struct wall w);
void placeTempWall(struct wall w);
void eraseWall(struct wall w);
int checkWallDirection(struct wall w);
int checkMoveOption(int x0, int y0, int x1, int y1);
int checkVictory(int playerId);
void drawWinner(void);
