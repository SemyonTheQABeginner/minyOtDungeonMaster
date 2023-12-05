#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>


#define MAX_SIZE 101

int field[MAX_SIZE][MAX_SIZE];
int field_opened[MAX_SIZE][MAX_SIZE];

void generateField(int a, int b, int q_mines) {
	for (int i = 1; i < MAX_SIZE; i++) {
		for (int j = 1; j < MAX_SIZE; j++) {
			field[i][j] = 0;
		}
	}

	for (int i = 1; i < q_mines + 1; i++) {
		int x = rand() % a + 1;
		int y = rand() % b + 1;
		if (field[x][y] != -1)
			field[x][y] = -1;
		else {
			i--;
			continue;
		}
	}

	for (int i = 1; i < a + 1; i++) {
		for (int j = 1; j < b + 1; j++) {
			if (field[i][j] != -1) {
				int count_mines = 0;

				if (field[i - 1][j - 1] == -1)
					count_mines++;
				if (field[i - 1][j] == -1)
					count_mines++;
				if (field[i - 1][j + 1] == -1)
					count_mines++;

				if (field[i][j - 1] == -1)
					count_mines++;
				if (field[i][j + 1] == -1)
					count_mines++;

				if (field[i + 1][j - 1] == -1)
					count_mines++;
				if (field[i + 1][j] == -1)
					count_mines++;
				if (field[i + 1][j + 1] == -1)
					count_mines++;

				field[i][j] = count_mines;
			}
		}
	}
}

void printField(int a, int b) {
	printf("    ");
	for (int i = 1; i < b + 1; i++) {
		printf("%3d ", i);
	}
	printf("\n");
	for (int i = 1; i < a + 1; i++) {
		printf("%3d ", i);
		for (int j = 1; j < b + 1; j++) {
			if (field_opened[i][j]) {
				if (field[i][j] == -1) {
					printf("%3c ", '*');
				} else {
					printf("%3c ", field[i][j] + '0');
				}
			} else {
				printf("%3c ", '#');
			}
		}
		printf("\n");
	}
}

int gameWin(int a, int b) {
	for (int i = 1; i < a + 1; i++) {
		for (int j = 1; j < b + 1; j++) {
			if (!field_opened[i][j] && field[i][j] != -1)
				return 0;
		}
	}
	return 1;
}

int makeMove(int x, int y, int a, int b) {
	if (field[x][y] == -1) {
		field_opened[x][y] = 1;
		return 0;
	} else {
		field_opened[x][y] = 1;
		if (field[x][y] == 0) {
			if (x - 1 > 0 && y - 1 > 0 && !field_opened[x - 1][y - 1])
				makeMove(x - 1, y - 1, a, b);
			if (x - 1 > 0 && !field_opened[x - 1][y])
				makeMove(x - 1, y, a, b);
			if (x - 1 > 0 && y + 1 <= b && !field_opened[x - 1][y + 1])
				makeMove(x - 1, y + 1, a, b);
			if (y - 1 > 0 && !field_opened[x][y - 1])
				makeMove(x, y - 1, a, b);
			if (y + 1 <= b && !field_opened[x][y + 1])
				makeMove(x, y + 1, a, b);
			if (x + 1 <= a && y - 1 > 0 && !field_opened[x + 1][y - 1])
				makeMove(x + 1, y - 1, a, b);
			if (x + 1 <= a && !field_opened[x + 1][y])
				makeMove(x + 1, y, a, b);
			if (x + 1 <= a && y + 1 <= b && !field_opened[x + 1][y + 1])
				makeMove(x + 1, y + 1, a, b);
		}
		return 1;
	}
}

void clearConsole() {
	system("cls"); 
}

void game(int a, int b, int q_mines, long *result) {
	LARGE_INTEGER start_time, end_time, frequency;
	int count_moves = 0;
	int game_result = 0;
	generateField(a, b, q_mines);
	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&start_time);


	while (1) {
		int x, y;
		while (1) {
			clearConsole();
			printField(a, b);
			printf("Enter coordinates (first number - row, second number - "
				   "column): ");
			if (scanf("%d %d", &x, &y) != 2 || x < 1 || x > a || y < 1 ||
				y > b) {
				clearConsole();
				printf("Invalid input. Try again.\n");
			} else {
				break;
			}
		}

		if (count_moves == 0) {
			while (1) {
				int zero_present = 0;
				for (int i = 1; i < a + 1; i++) {
					for (int j = 1; j < b + 1; j++) {
						if (field[i][j] == 0) {
							zero_present = 1;
							break;
						}
					}
					if (zero_present) {
						break;
					}
				}
				if (zero_present && field[x][y] == 0) {
					break;
				} else if (!zero_present && field[x][y] != -1) {
					break;
				}
				generateField(a, b, q_mines);
			}
		}

		if (makeMove(x, y, a, b)) {
			count_moves++;
			if (gameWin(a, b)) {
				clearConsole();
				printField(a, b);
				if (a == 100 && b == 100) {
					printf("GOOOOOOOIIIDDDAAAAAAA!!!\n");
				} else {
					printf("Nice job!\n");
				}
				game_result = 1;
				break;	
			}
			
		} else {
			clearConsole();
			printField(a, b);
			printf("You lose, try again!\n");
			break;
		}
	}
	
	QueryPerformanceCounter(&end_time);

	if (game_result) {
		*result = (end_time.QuadPart - start_time.QuadPart) * 1000000 / frequency.QuadPart;
	}
	else {
		*result = -1;
	}
	printf("%lld\n", *result);
}

int main() {
	srand(time(NULL));

	for (int i = 1; i < MAX_SIZE; i++) {
		for (int j = 1; j < MAX_SIZE; j++) {
			field_opened[i][j] = 0;
		}
	}

	clearConsole();
	printf("================= Welcome To MineSweeper! =================\n\n");
	printf("Select option:\n1. Start game\n2. Previous result\n3. Exit\n-> ");
	char option;
	scanf("%c", &option);

	switch (option) {
	case '1': {
		long time_elapsed;
		int count_moves = 0;
		int a, b, q_mines;
		clearConsole();
		while (1) {
			printf("Enter the size of the field (two numbers a x b).\nNumbers "
				   "must "
				   "be less than 100 and bigger, than 5: ");
			if (scanf("%d %d", &a, &b) != 2 || a > 100 || a < 5 || b > 100 ||
				b < 5) {
				clearConsole();
				printf("Invalid input. Try again.\n");
			} else {
				break;
			}
		}
		clearConsole();
		while (1) {
			printf("Enter the number of mines in the field (one number)\nThe "
				   "number of mines must be less than 75 percent of the total "
				   "number of cells in the field: ");
			if (scanf("%d", &q_mines) != 1 ||
				q_mines * 100 / (double)(a * b) >= 75) {
				clearConsole();
				printf("Invalid input. Try again.\n");
			} else {
				break;
			}
		}
		game(a, b, q_mines, &time_elapsed);
		if (time_elapsed != -1) {
			printf("Time elapsed: %f seconds\n", time_elapsed / 1000000.);
			FILE *file = fopen("result.txt", "w");
			if (file == NULL) {
				perror("OPEN FILE ERROR\n");
				return 1;
			}
			fprintf(file, "%f", time_elapsed / 1000000.);
			fclose(file);
		}
		break;
	}
	case '2': {
		FILE *file = fopen("result.txt", "r");
		if (file == NULL) {
			perror("OPEN FILE ERROR\n");
			return 1;
		}

		printf("Your previous time is ");
		int ch;
		while ((ch = fgetc(file)) != EOF) {
			putchar(ch);
		}
		printf(" seconds.\n");
		return 0;
	}
	case '3': {
		return 0;
	}
	default: {
		printf("You choosed wrong door, the leather club is two block down.\n");
		return 0;
	}
	}
	return 0;
}
