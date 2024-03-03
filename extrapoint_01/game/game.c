#ifndef _BOARD
#define _BOARD
#include "board.h"
#endif

#include "game.h"
#include "move.h"
#include "../GLCD/GLCD.h"
#include "../timer/timer.h"
#include "../RIT/RIT.h"

unsigned int currentMove = 0;
unsigned int moves[1000];
int numMoves = 0;

int loading = 0;
int gameStarted = 0;
int gameInitialized = 0;
int gameEnded = 0;
int player0walls = NUM_WALLS;
int player1walls = NUM_WALLS;
int timeLeft = TURN_TIME;
int playerTurn = 0;
struct player player0;
struct player player1;
int playerMadeChoice = 0;
struct player moveTo;
struct wall wallToPlace;
int mode =0; // move token: 0, place wall: 1
struct wall wallPlaced[MAX_WALL_TO_PLACE];
int numWallPlaced;

void initGame(){
	player1.posX = 3;
	player1.posY = PLAYER1_START_Y;
	player0.posX = 3;
	player0.posY = PLAYER0_START_Y;
	drawBoard();
	GUI_Text(MARGIN, 250, (uint8_t *) "PRESS 'INT0' TO START", TEXT_COLOR, BACKGROUND_COLOR);
	gameInitialized=1;
}

void startGame(){
	GUI_Text(MARGIN, 250, (uint8_t *) "PRESS 'INT0' TO START", BACKGROUND_COLOR, BACKGROUND_COLOR);
	drawPlayerInPosition(0);
	drawPlayerInPosition(1);
	drawPlayer0Walls();
	drawPlayer1Walls();
	drawTimeLeft();
	drawPlayerTurn();
	drawOptionsOnPlayer(playerTurn);
	gameStarted = 1;
	enable_timer(0);
	enable_RIT();	
}

void endGame(){
	gameEnded=1;
	disable_timer(0);
	disable_RIT();	
}

void changePlayer(){
	if(playerTurn) playerTurn=0;
	else playerTurn=1;
	timeLeft = TURN_TIME;
	playerMadeChoice=0;
	mode=0;
	wallToPlace.leftX=3; wallToPlace.rightX=3;
	wallToPlace.upY=3; wallToPlace.downY=4;
	drawTimeLeft();
	drawPlayerTurn();
}

void changeMode(){
	playerMadeChoice=0;
	if(mode){
		mode = 0;
		eraseWall(wallToPlace);
		drawPlacedWalls();
		drawOptionsOnPlayer(playerTurn);
	}else{
		mode=1;
		wallToPlace.leftX=3; wallToPlace.rightX=3;
		wallToPlace.upY=3; wallToPlace.downY=4;
		eraseOptionsOnPlayer(playerTurn);
		placeTempWall(wallToPlace);
	}
}

void movePlayerTo(int playerId, int x, int y){
	eraseOptionsOnPlayer(playerId);
	erasePlayerInPosition(playerId);
	if(playerId){
		player1.posX = x;
		player1.posY = y;
	}else{
		player0.posX = x;
		player0.posY = y;
	}
	drawPlayerInPosition(playerId);
	saveMove();
	if(checkVictory(playerId)){
		drawWinner();
		endGame();
	}else{
		changePlayer();
		drawOptionsOnPlayer(playerTurn);
	}
}

int isWallEqualTo(struct wall w1, struct wall w2){
	if(w1.downY==w2.downY && w1.leftX==w2.leftX && w1.rightX==w2.rightX && w1.upY==w2.upY){
				return 1;
		}
	return 0;
}

struct wall rotate(struct wall w){
	int wd;
	wd = checkWallDirection(w);
	if(wd==0){
		w.rightX++;
		w.downY--;
		w.downY--;
		w.upY--;
	}else if(wd==1){
		w.rightX--;
		w.downY++;
		w.downY++;
		w.upY++;
	}
	return w;
}

int checkWallSovrapposition(){
	int i, wdToPlace, wdPresent;
	struct wall tempW;
	wdToPlace=checkWallDirection(wallToPlace);
	for(i=0; i<numWallPlaced; i++){
		if(isWallEqualTo(wallPlaced[i], wallToPlace)) return 1;
		tempW=wallPlaced[i];
		wdPresent=checkWallDirection(tempW);
		if(wdToPlace==0 && wdPresent==0){ //both horizontal
			tempW.leftX++; tempW.rightX++;
			if(isWallEqualTo(tempW, wallToPlace)) return 1;
			tempW=wallToPlace;
			tempW.leftX++; tempW.rightX++;
			if(isWallEqualTo(tempW, wallPlaced[i])) return 1;
		}
		else if(wdToPlace==1 && wdPresent==1){ //both vertical
			if(tempW.upY>wallToPlace.upY){
				tempW.upY--; tempW.downY--;
				if(isWallEqualTo(tempW, wallToPlace)) return 1;
			}
			else{
				tempW.upY++; tempW.downY++;
				if(isWallEqualTo(tempW, wallToPlace)) return 1;
			}
		}
		else {
			tempW=rotate(tempW);
			if(isWallEqualTo(tempW, wallToPlace)) return 1;
		}
	}
	return 0;
}

void drawPlacedWalls(){
	int i;
	for(i=0; i<numWallPlaced; i++){
		placeWall(wallPlaced[i]);
	}
}

//direction-> 0:su, 1:destra, 2:giu, 3:sinistra
void moveWall(int direction){
	int wd;
	wd = checkWallDirection(wallToPlace);
	eraseWall(wallToPlace);
	
	switch(direction){
		case 0:
			if((wallToPlace.upY>0&&wd==1) || (wallToPlace.upY>1&&wd==0)){
				wallToPlace.upY--; wallToPlace.downY--;
			}
			break;
		case 1:
			if((wallToPlace.rightX<6&&wd==1)||(wallToPlace.rightX<5&&wd==0)){
				wallToPlace.leftX++; wallToPlace.rightX++;
			}
			break;
		case 2:
			if((wallToPlace.downY<7&&wd==0)||(wallToPlace.downY<5&&wd==1)){
				wallToPlace.upY++; wallToPlace.downY++;
			}
			break;
		case 3:
			if(wallToPlace.leftX>0){
				wallToPlace.leftX--; wallToPlace.rightX--;
			}
			break;
	}
	
	drawPlacedWalls();
	placeTempWall(wallToPlace);
}

void rotateWall(){
	eraseWall(wallToPlace);
	
	wallToPlace=rotate(wallToPlace);
	
	drawPlacedWalls();
	placeTempWall(wallToPlace);
}

int confirmWallFunction(){
	if(playerTurn){
		player1walls--;
		drawPlayer1Walls();
	} else{
		player0walls--;
		drawPlayer0Walls();
}
	wallPlaced[numWallPlaced] = wallToPlace;
	numWallPlaced++;
	return 1;
}

int confirmWall(void){
	if(confirmWallFunction()){
		saveMove();
		placeWall(wallToPlace);
		eraseOptionsOnPlayer(playerTurn);
		changePlayer();
		drawOptionsOnPlayer(playerTurn);
		return 1;
	}
	return 0;
}

