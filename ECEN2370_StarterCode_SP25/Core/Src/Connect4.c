/*
 * Connect4.c
 *
 *  Created on: Apr 30, 2025
 *      Author: mmkna
 */

#include "Connect4.h"
#include <stdio.h>

//extern RNG_HandleTypeDef hrng;



uint8_t board[ROWS][COLS];
int currentColumn = 3;
int currentPlayer = PLAYER1;
bool gameActive = false;
bool singlePlayerMode = false;
int scoreP1 = 0, scoreP2 = 0;
uint32_t roundStartTime;


void clearBoard(){
	for (int r = 0; r < ROWS; r++) {
		for (int c = 0; c < COLS; c++) {
			board[r][c] = EMPTY;
		}
	}
}


//int main(void) {
//    HAL_Init();
//    BSP_LCD_Init();
//    BSP_LCD_LayerDefaultInit(0, LCD_FRAME_BUFFER);
//    BSP_LCD_SelectLayer(0);
//    BSP_LCD_Clear(LCD_COLOR_WHITE);
//    BSP_LCD_DisplayOn();
//    BSP_TS_Init(BSP_LCD_GetXSize(), BSP_LCD_GetYSize());
//
//    drawHomeScreen();
//
//    while (1) {
//        handleTouch();
//        if (gameActive) {
//            if (HAL_GPIO_ReadPin(BUTTON_DROP_PORT, BUTTON_DROP_PIN) == GPIO_PIN_SET) {
//                dropCoin();
//                HAL_Delay(200);
//            }
//        }
//    }
//}

void drawHomeScreen(void) {

    LCD_Clear(0,LCD_COLOR_WHITE);
    LCD_SetTextColor(LCD_COLOR_BLACK);
	LCD_SetFont(&Font16x24);

    LCD_DisplayChar(40,80,'C');
	LCD_DisplayChar(55,80,'o');
	LCD_DisplayChar(70,80,'n');
	LCD_DisplayChar(85,80,'n');
	LCD_DisplayChar(100,80,'e');
	LCD_DisplayChar(115,80,'c');
	LCD_DisplayChar(125,80,'t');
	LCD_DisplayChar(155,80,'4');


	LCD_Draw_Square_Fill(10, 205, 95, LCD_COLOR_GREEN);
	LCD_DisplayChar(15,240,'1');
	LCD_DisplayChar(40,240,'P');
	LCD_DisplayChar(50,240,'l');
	LCD_DisplayChar(60,240,'a');
	LCD_DisplayChar(70,240,'y');
	LCD_DisplayChar(80,240,'e');
	LCD_DisplayChar(90,240,'r');


	LCD_Draw_Square_Fill(120, 205, 95, LCD_COLOR_RED);
	LCD_DisplayChar(125,240,'2');
	LCD_DisplayChar(150,240,'P');
	LCD_DisplayChar(160,240,'l');
	LCD_DisplayChar(170,240,'a');
	LCD_DisplayChar(180,240,'y');
	LCD_DisplayChar(190,240,'e');
	LCD_DisplayChar(200,240,'r');

}

void drawGameBoard(void) {
    LCD_Clear(0, LCD_COLOR_WHITE);
//    HAL_Delay(10);
    LCD_Draw_Rectangle_Fill(0, 120, 240, 200, BOARD_COLOR);
//    HAL_Delay(2);

    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            uint16_t color = LCD_COLOR_WHITE;
            if (board[r][c] == PLAYER1) color = COLOR_PLAYER1;
            else if (board[r][c] == PLAYER2) color = COLOR_PLAYER2;

            LCD_Draw_Circle_Fill(30 + c * 30, 140 + r * 30, 12, color);
//            HAL_Delay(1);
        }
    }

    drawCoinIndicator();
//    HAL_Delay(2);
}

void drawCoinIndicator(void) {
	uint16_t color = COLOR_PLAYER1;
    if (currentPlayer == PLAYER2){
    	color = COLOR_PLAYER2;
    }
    LCD_Draw_Circle_Fill(30 + currentColumn * 30, 100, 12, color);
}

int getCurrentColumn(){
	return currentColumn;
}
void setCurrentColumn(int newColumn){
	if(newColumn <= 6){
		if(newColumn >= 0){
			currentColumn = newColumn;
		}
		else{
			currentColumn = 6;
		}
	}
	else{
		currentColumn = 0;
	}

}
int getCurrentPlayer(){
	return currentPlayer;
}
void setCurrentPlayer(int player){
	currentPlayer = player;
}
void incrementPlayerScore(int player){
	if(player == 1){
		scoreP1++;
	}
	else{
		scoreP2++;
	}
}
void setSinglePlayerMode(bool mode){
	if(mode == true){
		singlePlayerMode = true;
	}
	else{
		singlePlayerMode = false;
	}
}

void dropCoin(void) {
    for (int r = ROWS - 1; r >= 0; r--) {
        if (board[r][currentColumn] == EMPTY) {
            board[r][currentColumn] = currentPlayer;

			currentPlayer = (currentPlayer == PLAYER1) ? PLAYER2 : PLAYER1;
			drawGameBoard();

			if (singlePlayerMode && currentPlayer == PLAYER2) {
//				int col;
				uint32_t randomNum;
				HAL_RNG_GenerateRandomNumber(&hrng, &randomNum);
//				while (HAL_RNG_GetState(&hrng) != HAL_RNG_STATE_READY);
//				HAL_Delay(1);
				HAL_RNG_GenerateRandomNumber(&hrng, &randomNum);
//				while (HAL_RNG_GetState(&hrng) != HAL_RNG_STATE_READY);

				//	}

				currentColumn = randomNum % COLS; //getAIMove();
				drawGameBoard();



				while(!aiDropCheck()){
					currentColumn++;
					if (currentColumn > 6){
						currentColumn = 0;
					}
				}
//					HAL_RNG_GenerateRandomNumber(&hrng, &randomNum);
////					while (HAL_RNG_GetState(&hrng) != HAL_RNG_STATE_READY);
//					HAL_RNG_GenerateRandomNumber(&hrng, &randomNum);
//					while (HAL_RNG_GetState(&hrng) != HAL_RNG_STATE_READY);
//					currentColumn = randomNum % COLS; //getAIMove();
//
//				}

//				HAL_Delay(700);
//				printf("entered ai box \n");
//				dropCoin();

			}
//			printf("%d\n", r);

			break;
		}
    }
}


bool aiDropCheck(){
	for (int r = ROWS - 1; r >= 0; r--) {
		if (board[r][currentColumn] == EMPTY) {
			board[r][currentColumn] = currentPlayer;

			currentPlayer = (currentPlayer == PLAYER1) ? PLAYER2 : PLAYER1;
			drawGameBoard();
			return true;
		}
	}
	return false; //column full
}

//
//void handleTouch(void) {
//    TS_StateTypeDef ts;
//    BSP_TS_GetState(&ts);
//    if (!ts.TouchDetected) return;
//
//    uint16_t x = ts.X;
//    uint16_t y = ts.Y;
//
//    if (!gameActive) {
//        if (x > 60 && x < 260) {
//            if (y > 100 && y < 150) {
//                singlePlayerMode = true;
//                resetGame();
//            } else if (y > 180 && y < 230) {
//                singlePlayerMode = false;
//                resetGame();
//            }
//        }
//    } else {
//        if (y < 240) {
//            if (x < BSP_LCD_GetXSize() / 2) currentColumn = (currentColumn > 0) ? currentColumn - 1 : currentColumn;
//            else currentColumn = (currentColumn < COLS - 1) ? currentColumn + 1 : currentColumn;
//            drawGameBoard();
//        }
//    }
//    HAL_Delay(150);
//}
//


bool checkWin(void) {
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            int player = board[r][c];
            if (player == EMPTY) continue;
            if (c + 3 < COLS && player == board[r][c + 1] && player == board[r][c + 2] && player == board[r][c + 3]) return true;
            if (r + 3 < ROWS && player == board[r + 1][c] && player == board[r + 2][c] && player == board[r + 3][c]) return true;
            if (r + 3 < ROWS && c + 3 < COLS && player == board[r + 1][c + 1] && player == board[r + 2][c + 2] && player == board[r + 3][c + 3]) return true;
            if (r + 3 < ROWS && c - 3 >= 0 && player == board[r + 1][c - 1] && player == board[r + 2][c - 2] && player == board[r + 3][c - 3]) return true;
        }
    }
    return false;
}


bool isBoardFull(void) {
    for (int c = 0; c < COLS; c++) {
        if (board[0][c] == EMPTY) return false;
    }
    return true;
}


void drawGameOverScreen(bool win) {
//    gameActive = false;
//    uint32_t duration = HAL_GetTick() - roundStartTime;
	uint32_t timeElapsed = __HAL_TIM_GET_COUNTER(&htim2);
//	HAL_TIM_Base_Stop(&htim2);
	__HAL_TIM_SET_COUNTER(&htim2, 0);

	HAL_Delay(1000);

    LCD_Clear(0, LCD_COLOR_WHITE);
//    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
//    BSP_LCD_DisplayStringAt(0, 30, (uint8_t *)(win ? "Winner!" : "It's a Tie!"), CENTER_MODE);


    if(win == false){
    LCD_DisplayChar(40,80,'T');
	LCD_DisplayChar(55,80,'i');
	LCD_DisplayChar(70,80,'e');
//	LCD_DisplayChar(85,80,'');
	LCD_DisplayChar(100,80,'G');
	LCD_DisplayChar(115,80,'a');
	LCD_DisplayChar(130,80,'m');
	LCD_DisplayChar(145,80,'e');
    }

    else{
		LCD_DisplayChar(40,70,'W');
		LCD_DisplayChar(55,70,'i');
		LCD_DisplayChar(70,70,'n');
		LCD_DisplayChar(85,70,'n');
		LCD_DisplayChar(100,70,'e');
		LCD_DisplayChar(115,70,'r');

		LCD_DisplayChar(40,90,'P');
		LCD_DisplayChar(55,90,'l');
		LCD_DisplayChar(70,90,'a');
		LCD_DisplayChar(85,90,'y');
		LCD_DisplayChar(100,90,'e');
		LCD_DisplayChar(115,90,'r');

		if(currentPlayer == 1){
			LCD_DisplayChar(145,90,'1');
		}
		else{
			LCD_DisplayChar(145,90,'2');
		}
    }




    LCD_DisplayChar(30,130,'T');
	LCD_DisplayChar(45,130,'i');
	LCD_DisplayChar(60,130,'m');
	LCD_DisplayChar(75,130,'e');
	LCD_DisplayChar(90,130,':');

	int lastI;
	char timeStr[12];
	//	sprintf(scoreStr2, "000");
		sprintf(timeStr, "%lu", timeElapsed);

		LCD_DisplayChar(105, 130, timeStr[0]);
		LCD_DisplayChar(120, 130, '.');

		for(int i = 1; i < 6; i++){
			LCD_DisplayChar(120 + (15 * i), 130, timeStr[i]);
			lastI = i;
		}

		LCD_DisplayChar(120 + (15 * (lastI + 1)), 130, 's');

//	LCD_DisplayChar(115,90,'r');

//    char scoreStr[64];
//    snprintf(scoreStr, sizeof(scoreStr), "Red: %d  Yellow: %d", scoreP1, scoreP2);
//    BSP_LCD_DisplayStringAt(0, 70, (uint8_t *)scoreStr, CENTER_MODE);
//
//    char timeStr[64];
//    snprintf(timeStr, sizeof(timeStr), "Time: %lu sec", duration / 1000);
//    BSP_LCD_DisplayStringAt(0, 110, (uint8_t *)timeStr, CENTER_MODE);

    LCD_Draw_Square_Fill(125, 205, 95, LCD_COLOR_RED);
	LCD_DisplayChar(130,240,'R');
	LCD_DisplayChar(145,240,'e');
	LCD_DisplayChar(160,240,'s');
	LCD_DisplayChar(170,240,'t');
	LCD_DisplayChar(180,240,'a');
	LCD_DisplayChar(190,240,'r');
	LCD_DisplayChar(200,240,'t');


	LCD_DisplayChar(20,210,'S');
	LCD_DisplayChar(35,210,'c');
	LCD_DisplayChar(50,210,'o');
	LCD_DisplayChar(65,210,'r');
	LCD_DisplayChar(80,210,'e');
	LCD_DisplayChar(95,210,'s');


	char scoreStr1[12];
//	sprintf(scoreStr1, "000");
	sprintf(scoreStr1, "%d", scoreP1); // \0 =(

	char scoreStr2[12];
//	sprintf(scoreStr2, "000");
	sprintf(scoreStr2, "%d", scoreP2);


	LCD_DisplayChar(20,230,'P');
	LCD_DisplayChar(35,230,'1');
	LCD_DisplayChar(50,230,':');

	for(int i = 0; scoreStr1[i] != '\0'; i++){
		LCD_DisplayChar(65 + (15 * i), 230, scoreStr1[i]);
	}


	LCD_DisplayChar(20,250,'P');
	LCD_DisplayChar(35,250,'2');
	LCD_DisplayChar(50,250,':');

	for(int i = 0; scoreStr2[i] != '\0'; i++){
		LCD_DisplayChar(65 + (15 * i), 250, scoreStr2[i]);
	}

//    BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
//    BSP_LCD_FillRect(80, 160, 160, 50);
//    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
//    BSP_LCD_DisplayStringAt(0, 175, (uint8_t *)"Restart", CENTER_MODE);

//    while (1) {
//        TS_StateTypeDef ts;
//        BSP_TS_GetState(&ts);
//        if (ts.TouchDetected && ts.X > 80 && ts.X < 240 && ts.Y > 160 && ts.Y < 210) {
//            resetGame();
//            break;
//        }
//    }
}


//
int getAIMove(void) {
	int col;
	uint32_t randomNum;
	HAL_RNG_GenerateRandomNumber(&hrng, &randomNum);
//		HAL_RNG_GenerateRandomNumber(&hrng, &randomNum);

//	}
    return randomNum % COLS;
}
////
////void resetGame(void) {
////    memset(board, 0, sizeof(board));
////    currentColumn = 3;
////    currentPlayer = PLAYER1;
////    roundStartTime = HAL_GetTick();
////    gameActive = true;
////    drawGameBoard();
////}
