/*
 * Connect4.h
 *
 *  Created on: Apr 30, 2025
 *      Author: mmkna
 */

#ifndef INC_CONNECT4_H_
#define INC_CONNECT4_H_

#include "stm32f4xx_hal.h"
//#include "stm32f429i_discovery_lcd.h"
//#include "stm32f429i_discovery_ts.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "LCD_Driver.h"
#include "stmpe811.h"


#define ROWS 6
#define COLS 7
#define COLOR_PLAYER1 LCD_COLOR_RED
#define COLOR_PLAYER2 LCD_COLOR_YELLOW
#define BOARD_COLOR LCD_COLOR_BLUE
#define EMPTY 0
#define PLAYER1 1
#define PLAYER2 2
#define TOUCH_THRESHOLD 120
#define BUTTON_DROP_PIN GPIO_PIN_0
#define BUTTON_DROP_PORT GPIOA


void drawHomeScreen(void);
void drawGameBoard(void);
void drawCoinIndicator(void);
int getCurrentColumn();
void setCurrentColumn(int newColumn);
int getCurrentPlayer();
void incrementPlayerScore(int player);
void setSinglePlayerMode(bool mode);
void dropCoin(void);
void handleTouch(void);
bool checkWin(void);
bool isBoardFull(void);
void drawGameOverScreen(bool win);
int getAIMove(void);
void resetGame(void);


#endif /* INC_CONNECT4_H_ */
