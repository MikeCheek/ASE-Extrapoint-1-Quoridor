#include "../GLCD/GLCD.h"

#define MAX_SCREEN_X 240
#define MAX_SCREEN_Y 320

#define SCREEN_CENTER_X MAX_SCREEN_X * 0.5
#define SCREEN_CENTER_Y MAX_SCREEN_Y * 0.5

#define CELL_DIM 30
#define NUM_CELLS 7

#define PLAYER_DIM 20
#define PLAYER0_START_Y 6
#define PLAYER0_END_Y 0
#define PLAYER1_START_Y 0
#define PLAYER1_END_Y 6

#define NUM_WALLS 8

#define MARGIN 15

#define WINDOW_SIZE 60

#define WALL_SIZE 2
#define MAX_WALL_TO_PLACE 30

#define TURN_TIME 20

#define VISUALIZE_TRAP_CHECKER 0  //0:no, 1:yes

#define WALL_WARNING "No walls available, move the token"
#define WALL_PLACE_ERROR "You cannot place a wall here"
#define WALL_TRAP_ERROR "You cannot block the way"

#define Pink            0xF8DD

#define BACKGROUND_COLOR Black//RGB565CONVERT(231, 158, 75)
#define BOARD_LINE_COLOR Blue2//RGB565CONVERT(215, 129, 29) 
#define TEXT_COLOR Magenta 
#define PLAYER0_COLOR RGB565CONVERT(255, 119, 61) //Magenta
#define PLAYER1_COLOR RGB565CONVERT(19, 224, 0)
#define OPTION_COLOR Blue2
#define CHOOSE_COLOR Blue
#define WALL_COLOR Red
#define WALL_TEMP_COLOR Pink

struct player{
	int posX;
	int posY;
};

// se il muro è orizzontale: 	left==right	up!=down
// se il muro è verticale: 		left!=right	up==down
struct wall{
	int leftX;
	int rightX;
	int upY;
	int downY;
};

struct choosed{
	int up;
	int dwn;
	int dx;
	int sx;
};
