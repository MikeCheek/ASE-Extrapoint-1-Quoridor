/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_timer.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    timer.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "lpc17xx.h"
#include "timer.h"
#include "../game/board.h"
#include "../game/game.h"
#include "../game/move.h"

/******************************************************************************
** Function name:		Timer0_IRQHandler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

extern int timeLeft;
extern int playerTurn;
extern int gameEnded;
extern int playerMadeChoice;
extern struct wall wallToPlace;

void TIMER0_IRQHandler (void)
{
	if(!gameEnded){
		if(timeLeft>0){
			timeLeft--;
			drawTimeLeft();
		} else{
			saveTurnSkippedMove();
			eraseOptionsOnPlayer(playerTurn);
			eraseWall(wallToPlace);
			drawPlacedWalls();
			changePlayer();
			drawOptionsOnPlayer(playerTurn);
		}
	}
  LPC_TIM0->IR = 1;			/* clear interrupt flag */
	enable_timer(0);
  return;
}


/******************************************************************************
** Function name:		Timer1_IRQHandler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER1_IRQHandler (void)
{
  LPC_TIM1->IR = 1;			/* clear interrupt flag */
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
