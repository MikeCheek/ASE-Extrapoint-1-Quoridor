/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_RIT.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    RIT.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "lpc17xx.h"
#include "RIT.h"
#include "../game/board.h"
#include "../game/game.h"
#include "../game/trapping.h"

/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

volatile int key1=0;
volatile int key2=0;
extern int playerTurn;
extern int gameStarted;
extern int gameEnded;
extern int playerMadeChoice;
extern struct player moveTo;
extern struct player player0;
extern struct player player1;
extern int mode;
extern struct wall wallToPlace;
extern int player0walls;
extern int player1walls;
extern int loading;

void RIT_IRQHandler (void)
{					
	static int J_select=0;
	static int J_down = 0;
	static int J_left = 0;
	static int J_up = 0;
	static int J_right = 0;
	int x, y;
	if(playerTurn){ x=player1.posX; y=player1.posY;}
	else { x=player0.posX; y=player0.posY;}
	/* Joytick J_Select pressed p1.25*/
	/* Joytick J_Down pressed p1.26 --> using J_DOWN due to emulator issues*/
	/* Joytick J_Left pressed p1.27*/
	/* Joytick J_Right pressed p1.28*/
	/* Joytick J_Up pressed p1.29*/
	
	if(gameEnded || loading) return;
	
	//option-> 0:su, 1:destra, 2:giu, 3:sinistra
	//direction-> 0:su, 1:destra, 2:giu, 3:sinistra

	if((LPC_GPIO1->FIOPIN & (1<<25)) == 0){			
		J_select++;
		switch(J_select){
			case 1:
				if(mode==0){
					if(gameStarted && playerMadeChoice){
						loading=1;
						movePlayerTo(playerTurn, moveTo.posX, moveTo.posY);
						loading=0;
					}
				}else{
					if(gameStarted){
						if(checkWallSovrapposition()) drawWallPlaceErrorMessage();
						else if(checkIfWallTrapsOpponent(wallToPlace)) drawWallTrapMessage();
						else{
							loading=1;
							confirmWall();
							loading=0;
						}
					}
				}
				break;
			default:
				break;
		}
	}
	else{
			J_select=0;
	}
	
	if((LPC_GPIO1->FIOPIN & (1<<26)) == 0){			
		J_down++;
		switch(J_down){
			case 1:
				if(mode==0){
					if(gameStarted){
						loading=1;
						if(chooseOption(x,y, 2, 1))
											playerMadeChoice=1;
						loading=0;
					}
				}else{
					loading=1;
					moveWall(2);
					loading=0;
				}
				break;
			default:
				break;
		}
	}
	else{
			J_down=0;
	}
	
	if((LPC_GPIO1->FIOPIN & (1<<27)) == 0){			
		J_left++;
		switch(J_left){
			case 1:
				if(mode==0){
					if(gameStarted){
						loading=1;
						if(chooseOption(x,y, 3, 1))
								playerMadeChoice=1;
						loading=0;
					}
				}else{
					loading=1;
					moveWall(3);
					loading=0;
				}
				break;
			default:
				break;
		}
	}
	else{
			J_left=0;
	}
	
	if((LPC_GPIO1->FIOPIN & (1<<28)) == 0){			
		J_right++;
		switch(J_right){
			case 1:
				if(mode==0){
					if(gameStarted){
						loading=1;
						if(chooseOption(x,y, 1, 1))
											playerMadeChoice=1;
						loading=0;
					}
				}else{
					loading=1;
					moveWall(1);
					loading=0;
				}
				break;
			default:
				break;
		}
	}
	else{
			J_right=0;
	}
	
	if((LPC_GPIO1->FIOPIN & (1<<29)) == 0){			
		J_up++;
		switch(J_up){
			case 1:
				if(mode==0){
					if(gameStarted){
						loading=1;
						if(chooseOption(x,y, 0, 1))
											playerMadeChoice=1;
						loading=0;
					}
				}else{
					loading=1;
					moveWall(0);
					loading=0;
				}
				break;
			default:
				break;
		}
	}
	else{
			J_up=0;
	}
	
	/* button management */
	if(key1!=0){ 
		if((LPC_GPIO2->FIOPIN & (1<<11)) == 0){	/* KEY1 pressed */
			key1++;			
			switch(key1){
				case 2:				/* pay attention here: please see slides to understand value 2 */
					if(playerTurn){
						if(player1walls>0) changeMode();
						else drawWallLeftMessage();
					} else {
						if(player0walls>0) changeMode();
						else drawWallLeftMessage();
					}
					break;
				default:
					break;
			}
		}
		else {	/* button released */
			key1=0;			
			NVIC_EnableIRQ(EINT1_IRQn);							 /* enable Button interrupts			*/
			LPC_PINCON->PINSEL4    |= (1 << 22);     /* External interrupt 0 pin selection */
		}
	}
	
	if(key2!=0){ 
		if((LPC_GPIO2->FIOPIN & (1<<12)) == 0){	/* KEY2 pressed */
			key2++;				
			switch(key2){
				case 2:				/* pay attention here: please see slides to understand value 2 */
					if(mode==1) rotateWall();
					break;
				default:
					break;
			}
		}
		else {	/* button released */
			key2=0;			
			NVIC_EnableIRQ(EINT2_IRQn);							 /* enable Button interrupts			*/
			LPC_PINCON->PINSEL4    |= (1 << 24);     /* External interrupt 0 pin selection */
		}
	}
	
  LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
