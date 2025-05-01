/*
 * ApplicationCode.c
 *
 *  Created on: Dec 30, 2023 (updated 11/12/2024) Thanks Donavon! 
 *      Author: Xavion
 */

#include "ApplicationCode.h"

/* Static variables */


extern void initialise_monitor_handles(void); 

#if COMPILE_TOUCH_FUNCTIONS == 1
static STMPE811_TouchData StaticTouchData;
#endif // COMPILE_TOUCH_FUNCTIONS

void ApplicationInit(void)
{
	initialise_monitor_handles(); // Allows printf functionality
    LTCD__Init();
    LTCD_Layer_Init(0);
    LCD_Clear(0,LCD_COLOR_WHITE);

    #if COMPILE_TOUCH_FUNCTIONS == 1
	InitializeLCDTouch();

	// This is the orientation for the board to be direclty up where the buttons are vertically above the screen
	// Top left would be low x value, high y value. Bottom right would be low x value, low y value.
	StaticTouchData.orientation = STMPE811_Orientation_Portrait_2;

	#endif // COMPILE_TOUCH_FUNCTIONS
}

void LCD_Visual_Demo(void)
{
	visualDemo();
}

void playGame(){


	bool homeLoop = true;
	bool gameLoop = true;
	bool endLoop = true;

	while(1){
		drawHomeScreen();
		endLoop = true;
		while (homeLoop) { //loop screen 1, checking for a press on either one of the 2 buttons
			/* If touch pressed */
			if (returnTouchStateAndLocation(&StaticTouchData) == STMPE811_State_Pressed) {
				/* Touch valid */
				printf("\nX: %03d\nY: %03d\n", StaticTouchData.x, StaticTouchData.y);
				if(StaticTouchData.x > 0 && StaticTouchData.x < 110){
					if(StaticTouchData.y > 0 && StaticTouchData.y < 110){ //pressed 1 Player button
						LCD_Clear(0, LCD_COLOR_GREEN);
						setSinglePlayerMode(true);
						homeLoop = false;
					}
				}

				if(StaticTouchData.x > 110 && StaticTouchData.x < 220){
					if(StaticTouchData.y > 0 && StaticTouchData.y < 110){ //pressed 2 Player button
						LCD_Clear(0, LCD_COLOR_RED);
						setSinglePlayerMode(false);
						homeLoop = false;
					}
				}

			} else {
				/* Touch not pressed */
				printf("Not Pressed\n\n");
			}
		}

		drawGameBoard();
//		__HAL_TIM_SET_COUNTER(&htim2, 0);
		setStartTime(HAL_GetTick());
//		HAL_TIM_Base_Start(&htim2);

		while (gameLoop) { //loop screen 2, checking for presses on the left and right to move the game piece left and
						   // right respectively, checking for a 4 in a row or full board to trigger screen 3
				/* If touch pressed */
			if (returnTouchStateAndLocation(&StaticTouchData) == STMPE811_State_Pressed) {
				/* Touch valid */
				printf("\nX: %03d\nY: %03d\n", StaticTouchData.x, StaticTouchData.y);
				if(StaticTouchData.x > 0 && StaticTouchData.x < 110){ //left side of screen pressed
					setCurrentColumn(getCurrentColumn() - 1); //move piece left
					drawGameBoard();

				} else if(StaticTouchData.x > 110 && StaticTouchData.x < 220){ //right side of screen pressed
					setCurrentColumn(getCurrentColumn() + 1); //move piece right
					drawGameBoard();

				}

			}
			else if (checkWin()) { //4 in a row detected
				if (getCurrentPlayer() == PLAYER1) incrementPlayerScore(1);
				else incrementPlayerScore(2);
				gameLoop = false;
				drawGameOverScreen(true); //-> screen 3 with a winner
			}
			else if (isBoardFull()) { //no more moves detected
				gameLoop = false;
				drawGameOverScreen(false); //-> screen 3 with a tie
			}

			else {
				/* Touch not pressed */
				printf("Not Pressed\n\n");

			}
		}


		while(endLoop){ //loop screen 3, displaying the winner/tie, scoreboard, time elapsed for the game, and the restart button
			if (returnTouchStateAndLocation(&StaticTouchData) == STMPE811_State_Pressed) {
				/* Touch valid */
				printf("\nX: %03d\nY: %03d\n", StaticTouchData.x, StaticTouchData.y);
				if(StaticTouchData.x > 110 && StaticTouchData.x < 220){
					if(StaticTouchData.y > 0 && StaticTouchData.y < 110){ //restart press detected
						endLoop = false;

						homeLoop = true; //reset board and states for next run
						gameLoop = true;
						clearBoard();
						setCurrentPlayer(PLAYER1);
						setCurrentColumn(3);

					}
				}
			}
		}

	}


}

#if COMPILE_TOUCH_FUNCTIONS == 1
void LCD_Touch_Polling_Demo(void)
{
	LCD_Clear(0,LCD_COLOR_GREEN);
	while (1) {
		/* If touch pressed */
		if (returnTouchStateAndLocation(&StaticTouchData) == STMPE811_State_Pressed) {
			/* Touch valid */
			printf("\nX: %03d\nY: %03d\n", StaticTouchData.x, StaticTouchData.y);
			LCD_Clear(0, LCD_COLOR_RED);
		} else {
			/* Touch not pressed */
			printf("Not Pressed\n\n");
			LCD_Clear(0, LCD_COLOR_GREEN);
		}
	}
}
#endif // COMPILE_TOUCH_FUNCTIONS



void EXTI0_IRQHandler(void){
	HAL_NVIC_DisableIRQ(EXTI0_IRQ_NUMBER);

	dropCoin();
	printf("BUTTON\nBUTTON\n");

	__NVIC_ClearPendingIRQ(EXTI0_IRQ_NUMBER);
	__HAL_GPIO_EXTI_CLEAR_IT(1);

	HAL_NVIC_EnableIRQ(EXTI0_IRQ_NUMBER);
}

