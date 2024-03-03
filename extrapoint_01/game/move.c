#ifndef _BOARD
#define _BOARD
#include "board.h"
#endif

extern unsigned int moves[100];
extern int numMoves;
extern int playerTurn;
extern struct wall wallToPlace;
extern int mode;
extern struct player player0;
extern struct player player1;

void saveMove(void){
	unsigned int currentMove = 0;
	int wd;
	
	if(playerTurn) currentMove |= 1 << 24;
	
	if(mode) { //wall placement
		currentMove |= 1 << 20;
		wd = checkWallDirection(wallToPlace);
		if(wd==0){ //horizontal
			currentMove |= 1 << 16;
			currentMove |= wallToPlace.downY << 8;
			currentMove |= wallToPlace.leftX+1;
		} else{ //vertical
			currentMove |= (wallToPlace.downY+1) << 8;
			currentMove |= wallToPlace.leftX;
		}
	} else { //player move
		if(playerTurn){
			currentMove |= player1.posY << 8;
			currentMove |= player1.posX;
		} else {
			currentMove |= player0.posY << 8;
			currentMove |= player0.posX;
		}
	}
	
	if(numMoves>=100) numMoves=0;
	moves[numMoves] = currentMove;
	numMoves++;
}

void saveTurnSkippedMove(void){
	unsigned int currentMove = 0;
	
	if(playerTurn) currentMove |= 1 << 24;
	
	if(playerTurn){
		currentMove |= player1.posY << 8;
		currentMove |= player1.posX;
	} else {
		currentMove |= player0.posY << 8;
		currentMove |= player0.posX;
	}
	
	currentMove |= 1 << 16;
	
	if(numMoves>=100) numMoves=0;
	moves[numMoves] = currentMove;
	numMoves++;
}
