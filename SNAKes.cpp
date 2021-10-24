#include <iostream>
#include<windows.h>
#include<conio.h>
#include<time.h>
using namespace std;
void gotoxy(int x, int y);
void draw_snake(int x, int y);
void erase_snake(int x, int y);
void setcursor(bool visible);
void setcolor(int fg, int bg);
void pause();
void draw_wall();
void snakePlus();

int x = 38, y = 20;
int time1, time2,playTime;
int i, j;
int gameTime ;
int main()
{
	time_t startTime = time(NULL);
	setcursor(0);
	draw_wall();
	char ch = ' ';
	int movement = 0;
	gotoxy(x, y);
	draw_snake(x, y);
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
			if (movement == 1 && x >= 1) { erase_snake(x, y); draw_snake(--x, y); }//a
			if (movement == 2 && x < 80) { erase_snake(x, y); draw_snake(++x, y); }//d
			if (movement == 3 && y >= 1) { erase_snake(x, y); draw_snake(x, --y); }//w
			if (movement == 4 && y < 30) { erase_snake(x, y); draw_snake(x, ++y); }//s
		}
		else { pause(); }
		snakePlus();
		Sleep(50);
	} while (ch != 'x');
	return 0;
}
void snakePlus() //ทำใหม่
{
	int timeSpend,timeX,timeY;
	timeSpend = gameTime%5;
	if (timeSpend = 0)
	{
		timeX = x;
		timeY = y + 1;
		gotoxy(timeX, timeY);
		setcolor(3, 0);
		cout << "o";
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
	for (int i = 0;i <= 80;i++)
	{
		for (int j = 0;j <= 2;j++)
		{
			gotoxy(i, j);
			cout << "_";
		}
	}
	for (int i = 0;i <= 80;i++)
	{
		for (int j = 30;j <= 30;j++)
		{
			gotoxy(i, j);
			cout << "_";
		}
	}
	for (int i = 0;i <= 0;i++)
	{
		for (int j = 0;j <= 30;j++)
		{
			gotoxy(i, j);
			cout << "|\n";
		}
	}
	for (int i = 80;i <= 80;i++)
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
	cout<<"   ";
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