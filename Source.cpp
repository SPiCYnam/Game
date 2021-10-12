#include<stdio.h>
#include<windows.h>
#include<conio.h>
#include<time.h>
using namespace std;

void gotoxy(int x, int y);
void draw_ship(int x, int y);
void erase_ship(int x, int y);
void setcursor(bool visible);
void setcolor(int fg, int bg);
int x = 38, y = 20, i = 1;
int time1, time2,playTime;
int main()
{
	time_t time1= time(NULL);
	time_t time2 = time(NULL);
	playTime = time2 - time1;
	printf("time=%d", playTime);
	setcursor(0);
	char ch = ' ';
	int movement = 0;
	gotoxy(x, y);
	draw_ship(x, y);
	do
	{
		if (_kbhit())
		{
			ch = _getch();
			if (ch == 'a') { movement = 1; }
			if (ch == 'd') { movement = 2; }
			if (ch == 'w' ) { movement = 3; }
			if (ch == 's' ) { movement = 4; }
			fflush(stdin);
		}
		if (movement != 0)
		{
			if (movement == 1 && x >= 1) { erase_ship(x, y); draw_ship(--x, y); }//a
			if (movement == 2 && x < 80) { erase_ship(x, y); draw_ship(++x, y); }//d
			if (movement == 3 && y >= 1) { erase_ship(x, y); draw_ship(x, --y); }//w
			if (movement == 4 && y < 29) { erase_ship(x, y); draw_ship(x, ++y); }//s
		}
		Sleep(50);
	} while (ch != 'x');
	return 0;
}
void draw_ship(int x, int y)
{
	setcolor(2, 0);
	gotoxy(x, y);
	printf(" 0 ");
	setcolor(0, 0);
}
void gotoxy(int x, int y)
{
	COORD c = { x, y };
	SetConsoleCursorPosition(
		GetStdHandle(STD_OUTPUT_HANDLE), c);
}
void erase_ship(int x, int y)
{
	gotoxy(x, y);
	printf("       ");
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