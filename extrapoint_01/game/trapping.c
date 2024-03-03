#include "trapping.h"
#include "helper.h"

extern int playerTurn;
extern struct player player0;
extern struct player player1;
extern struct wall wallPlaced[MAX_WALL_TO_PLACE];
extern int numWallPlaced;

int check;

struct choosed matrix[7][7];

//option-> 0:su, 1:destra, 2:giu, 3:sinistra

void recursiveSearch(int x, int y){
	int goal;
	
	if(check==0) return;
	
	if(playerTurn) goal=PLAYER0_END_Y;
	else goal=PLAYER1_END_Y;
	
	if(y==goal){
		check=0;
		return;
	}
	
	if(goal>y) {
		tryDwn(x,y);
		if(x>3){
			tryDx(x,y);
			trySx(x,y);
		}else{
			trySx(x,y);
			tryDx(x,y);
		}
		tryUp(x,y);
	} else {
		tryUp(x,y);
		if(x>3){
			tryDx(x,y);
			trySx(x,y);
		}else{
			trySx(x,y);
			tryDx(x,y);
		}
		tryDwn(x,y);
	}
}

int checkIfWallTrapsOpponent(struct wall w){
	int x, y, i, j;
	struct choosed zeroChoosed;
	
	wallPlaced[numWallPlaced] = w;
	numWallPlaced++;
	
	if(playerTurn){ x=player0.posX; y=player0.posY;}
	else { x=player1.posX; y=player1.posY;}
	
	zeroChoosed.dwn=0; zeroChoosed.dx=0; zeroChoosed.sx=0; zeroChoosed.up=0;
	
	for(i=0; i<7; i++){
		for(j=0; j<7; j++){
			matrix[i][j] = zeroChoosed;
		}
	}
	
	check=1;
	
 	recursiveSearch(x,y);
	
	numWallPlaced--;
	
	return check;
}
