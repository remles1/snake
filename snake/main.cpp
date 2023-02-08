#include <iostream>
#include <vector>
#include <windows.h>
#include <conio.h>

std::vector<std::vector<int>> snake_body = {}; // { {x,y,wektor_kierunkowy} }			//wektor kierunkowy:
int width = 20;																			//
int height = 20;																		//		-2
																						//	  -1   1
int pos_x = width/2;																	//	     2
int pos_y = height/2;																	//
																						//
int apple_x = 0;
int apple_y = 0;
bool generate_apple = true;

int score = 0;
int difficulty_start = 100;
int difficulty = difficulty_start;
int difficulty_jump = 1;

void gotoxy(int x, int y) {
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void print_board() {
	for (int i = 0; i <= width; i++) {
		for (int j = 0; j <= height; j++) {
			if (i == 0 || i == width || j == 0 || j == height) {
				gotoxy(i, j);
				std::cout << "*";
			}
		}
	}

	gotoxy(width+10,height/2);
	std::cout << "SCORE: 0";
}

void move_head() {
	if (snake_body[0][2] == 1) {
		pos_x++;
	}
	else if (snake_body[0][2] == -1) {
		pos_x--;
	}
	else if (snake_body[0][2] == 2) {
		pos_y++;
	}
	else if (snake_body[0][2] == -2) {
		pos_y--;
	}
}

bool check_if_hit_body() {
	int count = 0;
	for (int i = 1; i < snake_body.size(); i++) {
		if (pos_x == snake_body[i][0] && pos_y == snake_body[i][1]) {
			count++;
			if (count > 1) {
				return true;
			}
		}
	}
	return false;
}

void game_over() {
	std::cout << "GAME OVER";
	while (true);
}

int main() {
	srand(time(NULL));
	system("CLS");

	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.nFont = 0;
	cfi.dwFontSize.X = 16;                  
	cfi.dwFontSize.Y = 16;                  
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	SetCursor(NULL);
	std::wcscpy(cfi.FaceName, L"Terminal");
	
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);

	

	print_board();														//	 -2	
																		// -1	1
																		//    2
	snake_body.push_back({pos_x,pos_y,1}); //head
	snake_body.push_back({ pos_x-1,pos_y,1 });
	snake_body.push_back({ pos_x-2,pos_y,1 });
	


	while (true) {
		if (generate_apple) {
			apple_x = rand() % (width - 2) + 1;
			apple_y = rand() % (height - 2) + 1;
			generate_apple = false;
		}
		gotoxy(apple_x, apple_y);
		putchar('X');

		if (_kbhit()) {
			switch (_getch())
			{
			case 'w':
				snake_body[0][2] = -2;
				break;
			case 's':
				snake_body[0][2] = 2;
				break;
			case 'd':
				snake_body[0][2] = 1;
				break;
			case 'a':
				snake_body[0][2] = -1;
				break;
			}
			
		}

		move_head();
		
		gotoxy(pos_x,pos_y);
		putchar(254);

		snake_body.insert(snake_body.begin() + 1, {pos_x,pos_y,snake_body[0][2]});
		
		int x_tail = snake_body.size()-1;
		int y_tail = snake_body.size()-1;

		gotoxy(snake_body[x_tail][0], snake_body[y_tail][1]);
		putchar(32);

		snake_body.pop_back();
		
		if (pos_x == apple_x && pos_y == apple_y) {
			generate_apple = true;
			
			switch (snake_body[snake_body.size()-1][2])
			{
			case 1:
				snake_body.push_back({ snake_body[ snake_body.size()-1 ][0] - 1,snake_body[snake_body.size() - 1][1],1 });
				break;
			case -1:
				snake_body.push_back({ snake_body[snake_body.size() - 1][0] + 1,snake_body[snake_body.size() - 1][1],1 });
				break;
			case 2:
				snake_body.push_back({ snake_body[snake_body.size() - 1][0],snake_body[snake_body.size() - 1][1] - 1,1 });
				break;
			case -2:
				snake_body.push_back({ snake_body[snake_body.size() - 1][0],snake_body[snake_body.size() - 1][1] + 1,1 });
				break;
			}
			difficulty -= difficulty_jump;
			score += 10 * (difficulty_start - difficulty);
			gotoxy(width+17, height/2);
			std::cout << score;
		}

		if (pos_x == 0 || pos_x == width || pos_y == 0 || pos_y == height) {
			game_over();
		}
		else if (check_if_hit_body()) {
			game_over();
		}

		Sleep(difficulty);
	}
}


