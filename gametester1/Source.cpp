#include <iostream>
#include <time.h>
#include <cstdlib>
#include <windows.h>
#include <process.h>
#include <conio.h>
using namespace std;
#define MAX 100
#define WIDTH 80
#define HEIGHT 30
#define SNAKE_LENGTH 10
#define POISON 2
#define FOOD 1
#define RUNTIME 3
#define BLOCK -3
#define WALL -2
#define SNAKE -1
#define NOTHING 0
#define RIGHT 0
#define UP 1
#define LEFT 2
#define DOWN 3
#define EXIT -1
#define scount 10
static int dx[5] = { 1, 0, -1, 0 };
static int dy[5] = { 0, -1, 0, 1 };
int input = RIGHT;
int item = NOTHING;
int gameTime;
int score;
bool foodstate = false;
bool poisonstate = false;
bool snakestate = false;


void setcolor(int fg, int bg)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, bg * 16 + fg);
}

void pause() {
	while (1) {
		for (int i = 0; i < 255; i++) {
			if (GetAsyncKeyState(i))
				return;
		}
	}
}

void gotoxy(int column, int row)
{
	HANDLE hStdOut;
	COORD coord;
	hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hStdOut == INVALID_HANDLE_VALUE) return;
	coord.X = column;
	coord.Y = row;
	SetConsoleCursorPosition(hStdOut, coord);
}

void clearScreen()
{
	HANDLE hStdOut;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD count;
	DWORD cellCount;
	COORD homeCoords = { 0, 0 };
	hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hStdOut == INVALID_HANDLE_VALUE) return;
	if (!GetConsoleScreenBufferInfo(hStdOut, &csbi)) return;
	cellCount = csbi.dwSize.X * csbi.dwSize.Y;
	if (!FillConsoleOutputCharacter(hStdOut, (TCHAR)' ', cellCount, homeCoords, &count)) return;
	if (!FillConsoleOutputAttribute(hStdOut, csbi.wAttributes, cellCount, homeCoords, &count)) return;
	SetConsoleCursorPosition(hStdOut, homeCoords);
}

int oppositeDirection(int input1, int input2)
{
	if (input1 == LEFT && input2 == RIGHT)
		return 1;
	if (input1 == RIGHT && input2 == LEFT)
		return 1;
	if (input1 == UP && input2 == DOWN)
		return 1;
	if (input1 == DOWN && input2 == UP)
		return 1;
	return 0;
}

struct Coordinate
{
	int x, y;
};

class snake
{
private:
	int length;
	Coordinate body[WIDTH * HEIGHT];
	int direction;
	int ground[MAX][MAX];
	int score;
	Coordinate food[scount];
	Coordinate block[scount];
	Coordinate poison[scount];
public:
	void Ground();
	void initSnake();
	void drawSnake(int delay);
	void randFood();
	void eraseFood();
	void randBlock();
	void eraseBlock();
	void randPoison();
	void erasePoison();
	void firstDraw();
	int foodCounter();
};

void snake::Ground()
{
	setcolor(8, 0);
	int i, j;
	for (i = 0; i < MAX; i++)
		for (j = 0; j < MAX; j++)
			ground[i][j] = 0;

	for (i = 0; i <= WIDTH + 1; i++)
	{
		//top bottom wall
		ground[0][i] = WALL;
		ground[HEIGHT + 1][i] = WALL;
	}
	for (i = 0; i <= HEIGHT + 1; i++)
	{
		//right left wall
		ground[i][0] = WALL;
		ground[i][WIDTH + 1] = WALL;
	}
}

void snake::initSnake()
{
	length = SNAKE_LENGTH;
	body[0].x = WIDTH / 2;
	body[0].y = HEIGHT / 2;
	direction = input;
	score = 0;

	int i;
	for (i = 1; i < length; i++)
	{
		body[i].x = body[i - 1].x - dx[direction];	//��Ѻ��� ���仫���
		body[i].y = body[i - 1].y - dy[direction];
	}
	//���˹觧�
	for (i = 0; i < length; i++)
		ground[body[i].y][body[i].x] = SNAKE;
}

void snake::drawSnake(int delay)
{
	int i;
	Coordinate prev[WIDTH * HEIGHT];
	for (i = 0; i < length; i++)
	{
		prev[i].x = body[i].x;
		prev[i].y = body[i].y;
	}

	if (input != EXIT && !oppositeDirection(direction, input))
		direction = input;
	body[0].x = prev[0].x + dx[direction];
	body[0].y = prev[0].y + dy[direction];

	if (ground[body[0].y][body[0].x] < NOTHING)
	{
		item = -1;
		return;
	}

	if (ground[body[0].y][body[0].x] == FOOD)
	{
		ground[body[length - 1].y][body[length - 1].x] = NOTHING;
		item = NOTHING;
		ground[body[length - 2].y][body[length - 2].x] = NOTHING;
		item = NOTHING;
		gotoxy(body[length - 1].x, body[length - 1].y);
		cout << " ";
		gotoxy(body[length - 2].x, body[length - 2].y);
		cout << " ";
		length -= 2;
		item = FOOD;
		score++;
	}

	if (ground[body[0].y][body[0].x] == POISON)
	{
		length++;
		item = POISON;
	}

	if (gameTime != 0 && gameTime % 2 == 0 && !snakestate)
	{
		snakestate = true;
		length++;
		item = RUNTIME;
	}
	else if (gameTime != 0 && gameTime % 2 != 0 && snakestate)
	{
		snakestate = false;
		ground[body[length - 1].y][body[length - 1].x] = NOTHING;
		item = NOTHING;
		gotoxy(body[length - 1].x, body[length - 1].y);
		cout << " ";
	}

	else
	{
		ground[body[length - 1].y][body[length - 1].x] = NOTHING;
		item = NOTHING;
		gotoxy(body[length - 1].x, body[length - 1].y);		// ������������� ���ǡ�ҧ
		cout << " ";
	}

	for (i = 1; i < length; i++)
	{
		body[i].x = prev[i - 1].x;	//��ǧ٢�Ѻ������˹觡�͹˹��
		body[i].y = prev[i - 1].y;
	}

	setcolor(2, 0);
	gotoxy(body[1].x, body[1].y);
	cout << "o";
	gotoxy(body[0].x, body[0].y);
	cout << "O";

	for (i = 0; i < length; i++)
		ground[body[i].y][body[i].x] = SNAKE;

	Sleep(delay);
	return;
}

void snake::randFood()
{
	setcolor(6, 0);
	int x, y;
	for (int i = 0;i < 5;i++) {
		do
		{
			food[i].x = rand() % WIDTH + 1;
			food[i].y = rand() % HEIGHT + 1;
		} while (ground[food[i].y][food[i].x] != NOTHING);
		ground[food[i].y][food[i].x] = FOOD;
		gotoxy(food[i].x, food[i].y); cout << "#";
	}
}

void snake::eraseFood()
{
	int x, y;
	for (int i = 0;i < 5;i++) {
		ground[food[i].y][food[i].x] = NOTHING;
		gotoxy(food[i].x, food[i].y); cout << " ";
	}
}

void snake::randBlock()
{
	setcolor(3, 0);
	int x, y;
	for (int i = 0;i < 10;i++) {
		do
		{
			block[i].x = rand() % WIDTH - 6;
			block[i].y = rand() % HEIGHT;
		} while (ground[block[i].y][block[i].x] != NOTHING);
		ground[block[i].y][block[i].x] = BLOCK;
		ground[block[i].y][block[i].x + 1] = BLOCK;
		ground[block[i].y][block[i].x + 2] = BLOCK;
		ground[block[i].y][block[i].x + 3] = BLOCK;
		ground[block[i].y][block[i].x + 4] = BLOCK;
		ground[block[i].y][block[i].x + 5] = BLOCK;
		ground[block[i].y][block[i].x + 6] = BLOCK;
		ground[block[i].y + 1][block[i].x] = BLOCK;
		ground[block[i].y + 1][block[i].x + 1] = BLOCK;
		ground[block[i].y + 1][block[i].x + 2] = BLOCK;
		ground[block[i].y + 1][block[i].x + 3] = BLOCK;
		ground[block[i].y + 1][block[i].x + 4] = BLOCK;
		ground[block[i].y + 1][block[i].x + 5] = BLOCK;
		ground[block[i].y + 1][block[i].x + 6] = BLOCK;
		gotoxy(block[i].x, block[i].y); cout << "X";
		gotoxy(block[i].x + 1, block[i].y); cout << "X";
		gotoxy(block[i].x + 2, block[i].y); cout << "X";
		gotoxy(block[i].x + 3, block[i].y); cout << "X";
		gotoxy(block[i].x + 4, block[i].y); cout << "X";
		gotoxy(block[i].x + 5, block[i].y); cout << "X";
		gotoxy(block[i].x + 6, block[i].y); cout << "X";
		gotoxy(block[i].x, block[i].y + 1); cout << "X";
		gotoxy(block[i].x + 1, block[i].y + 1); cout << "X";
		gotoxy(block[i].x + 2, block[i].y + 1); cout << "X";
		gotoxy(block[i].x + 3, block[i].y + 1); cout << "X";
		gotoxy(block[i].x + 4, block[i].y + 1); cout << "X";
		gotoxy(block[i].x + 5, block[i].y + 1); cout << "X";
		gotoxy(block[i].x + 6, block[i].y + 1); cout << "X";
	}
}

void snake::eraseBlock()
{
	int x, y;
	for (int i = 0;i < 10;i++) {
		ground[food[i].y][food[i].x] = NOTHING;
		gotoxy(block[i].x, block[i].y); cout << " ";
		gotoxy(block[i].x + 1, block[i].y); cout << " ";
		gotoxy(block[i].x + 2, block[i].y); cout << " ";
		gotoxy(block[i].x + 3, block[i].y); cout << " ";
		gotoxy(block[i].x + 4, block[i].y); cout << " ";
		gotoxy(block[i].x + 5, block[i].y); cout << " ";
		gotoxy(block[i].x + 6, block[i].y); cout << " ";
		gotoxy(block[i].x, block[i].y + 1); cout << " ";
		gotoxy(block[i].x + 1, block[i].y + 1); cout << " ";
		gotoxy(block[i].x + 2, block[i].y + 1); cout << " ";
		gotoxy(block[i].x + 3, block[i].y + 1); cout << " ";
		gotoxy(block[i].x + 4, block[i].y + 1); cout << " ";
		gotoxy(block[i].x + 5, block[i].y + 1); cout << " ";
		gotoxy(block[i].x + 6, block[i].y + 1); cout << " ";
	}
}

void snake::randPoison()
{
	setcolor(4, 0);
	int x, y;
	for (int i = 0;i < 5;i++) {
		do
		{
			poison[i].x = rand() % WIDTH;
			poison[i].y = rand() % HEIGHT;
		} while (ground[poison[i].y][poison[i].x] != NOTHING);
		ground[poison[i].y][poison[i].x] = POISON;
		gotoxy(poison[i].x, poison[i].y);
		cout << "@";
	}
}

void snake::erasePoison()
{
	int x, y;
	for (int i = 0;i < 5;i++) {
		ground[poison[i].y][poison[i].x] = NOTHING;
		gotoxy(poison[i].x, poison[i].y); cout << " ";
	}
}

void snake::firstDraw()
{
	clearScreen();
	int i, j;
	for (i = 0; i <= HEIGHT + 1; i++)
	{
		for (j = 0; j <= WIDTH + 1; j++)
		{
			switch (ground[i][j])
			{
			case NOTHING:
				cout << " "; break;
			case WALL:
				if ((i == 0 && j == 0) || (i == 0 && j == WIDTH + 1) || (i == HEIGHT + 1 && j == 0) || (i == HEIGHT + 1 && j == WIDTH + 1))
				{
					setcolor(8, 0);
					cout << "+";
				}
				else {
					if (j == 0 || j == WIDTH + 1)
					{
						setcolor(8, 0);
						cout << "|";
					}
					//left right wall
					else {
						setcolor(8, 0);
						cout << "-";
					}   //top bottom wall
					break;
				}
			case SNAKE:
				if (i == body[0].y && j == body[0].x) {
					setcolor(2, 0);
					cout << "O";
				}
				else {
					setcolor(2, 0);
					cout << "o";
				}
				break;
			default:				//food
				setcolor(6, 0);
				cout << "#";
			}
		}
		cout << endl;
	}
}

int snake::foodCounter()
{
	return score;
}

void userInput(void* id)
{
	do
	{
		int c = _getch();
		switch (c)
		{
		case 'W': case 'w': input = UP; break;
		case 'S': case 's': input = DOWN; break;
		case 'D': case 'd': input = RIGHT; break;
		case 'A': case 'a': input = LEFT; break;
		case 27:	    input = EXIT; break;
		case 'X': case 'x': input = EXIT; break;
			//case 'P': case 'p': pause();
		}
	} while (input != EXIT && item >= 0);

	_endthread();
	return;
}

int main()
{
	int delay = 100;
	int wait;
	time_t startTime = time(NULL);
	srand(time(NULL));
	snake S;
	S.Ground();
	S.initSnake();
	S.randFood();
	S.firstDraw();
	_beginthread(userInput, 0, (void*)0);

	do
	{
		//int c = _getch();{if (c == 'p' || c == 'P') { pause(); }}
		gotoxy(1, HEIGHT + 2);
		gameTime = difftime(time(NULL), startTime);
		cout << "TIME : " << gameTime;
		S.drawSnake(delay);
		if (item == FOOD)
		{
			S.randFood();
			delay = 100;
		}
		if (item == POISON)
		{
			delay = 50;
		}
		if (item == RUNTIME)
		{
			delay = 100;
		}
		if (gameTime % 2 == 0)//����delay
		{
			S.drawSnake(delay);
		}
		if (gameTime != 0 && gameTime % 15 == 0 && !poisonstate) //����delay
		{
			poisonstate = true;
			S.erasePoison();
			S.randPoison();

		}
		else if (gameTime % 15 != 0 && poisonstate) //����delay
		{
			poisonstate = false;
			//S.erasePoison();
			//S.randPoison();
		}
		if (gameTime != 0 && gameTime % 15 == 0 && !foodstate) //����delay
		{
			foodstate = true;
			S.eraseFood();
			S.randFood();
		}
		else if (gameTime % 15 != 0 && foodstate) //����delay
		{
			foodstate = false;
			//S.eraseFood();
		//	S.randFood();
		}

	} while (item >= 0 && input != EXIT);

	//clearScreen(); //��������ѧ
	setcolor(7, 0);
	gotoxy(WIDTH / 2 - 5, HEIGHT / 2 - 2);
	cout << "GAME OVER";
	gotoxy(WIDTH / 2 - 5, HEIGHT / 2);
	cout << "score : " << S.foodCounter() << endl;
	gotoxy(WIDTH / 2, HEIGHT / 2);
	_getch();
	return 0;
}