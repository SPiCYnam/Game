#include <iostream>
#include<windows.h>
#include<conio.h>
#include<time.h>
using namespace std;
#define scount 10
COORD food[scount];
COORD block[scount];
COORD poison[scount];
void gotoxy(int x, int y);
void draw_snake(int x, int y);
void erase_snake(int x, int y);
void setcursor(bool visible);
void setcolor(int fg, int bg);
void pause();
void draw_wall();
void snakePlus();
void randFood();
void randBlock();
void randPoison();


int i, j,x,y;
int gameTime ;

int main()
{
	time_t startTime = time(NULL);
	setcursor(0);
	draw_wall();
	randFood();
	randBlock();
	randPoison();
	x = rand() % 80;
	y = rand() % 30;
	draw_snake(x, y);
	char ch = ' ';
	int movement = 0;
	gotoxy(x, y);
	
	do
	{
		setcolor(3,0);
		gotoxy(0,0);
		gameTime = difftime(time(NULL), startTime);
		cout <<"TIME : "<<gameTime ; 
		if (_kbhit())
		{
			ch = _getch();
			if (ch == 'a') { movement = 1; }
			if (ch == 'd') { movement = 2; }
			if (ch == 'w' ) { movement = 3; }
			if (ch == 's' ) { movement = 4; }
			if (ch == 'p') { movement = 0; }
			fflush(stdin);
		}
		if (movement != 0)
		{
			if (movement == 1 && x > 0) { erase_snake(x, y); draw_snake(--x, y); }//a
			if (movement == 2 && x < 80) { erase_snake(x, y); draw_snake(++x, y); }//d
			if (movement == 3 && y >= 2) { erase_snake(x, y); draw_snake(x, --y); }//w
			if (movement == 4 && y < 30) { erase_snake(x, y); draw_snake(x, ++y); }//s
		}
		if (movement == 0)
		{
			pause();
		}
		snakePlus();
		Sleep(100);
		if (x == 80 || x == 0 || y == 1 || y == 30)
		{
			return 0;
		}
	} while (ch != 'x');
	return 0;
}
void randBlock()
{
	if (gameTime % 10 == 0)
	{
		for (int i = 0;i < 5;i++)
		{
			block[i].X = rand() % 80;
			block[i].Y = rand() % 30;
			gotoxy(block[i].X, block[i].Y);
			cout << "X";
			gotoxy(block[i].X + 1, block[i].Y);
			cout << "X";
			gotoxy(block[i].X + 2, block[i].Y);
			cout << "X";
			gotoxy(block[i].X, block[i].Y + 1);
			cout << "X";
			gotoxy(block[i].X + 1, block[i].Y + 1);
			cout << "X";
			gotoxy(block[i].X + 2, block[i].Y + 1);
			cout << "X";
		}
	}
	else 
	{
			gotoxy(block[i].X, block[i].Y);
			cout << " ";
			gotoxy(block[i].X + 1, block[i].Y);
			cout << " ";
			gotoxy(block[i].X + 2, block[i].Y);
			cout << " ";
			gotoxy(block[i].X, block[i].Y + 1);
			cout << " ";
			gotoxy(block[i].X + 1, block[i].Y + 1);
			cout << " ";
			gotoxy(block[i].X + 2, block[i].Y + 1);
			cout << " ";
	}
}
void randPoison()
{
	if (gameTime % 15 == 0)
	{
		for (int i = 0;i < 5;i++)
		{
			poison[i].X = rand() % 80;
			poison[i].Y = rand() % 30;
			gotoxy(poison[i].X, poison[i].Y);
			cout << "+";
		}
	}
}
void randFood()
{
	if (gameTime % 10 == 0)
	{
		for (int i = 0;i < 10;i++)
		{
			food[i].X = rand() % 80;
			food[i].Y = rand() % 30;
			gotoxy(food[i].X, food[i].Y);
			cout << "*";
		}
	}
}
void snakePlus() //ทำใหม่
{
	if (gameTime % 5 == 0)
	{
		gotoxy(x, y);
		setcolor(3, 0);
		cout << "o";
		//setcolor(0, 0);
	}
}
void pause() {
		while (1) {
				for (int i = 0; i < 255; i++) {
						if (GetAsyncKeyState(i))
							return;
				}
		}
}
void draw_wall()
{
	for (int i = 0;i <= 81;i++) //top wall
	{
		for (int j = 1;j <= 1;j++)
		{
			gotoxy(i, j);
			cout << "_";
		}
	}
	for (int i = 0;i <= 81;i++) //bottom wall
	{
		for (int j = 30;j <= 30;j++)
		{
			gotoxy(i, j);
			cout << "_";
		}
	}
	for (int i = 0;i <= 0;i++) //left wall
	{
		for (int j = 0;j <= 30;j++)
		{
			gotoxy(i, j);
			cout << "|\n";
		}
	}
	for (int i = 81;i <= 81;i++) //right wall
	{
		for (int j = 0;j <= 30;j++)
		{
			gotoxy(i, j);
			cout << "|\n";
		}
	}
}
void draw_snake(int x, int y)
{
	setcolor(2, 0);
	gotoxy(x, y);
	cout<<"o";
	setcolor(0, 0);
}
void gotoxy(int x, int y)
{
	COORD c = { x, y };
	SetConsoleCursorPosition(
		GetStdHandle(STD_OUTPUT_HANDLE), c);
}
void erase_snake(int x, int y)
{
	gotoxy(x, y);
	cout<<" ";
}
void setcursor(bool visible)
{
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO lpCursor;
	lpCursor.bVisible = visible;
	lpCursor.dwSize = 20;
	SetConsoleCursorInfo(console, &lpCursor);
}
void setcolor(int fg, int bg)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, bg * 16 + fg);
}