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
#define INIT_SNAKE_LENGTH 10
#define POISON 2
#define FOOD 1
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
int gameTime, score;
int movement = 0;

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
	int foodCounter;
	Coordinate food[scount];
	Coordinate block[scount];
	Coordinate poison[scount];
public:
	void initGround();
	void initSnake();
	void updateSnake(int delay);
	void updateFood();
	void eraseFood();
	void updateBlock();
	void eraseBlock();
	void updatePoison();
	void erasePoison();
	void firstDraw();
	int getFoodCounter();
};

void snake::initGround()
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
	length = INIT_SNAKE_LENGTH;
	body[0].x = WIDTH / 2;
	body[0].y = HEIGHT / 2;
	direction = input;
	foodCounter = 0;

	int i;
	for (i = 1; i < length; i++)
	{
		body[i].x = body[i - 1].x - dx[direction];	//ขยับขวา ตัวไปซ้าย
		body[i].y = body[i - 1].y - dy[direction];
	}
	//ตำแหน่งงู
	for (i = 0; i < length; i++)
		ground[body[i].y][body[i].x] = SNAKE;
}

void snake::updateSnake(int delay)
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
	body[0].x = prev[0].x + dx[direction];		//หัวงู=ตามทาง
	body[0].y = prev[0].y + dy[direction];

	if (ground[body[0].y][body[0].x] < NOTHING)
	{
		item = -1;
		return;
	}

	if (ground[body[0].y][body[0].x] == FOOD)
	{
		length++;	
		item = FOOD;
	}

	else
	{
		ground[body[length - 1].y][body[length - 1].x] = NOTHING;
		item = NOTHING;
		gotoxy(body[length - 1].x, body[length - 1].y);		// ถ้าไม่ได้อาหาร ไม่บวกหาง
		cout << " ";
	}

	for (i = 1; i < length; i++)
	{
		body[i].x = prev[i - 1].x;	//ตัวงูขยับตามตำแหน่งก่อนหน้า
		body[i].y = prev[i - 1].y;
	}

	setcolor(2, 0);
	gotoxy(body[1].x, body[1].y);
	cout << "o";					//เปลี่ยนหัวเป็นตัว
	gotoxy(body[0].x, body[0].y);
	cout << "O";					//ใส่หัวเพิ่ม

	//ตำแหน่งงู
	for (i = 0; i < length; i++)
		ground[body[i].y][body[i].x] = SNAKE;

	Sleep(delay);
	return;
}

void snake::updateFood()
{
	setcolor(6, 0);
	int x, y;
	for (int i = 0;i < 5;i++) {
		//do
		{
			food[i].x = rand() % WIDTH + 1;
			food[i].y = rand() % HEIGHT + 1;
		} //while (ground[food[i].y][food[i].x] != NOTHING);
		ground[food[i].y][food[i].x] = FOOD;
		gotoxy(food[i].x, food[i].y); cout << "#";
	}
}

void snake::eraseFood()
{
	int x, y;
	for (int i = 0;i < 5;i++) {
		//ground[food[i].y][food[i].x] = FOOD;
		gotoxy(food[i].x, food[i].y); cout << " ";
	}
}

void snake::updateBlock()
{
	setcolor(3, 0);
	int x, y;
	for (int i = 0;i < 10;i++) {
		do
		{
			block[i].x = rand() % WIDTH - 6 ;
			block[i].y = rand() % HEIGHT ;
		} while (ground[block[i].y][block[i].x] != NOTHING);
		ground[block[i].y][block[i].x] = BLOCK;
		ground[block[i].y][block[i].x+1] = BLOCK;
		ground[block[i].y][block[i].x+2] = BLOCK;
		ground[block[i].y][block[i].x+3] = BLOCK;
		ground[block[i].y][block[i].x + 4] = BLOCK;
		ground[block[i].y][block[i].x + 5] = BLOCK;
		ground[block[i].y][block[i].x + 6] = BLOCK;
		ground[block[i].y+1][block[i].x] = BLOCK;
		ground[block[i].y+1][block[i].x+1] = BLOCK;
		ground[block[i].y+1][block[i].x+2] = BLOCK;
		ground[block[i].y+1][block[i].x+3] = BLOCK;
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
		//ground[food[i].y][food[i].x] = FOOD;
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

void snake::updatePoison()
{
	setcolor(4, 0);
	int x, y;
	for (int i = 0;i <5;i++) {
		//do
		{
			poison[i].x = rand() % WIDTH ;
			poison[i].y = rand() % HEIGHT;
		} //while (ground[poison[i].y][poison[i].x] != NOTHING);
		ground[poison[i].y][poison[i].x] = POISON;
		gotoxy(poison[i].x, poison[i].y);
		cout << "@";
	}
}

void snake::erasePoison()
{
	int x, y;
	for (int i = 0;i < 5;i++) {
		gotoxy(poison[i].x, poison[i].y); cout << " ";
		//[poison[i].y][poison[i].x] = NOTHING;
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
				if  (	(i == 0 && j == 0) || (i == 0 && j == WIDTH + 1) || (i == HEIGHT + 1 && j == 0) || (i == HEIGHT + 1 && j == WIDTH + 1)	)
				{	setcolor(8, 0);
					cout << "+"; 
				}   //มุมรั้ว
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

int snake::getFoodCounter()
{
	return foodCounter;
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
		//case 'P': case 'p': pause(); break;
		case 'X': case 'x': input = EXIT; break;
		}
	} while (input != EXIT && item >= 0);
	_endthread();
	return;
}

int main()
{
	//int c = _getch();
	int delay = 100;
	time_t startTime = time(NULL);
	srand(time(NULL));
	snake anaconda;
	anaconda.initGround();
	anaconda.initSnake();
	anaconda.updateFood();
	anaconda.firstDraw();
	_beginthread(userInput, 0, (void*)0);

	do
	{
		gotoxy(1, HEIGHT + 2);
		gameTime = difftime(time(NULL), startTime);
		cout << "TIME : " << gameTime;
		anaconda.updateSnake(delay);
		//if (c == 'p' || c == 'P')  { pause(); }
		if (item == FOOD) {
			anaconda.updateSnake(delay);
		}
		/*if (item == POISON) {
			delay = 5;
			if (gameTime == 5) { delay = 100; }
		}*/
		if (gameTime % 2 == 0)//เพิ่มdelay
		{
			anaconda.updateSnake(delay);
		}
		if (gameTime % 15 == 0) //เพิ่มdelay
		{
			anaconda.eraseFood();
			anaconda.updateFood();
		}
		/*if (gameTime % 30 == 0)//เพิ่มdelay
		{
			anaconda.eraseBlock();
			anaconda.updateBlock();
		}
		if (gameTime % 15 == 0)//เพิ่มdelay
		{
			anaconda.erasePoison();
			anaconda.updatePoison();
		}*/
	} while (item >= 0 && input != EXIT);

	//clearScreen(); //เพิ่มทีหลัง
	setcolor(7, 0);
	gotoxy(WIDTH / 2 - 5, HEIGHT / 2 - 2);
	cout << "GAME OVER";
	gotoxy(WIDTH / 2 - 5, HEIGHT / 2);
	cout << "score : " << anaconda.getFoodCounter()+1 << endl;
	gotoxy(WIDTH / 2, HEIGHT / 2);
	_getch();
	return 0;
}