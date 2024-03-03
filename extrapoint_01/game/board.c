#include <stdio.h>
#ifndef _BOARD
#define _BOARD
#include "board.h"
#endif
#include "../GLCD/GLCD.h"

extern int player0walls;
extern int player1walls;
extern int timeLeft;
extern int playerTurn;
extern struct player player0;
extern struct player player1;
extern int playerMadeChoice;
extern struct player moveTo;
extern struct wall wallToPlace;
extern struct wall wallPlaced[MAX_WALL_TO_PLACE];
extern int numWallPlaced;

void drawBoard(){
	int i;
	int mar = MARGIN;
	int dim = CELL_DIM;
	int length = MAX_SCREEN_X - mar;
	
	for(i=0; i<NUM_CELLS+1; i++){
		drawThickLine(0, mar, dim*i +mar, length, dim*i +mar, BOARD_LINE_COLOR);
		drawThickLine(1, dim*i +mar, mar, dim*i +mar, length, BOARD_LINE_COLOR);
	}
}

void drawThickLine( int vertical, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1 , uint16_t color ){
	if(vertical){
		LCD_DrawLine(x0-1, y0, x1-1, y1, color);
		LCD_DrawLine(x0, y0, x1, y1, color);
		LCD_DrawLine(x0+1, y0, x1+1, y1, color);
	}
	else{
		LCD_DrawLine(x0, y0-1, x1, y1-1, color);
		LCD_DrawLine(x0, y0, x1, y1, color);
		LCD_DrawLine(x0, y0+1, x1, y1+1, color);		
	}
}



void drawPlayerFunction(uint16_t x, uint16_t y, uint16_t color){
	int i;
	char id[2];
	for(i=0; i<PLAYER_DIM; i++){
		LCD_DrawLine(x,y +i,x+PLAYER_DIM,y +i,color);
	}
	if(color == PLAYER0_COLOR){
		sprintf(id, "%d", 1);
		GUI_Text(x+(PLAYER_DIM/2)-2,y+2,(uint8_t *) id, BACKGROUND_COLOR, color);
	} else if(color == PLAYER1_COLOR){
		sprintf(id, "%d", 2);
		GUI_Text(x+(PLAYER_DIM/2)-2,y+2,(uint8_t *) id, BACKGROUND_COLOR, color);
	}
}

void erasePlayer(int playerId, uint16_t x, uint16_t y){
	drawPlayerFunction(x, y, BACKGROUND_COLOR);
}

void drawPlayer(int playerId, uint16_t x, uint16_t y){
	switch(playerId){
		case 0: // Player 1
			drawPlayerFunction(x, y, PLAYER0_COLOR);
			break;
		case 1: // Player 2
			drawPlayerFunction(x, y, PLAYER1_COLOR);
			break;
		case 2: // Option of moving
			drawPlayerFunction(x, y, OPTION_COLOR);
			break;
		case 3: // Option of moving selected
			drawPlayerFunction(x, y, CHOOSE_COLOR);
			break;
		default: // Erase 
			drawPlayerFunction(x, y, BACKGROUND_COLOR);
			break;
	}
}

void erasePlayerInPosition(int playerId){
	struct player p;
	uint16_t posX;
	uint16_t posY;
	
	if(playerId){ 
		p = player1;
	}
	else { 
		p = player0;
	}
	
	posX = MARGIN + ((CELL_DIM - PLAYER_DIM)/2) + (p.posX*CELL_DIM);
	posY = MARGIN + ((CELL_DIM - PLAYER_DIM)/2) + (p.posY*CELL_DIM);
	
	erasePlayer(playerId, posX, posY);
}

void drawPlayerInPosition(int playerId){
	struct player p;
	uint16_t posX;
	uint16_t posY;
	
	if(playerId){ 
		p = player1;
	}
	else { 
		p = player0;
	}
	
	posX = MARGIN + ((CELL_DIM - PLAYER_DIM)/2) + (p.posX*CELL_DIM);
	posY = MARGIN + ((CELL_DIM - PLAYER_DIM)/2) + (p.posY*CELL_DIM);
	
	drawPlayer(playerId, posX, posY);
}

void drawInPosition(int playerId, int x, int y){	
	uint16_t pX = MARGIN + ((CELL_DIM - PLAYER_DIM)/2) + (x*CELL_DIM);
	uint16_t pY = MARGIN + ((CELL_DIM - PLAYER_DIM)/2) + (y*CELL_DIM);
	
	drawPlayer(playerId, pX, pY);
}

int checkOccupedAndDrawOver(int id, int x, int y, int nextX, int nextY, int draw){
	struct player opposite;
	if(playerTurn) opposite=player0;
	else opposite=player1;
	
	if(opposite.posX!=x || opposite.posY!=y) return 0;
	
	if(nextX>6 || nextX<0 || nextY>6 || nextY<0) return 0;

	if(checkMoveOption(opposite.posX, opposite.posY, nextX, nextY)){
		if(draw) drawInPosition(id, nextX, nextY);
		return 1;
	}
	return -1; // do nothing
}

void drawOptionsFunction(int id, int x, int y){
	int check;
	check = checkOccupedAndDrawOver(id, x-1, y, x-2, y, 1);
	if(x >0 && checkMoveOption(x,y, x-1, y) && check==0){
		drawInPosition(id, x-1, y);
	}
	check = checkOccupedAndDrawOver(id, x+1, y, x+2, y, 1);
	if(x<6 && checkMoveOption(x,y, x+1, y) && check==0){
		drawInPosition(id, x+1, y);	
	}
	check = checkOccupedAndDrawOver(id, x, y-1, x, y-2, 1);
	if(y>0 && checkMoveOption(x,y, x, y-1) && check==0){
		drawInPosition(id, x, y-1);
	}
	check = checkOccupedAndDrawOver(id, x, y+1, x, y+2, 1);
	if(y<6 && checkMoveOption(x,y, x, y+1) && check==0){
		drawInPosition(id, x, y+1);
	}
}

void eraseOptionsOnPlayer(int playerId){
	int x;
	int y;
	if(playerId){ x=player1.posX; y=player1.posY;}
	else { x=player0.posX; y=player0.posY;}
	
	drawOptionsFunction(-1, x, y);
}

int checkMoveOption(int x0, int y0, int x1, int y1){
	int i, wd;
	struct wall w;
	
	for(i=0; i<numWallPlaced; i++){
		w=wallPlaced[i];
		wd=checkWallDirection(w);
		if(wd==0){
			if(x0==w.leftX && y0==w.upY && x1==w.leftX && y1==w.upY-1) return 0;
			if(x0==w.leftX && y0==w.upY-1 && x1==w.leftX && y1==w.upY) return 0;
			if(x0==w.leftX+1 && y0==w.upY && x1==w.leftX+1 && y1==w.upY-1) return 0;
			if(x0==w.leftX+1 && y0==w.upY-1 && x1==w.leftX+1 && y1==w.upY) return 0;
		}else if(wd==1){
			if(y0==w.upY && x0==w.leftX && y1==w.upY && x1==w.rightX) return 0;
			if(y0==w.upY && x0==w.rightX && y1==w.upY && x1==w.leftX) return 0;
			if(y0==w.upY+1 && x0==w.leftX && y1==w.upY+1 && x1==w.rightX) return 0;
			if(y0==w.upY+1 && x0==w.rightX && y1==w.upY+1 && x1==w.leftX) return 0;
		}
	}
	return 1;
}

void drawOptionsOnPlayer(int playerId){
	int x;
	int y;
	if(playerId){ x=player1.posX; y=player1.posY;}
	else { x=player0.posX; y=player0.posY;}
	
	drawOptionsFunction(2, x,y);
}

//option-> 0:su, 1:destra, 2:giu, 3:sinistra
int chooseOption(int x, int y, int option, int draw){
	int check;
	
	if((option==0 && y<=0) || (option==1 && x>=6) || (option==2 && y>=6) || (option==3 && x<=0) || option>3)
		return 0;
	
	if(draw) drawOptionsFunction(2, x,y);
		
	switch(option){
		case 0:
			if(y>0 && checkMoveOption(x,y, x, y-1)){
				check = checkOccupedAndDrawOver(3, x, y-1, x, y-2, draw);
				if(check==1){
					moveTo.posX = x;
					moveTo.posY = y-2;
				}else if(check==0){
					moveTo.posX = x;
					moveTo.posY = y-1;
					if(draw) drawInPosition(3, x, y-1);
				} else return 0;
			}else return 0;
			break;
		case 1:
			if(x<6 && checkMoveOption(x,y, x+1, y)){
				check = checkOccupedAndDrawOver(3, x+1, y, x+2, y, draw);
				if(check==1){
					moveTo.posX = x+2;
					moveTo.posY = y;
				} else if(check==0){
					moveTo.posX = x+1;
					moveTo.posY = y;
					if(draw) drawInPosition(3, x+1, y);
				} else return 0;
			}else return 0;
			break;
		case 2:
			if(y<6 && checkMoveOption(x,y, x, y+1)){
				check = checkOccupedAndDrawOver(3, x, y+1, x, y+2, draw);
				if(check==1){
					moveTo.posX = x;
					moveTo.posY = y+2;
				} else if(check==0){
					moveTo.posX = x;
					moveTo.posY = y+1;
					if(draw) drawInPosition(3, x, y+1);
				}else return 0;
			}else return 0;
			break;
		case 3:
			if(x >0 && checkMoveOption(x,y, x-1, y)){
				check = checkOccupedAndDrawOver(3, x-1, y, x-2, y, draw);
				if(check==1){
					moveTo.posX = x-2;
					moveTo.posY = y;
				}else if(check==0){
					moveTo.posX = x-1;
					moveTo.posY = y;
					if(draw) drawInPosition(3, x-1, y);
				} else return 0;
			}else return 0;
			break;
	}
	return 1;
}

void drawBox(int x, int y, int dim){
	int height = dim-10;
	LCD_DrawLine(x, y, x+dim, y, BOARD_LINE_COLOR);
	LCD_DrawLine(x, y+height, x+dim, y+height, BOARD_LINE_COLOR);
	LCD_DrawLine(x, y, x, y+height, BOARD_LINE_COLOR);
	LCD_DrawLine(x+dim, y, x+dim, y+height, BOARD_LINE_COLOR);
}

void drawPlayer0Walls(){
	char walls1[2]="";
	sprintf(walls1, "%d", player0walls);
	
	drawBox(MARGIN, 240, WINDOW_SIZE);
	GUI_Text(MARGIN-1, 240+5, (uint8_t *)"P1 WALLS", PLAYER0_COLOR, BACKGROUND_COLOR);
	GUI_Text(MARGIN+10, 240+30, (uint8_t *) walls1, PLAYER0_COLOR, BACKGROUND_COLOR);
}

void drawPlayer1Walls(){
	int posX = MAX_SCREEN_X-MARGIN-WINDOW_SIZE;
	char walls2[2]="";
	sprintf(walls2, "%d", player1walls);
	
	drawBox(posX, 240, WINDOW_SIZE);
	GUI_Text(posX-1, 240+5, (uint8_t *)"P2 WALLS", PLAYER1_COLOR, BACKGROUND_COLOR);
	GUI_Text(posX+10, 240+30, (uint8_t *) walls2, PLAYER1_COLOR, BACKGROUND_COLOR);
}

void drawTimeLeft(){
	int posX = SCREEN_CENTER_X -(WINDOW_SIZE/2);
	char time[2]="";
	sprintf(time, "%2d", timeLeft);
	
	drawBox(posX, 240, WINDOW_SIZE);
	GUI_Text(posX-4, 240+5, (uint8_t *)"TIME LEFT", Red, BACKGROUND_COLOR);
	GUI_Text(posX+10, 240+30, (uint8_t *) time, Red, BACKGROUND_COLOR);
}

void eraseWallPlaceErrorMessage(void){
	GUI_Text(MARGIN/2, 300, (uint8_t *) WALL_PLACE_ERROR, BACKGROUND_COLOR, BACKGROUND_COLOR);
}

void drawWallPlaceErrorMessage(void){
	eraseWallPlaceErrorMessage();
	GUI_Text(MARGIN/2, 300, (uint8_t *) WALL_PLACE_ERROR, Red, BACKGROUND_COLOR);
}

void drawWallLeftMessage(){
	GUI_Text(MARGIN/2, 300, (uint8_t *) WALL_WARNING, Red, BACKGROUND_COLOR);
}

void eraseWallLeftMessage(){
	GUI_Text(MARGIN/2, 300, (uint8_t *) WALL_WARNING, BACKGROUND_COLOR, BACKGROUND_COLOR);
}

void eraseWallTrapMessage(void){
	GUI_Text(MARGIN/2, 300, (uint8_t *) WALL_TRAP_ERROR, BACKGROUND_COLOR, BACKGROUND_COLOR);
}

void drawWallTrapMessage(void){
	eraseWallTrapMessage();
	GUI_Text(MARGIN/2, 300, (uint8_t *) WALL_TRAP_ERROR, Red, BACKGROUND_COLOR);
}

void drawPlayerTurn(){
	char turn[20]="";
	int t = playerTurn+1;
	sprintf(turn, "Player %d turn", t);
	
	eraseWallLeftMessage();
	GUI_Text(MARGIN+50, 300, (uint8_t *) turn, Red, Yellow);
}

int checkWallDirection(struct wall w){
	if(w.leftX!=w.rightX && w.upY==w.downY){ //vertical
		return 1;
	}
	else if(w.upY!=w.downY && w.leftX==w.rightX){ //horizontal
		return 0;
	}
	return -1;
}

void drawWall(struct wall w, int mode){ //mode-> 0:place, 1:placetemp, -1:erase
	int posX, posY;
	uint16_t color;
	
	posX = MARGIN + (w.rightX * CELL_DIM);
	posY = MARGIN + (w.upY * CELL_DIM);
	
	if(mode==-1) color=BOARD_LINE_COLOR;
	else if(mode==0) color = WALL_COLOR;
	else if(mode==1) color = WALL_TEMP_COLOR;
	else return;
	
	if(checkWallDirection(w)==1){ //vertical
		posY+=2;
		drawThickLine(1, posX, posY, posX, posY+(WALL_SIZE*CELL_DIM)-4, color);
	}
	else if(checkWallDirection(w)==0){ //horizontal
		posX+=2;
		drawThickLine(0, posX, posY, posX+(WALL_SIZE*CELL_DIM)-4, posY, color);
	}
}

void eraseWall(struct wall w){
	drawWall(w, -1);
}

void placeWall(struct wall w){
	drawWall(w,0);
}

void placeTempWall(struct wall w){
	drawWall(w,1);
}

int checkVictory(int playerId){
	int goalY, y;
	if(playerId){
		goalY=PLAYER1_END_Y;
		y = player1.posY;
	} else{ 
		goalY=PLAYER0_END_Y;
		y = player0.posY;
	}
	if(y==goalY) return 1;
	return 0;
}

void drawWinner(void){
	char message[20];
	int winner = playerTurn+1;
	
	sprintf(message, "PLAYER %d WON", winner);
	GUI_Text(70, 100, (uint8_t *) message, White, Red);
	GUI_Text(80, 150, (uint8_t *) "GAME ENDED", White, Red);
}
