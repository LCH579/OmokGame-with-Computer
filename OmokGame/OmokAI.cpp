#include<stdio.h>
#include<stdlib.h>
#include<Windows.h>
#include<time.h>
#include<conio.h>

#define BOARD_SIZE 19
#define WHITE 1
#define BLACK 2

int checkCapability(int[]);
int AI_init(int, int[]);
void continueousScore(int[][BOARD_SIZE], int[][BOARD_SIZE]);
void extractBoard(int[][BOARD_SIZE], int, int[][BOARD_SIZE]);
void sumArray(int[][BOARD_SIZE], int[][BOARD_SIZE], int[][BOARD_SIZE]);
void filterDisable(int[][BOARD_SIZE]);
int getBestPosition(int[][BOARD_SIZE], int[][BOARD_SIZE], int[][BOARD_SIZE], int[]);
int getAbsPosition(int[][BOARD_SIZE], int[]);
extern int checkFiveInRange(int[]);
extern int checkFive(int[]);
extern int now_board[][BOARD_SIZE];


//우선순위
//------------------------------------
//(중요)
//1. 내 돌이 4개인 지점을 찾는다.
//2. 상대방 돌이 4개인 지점을 찾는다.
//3. 내 돌이 3개인 지점을 찾는다.
//4. 상대방 돌이 3개인 지점을 찾는다.
//------------------------------------
//(기타)
//5. 내 돌의 연속성을 찾는다.
//->연속된 개수(4개 > 3개 > 2개 > 1개)에 따라서 점수부여
//->공격, 수비의 점수를 합쳐서 착수함


/*
//This methods are for debugging
int now_board_temp[BOARD_SIZE][BOARD_SIZE] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

void printArray(int arr[][BOARD_SIZE]);
int main() {
	system("mode con: cols=100 lines=41");

	int firstBoard[BOARD_SIZE][BOARD_SIZE];
	continueousScore(now_board_temp, firstBoard);

	//printArray(now_board_temp);
	printArray(firstBoard);
	return 0;
}

void printArray(int arr[][BOARD_SIZE]) {
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			printf("%4d", arr[i][j]);
		}
		printf("\n\n");
	}
}

void printLargerArray(int arr[][BOARD_SIZE + 2]) {
	for (int i = 0; i < BOARD_SIZE + 2; i++) {
		for (int j = 0; j < BOARD_SIZE + 2; j++) {
			printf("%4d", arr[i][j]);
		}
		printf("\n\n");
	}
}
//End of Debugging Methods
*/

int AI_init(int color, int bestPosition[]) {
	
	//STEP 1
	if (getAbsPosition(now_board, bestPosition)) {
		return 1;
	}

	//STEP 2
	int def_board[BOARD_SIZE][BOARD_SIZE] = { 0 };
	int atk_board[BOARD_SIZE][BOARD_SIZE] = { 0 };

	int def_score_board[BOARD_SIZE][BOARD_SIZE] = { 0 };
	int atk_score_board[BOARD_SIZE][BOARD_SIZE] = { 0 };

	int com_color, play_color;
	if (color == WHITE) {
		com_color = WHITE;
		play_color = BLACK;
	} else {
		com_color = BLACK;
		play_color = WHITE;
	}

	extractBoard(now_board, com_color, atk_board);
	extractBoard(now_board, play_color, def_board);

	continueousScore(atk_board, atk_score_board);
	continueousScore(def_board, def_score_board);

	int score_sum[BOARD_SIZE][BOARD_SIZE] = { 0 };
	sumArray(atk_score_board, def_score_board, score_sum);
	filterDisable(score_sum);
	getBestPosition(score_sum, atk_score_board, def_score_board, bestPosition);

	return 1;
}

int getRandomInt(int range) {
	srand(time(NULL));
	return (rand() % range + 1);
}

int getAbsPosition(int board[][BOARD_SIZE], int position[2]) {
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			if (board[i][j] == 0) {
				board[i][j] = WHITE;
				int positionTemp[2] = { i, j };
				if (checkFiveInRange(positionTemp)) {
					position[1] = positionTemp[0];
					position[0] = positionTemp[1];
					board[i][j] = 0;
					return 1;
				}

				board[i][j] = BLACK;
				if (checkFiveInRange(positionTemp)) {
					position[1] = positionTemp[0];
					position[0] = positionTemp[1];
					board[i][j] = 0;
					return 1;
				}
				board[i][j] = 0;
			}
		}
	 }
	return 0;
}

int getBestPosition(int score_arr[][BOARD_SIZE], int atk_score_board[][BOARD_SIZE], int def_score_board[][BOARD_SIZE], int position[2]) {
	//STEP 1
	int position_stamp[BOARD_SIZE * BOARD_SIZE][2] = {0}, max = 0, cntA = 0;
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			if (score_arr[i][j] > max) {
				cntA = 1;
				max = score_arr[i][j];
				position_stamp[cntA - 1][0] = i;
				position_stamp[cntA - 1][1] = j;
			} else if (score_arr[i][j] == max) {
				cntA++;
				position_stamp[cntA - 1][0] = i;
				position_stamp[cntA - 1][1] = j;
			}
		}
	}
	if (cntA == 1) {
	
		position[1] = position_stamp[0][0];
		position[0] = position_stamp[0][1];
		return 1;
	
	}

	//STEP 2
	int cntB = 0;
	for (int i = 0; i < cntA; i++) {
		score_arr[position_stamp[i][0]][position_stamp[i][1]] = score_arr[position_stamp[i][0]][position_stamp[i][1]] + atk_score_board[position_stamp[i][0]][position_stamp[i][1]];
		if (score_arr[position_stamp[i][0]][position_stamp[i][1]] > max) {
			cntB = 1;
			max = score_arr[position_stamp[i][0]][position_stamp[i][1]];
			position_stamp[cntB - 1][0] = position_stamp[i][0];
			position_stamp[cntB - 1][1] = position_stamp[i][1];
		} else if (score_arr[position_stamp[i][0]][position_stamp[i][1]] == max) {
			cntB++;
			position_stamp[cntB - 1][0] = position_stamp[i][0];
			position_stamp[cntB - 1][1] = position_stamp[i][1];
		}
	}

	if (cntB == 1) {
		position[1] = position_stamp[0][0];
		position[0] = position_stamp[0][1];
		return 1;
	}

	//STEP 3
	int cntC = 0;
	for (int i = 0; i < cntB; i++) {
		(score_arr[position_stamp[i][0]][position_stamp[i][1]] = score_arr[position_stamp[i][0]][position_stamp[i][1]] + atk_score_board[position_stamp[i][0]][position_stamp[i][1]]);
		if (score_arr[position_stamp[i][0]][position_stamp[i][1]] > max) {
			cntC = 1;
			max = score_arr[position_stamp[i][0]][position_stamp[i][1]];
			position_stamp[cntC - 1][0] = position_stamp[i][0];
			position_stamp[cntC - 1][1] = position_stamp[i][1];
		}
		else if (score_arr[position_stamp[i][0]][position_stamp[i][1]] = score_arr[position_stamp[i][0]][position_stamp[i][1]] + atk_score_board[position_stamp[i][0]][position_stamp[i][1]] == max) {
			cntC++;
			position_stamp[cntC - 1][0] = position_stamp[i][0];
			position_stamp[cntC - 1][1] = position_stamp[i][1];
		}
	}

	if (cntC == 1) {
		position[1] = position_stamp[0][0];
		position[0] = position_stamp[0][1];
		return 1;
	}

	//STEP 4
	int randomValue = getRandomInt(cntC);
	position[1] = position_stamp[randomValue - 1][0];
	position[0] = position_stamp[randomValue - 1][1];

	return 0;

}

void filterDisable(int target[][BOARD_SIZE]) {
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			if (now_board[i][j] != 0) {
				target[i][j] = -1;
			}
		}
	}
}


void sumArray(int arrA[][BOARD_SIZE],int arrB[][BOARD_SIZE], int result[][BOARD_SIZE]) {
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			result[i][j] = arrA[i][j] + arrB[i][j];
		}
	}
}

void extractBoard(int original_board[][BOARD_SIZE], int color, int extracted_board[][BOARD_SIZE]) {
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			if (original_board[i][j] == color) {
				extracted_board[i][j] = 1; //Exist
			}
		}
	}
}


void continueousScore(int board[][BOARD_SIZE], int return_board[][BOARD_SIZE]) {
	int boardTemp[BOARD_SIZE + 2][BOARD_SIZE + 2] = { 0 };
	int result_larger_board[BOARD_SIZE + 2][BOARD_SIZE + 2] = { 0 };
	for (int i = 1; i < BOARD_SIZE + 1; i++) {
		for (int j = 1; j < BOARD_SIZE + 1; j++) {
			boardTemp[i][j] = board[i - 1][j - 1];
		}
	}

	for (int i = 1; i < BOARD_SIZE + 1; i++) {
		for (int j = 1; j < BOARD_SIZE + 1; j++) {
			
			if (boardTemp[j][i] == 1) {
				int debug_j = j;
				//Horizon
				int cnt_plus = 0, cnt_minus = 0, k = 0;
				while (1) {
					int debug_j = j;
					k++;
					if (boardTemp[j][i + k] == 1) {
						cnt_plus++;
					}
					else {
						break;
					}
				}
				k = 0;
				while (1) {
					k--;
					if (boardTemp[j][i + k] == 1) {
						cnt_minus++;
					}
					else {
						break;
					}
				}

				int cnt_total = cnt_plus + cnt_minus + 1;
				if (cnt_total > result_larger_board[j][i + (cnt_plus + 1)]) {
					result_larger_board[j][i + (cnt_plus + 1)] = cnt_total;
				}
				if (cnt_total > result_larger_board[j][i - (cnt_minus + 1)]) {
					result_larger_board[j][i - (cnt_minus + 1)] = cnt_total;
				}

				//Vertical
				cnt_plus = 0, cnt_minus = 0, k = 0;
				while (1) {
					k++;
					if (boardTemp[j + k][i] == 1) {
						cnt_plus++;
					}
					else {
						break;
					}
				}
				k = 0;
				while (1) {
					k--;
					if (boardTemp[j + k][i] == 1) {
						cnt_minus++;
					}
					else {
						break;
					}
				}
				cnt_total = cnt_plus + cnt_minus + 1;
				if (cnt_total > result_larger_board[j + (cnt_plus + 1)][i]) {
					result_larger_board[j + (cnt_plus + 1)][i] = cnt_total;
				}
				if (cnt_total > result_larger_board[j - (cnt_minus + 1)][i]) {
					result_larger_board[j - (cnt_minus + 1)][i] = cnt_total;
				}
				
				//EAST NORTH
				cnt_plus = 0, cnt_minus = 0, k = 0;
				while (1) {
					k++;
					if (boardTemp[j + k][i + k] == 1) {
						cnt_plus++;
					}
					else {
						break;
					}
				}
				k = 0;
				while (1) {
					k--;
					if (boardTemp[j + k][i + k] == 1) {
						cnt_minus++;
					}
					else {
						break;
					}
				}
				cnt_total = cnt_plus + cnt_minus + 1;
				if (cnt_total > result_larger_board[j + (cnt_plus + 1)][i + (cnt_plus + 1)]) {
					result_larger_board[j + (cnt_plus + 1)][i + (cnt_plus + 1)] = cnt_total;
				}
				if (cnt_total > result_larger_board[j - (cnt_minus + 1)][i - (cnt_minus + 1)]) {
					result_larger_board[j - (cnt_minus + 1)][i - (cnt_minus + 1)] = cnt_total;
				}

				//EAST SOUTH
				cnt_plus = 0, cnt_minus = 0, k = 0;
				while (1) {
					k++;
					if (boardTemp[j + k][i - k] == 1) {
						cnt_plus++;
					}
					else {
						break;
					}
				}
				k = 0;
				while (1) {
					k--;
					if (boardTemp[j + k][i - k] == 1) {
						cnt_minus++;
					}
					else {
						break;
					}
				}
				cnt_total = cnt_plus + cnt_minus + 1;
				if (cnt_total > result_larger_board[j + (cnt_plus + 1)][i - (cnt_plus + 1)]) {
					result_larger_board[j + (cnt_plus + 1)][i - (cnt_plus + 1)] = cnt_total;
				}
				if (cnt_total > result_larger_board[j - (cnt_minus + 1)][i + (cnt_minus + 1)]) {
					result_larger_board[j - (cnt_minus + 1)][i + (cnt_minus + 1)] = cnt_total;
				}
			}
		}
	}

	for (int i = 1; i < BOARD_SIZE + 1; i++) {
		for (int j = 1; j < BOARD_SIZE + 1; j++) {
			return_board[i - 1][j - 1] = result_larger_board[i][j];
		}
	}
}


int checkCapability(int position[]) {
	if (now_board[position[1]][position[0]] != 0) {
		return 0;
	} else {
		return 1;
	}
}