#ifndef _BOARD
#define _BOARD
#include "board.h"
#endif

#include "trapping.h"
#include "helper.h"

extern struct choosed matrix[7][7];
extern int check;

void tryUp(int x, int y){
	if(check==0) return;
	if(matrix[y][x].up==0 && chooseOption(x,y,0, VISUALIZE_TRAP_CHECKER)) {
		matrix[y][x].up++;
		recursiveSearch(x,y-1);
	}
}

void tryDx(int x, int y){
	if(check==0) return;
	if(matrix[y][x].dx==0 && chooseOption(x,y,1, VISUALIZE_TRAP_CHECKER)) {
		matrix[y][x].dx++;
		recursiveSearch(x+1,y);
	}
}

void tryDwn(int x, int y){
	if(check==0) return;
	if(matrix[y][x].dwn==0 && chooseOption(x,y,2, VISUALIZE_TRAP_CHECKER)) {
		matrix[y][x].dwn++;
		recursiveSearch(x,y+1);
	}
}

void trySx(int x, int y){
	if(check==0) return;
	if(matrix[y][x].sx==0 && chooseOption(x,y,3, VISUALIZE_TRAP_CHECKER)) {
		matrix[y][x].sx++;
		recursiveSearch(x-1,y);
	}
}
