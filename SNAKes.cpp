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
void eraseFood();
void randBlock();
void eraseBlock();
void randPoison();
void erasePoison();

int i,j,x,y;
int gameTime,score;
int m=1;
bool gameOver;

int main()
{
	//rand() = time(NULL);
	time_t startTime = time(NULL);
	setcursor(0);
	draw_wall();
	randFood();
	randPoison();
	x = rand() % 80;
	y = rand() % 30;
	gotoxy(x, y);
	draw_snake(x, y);
	char ch = ' ';
	int movement = 0;
	do
	{
		setcolor(7,0);
		gotoxy(1,31);
		gameTime = difftime(time(NULL), startTime);
		cout <<"TIME : "<<gameTime ; 
		if (_kbhit())
		{
			ch = _getch();
			if (ch == 'a') { movement = 1; }
			else if (ch == 'd') { movement = 2; }
			else if (ch == 'w' ) { movement = 3; }
			else if (ch == 's' ) { movement = 4; }
			if (ch == 'p') { movement = 0; }
			fflush(stdin);
		}
		if (movement != 0)
		{
			if (movement == 1 && x > 0) { erase_snake(x, y); draw_snake(--x, y);  }//a
			else if (movement == 2 && x < 80) { erase_snake(x, y); draw_snake(++x, y); }//d
			else if (movement == 3 && y >= 2) { erase_snake(x, y); draw_snake(x, --y); }//w
			else if (movement == 4 && y < 30) { erase_snake(x, y); draw_snake(x, ++y); }//s
		}
		if (movement == 0) { pause(); }
		if (gameTime % 5 == 0)
		{
			snakePlus();
		}
		if (gameTime % 10 == 0)
		{
				eraseFood();
				randFood();
				//randBlock();
				//eraseBlock();
		}
		if (gameTime % 15 == 0)
		{
			erasePoison();
			randPoison();
		}
		Sleep(50);
		if (x == 80 || x == 0 || y == 1 || y == 30)
		{
			return 0;
		}
	} while (ch != 'x');
	return 0;
}
/*bool gameOver()
{
	cout << "scores = " << score;
}*/
void randBlock() //สุ่มใหม่ไม่ลบอันเดิม
{
		for (int i = 0;i < 10;i++)
		{
			setcolor(7, 8);
			block[i].X = rand() % 78;
			block[i].Y = rand() % 30;
			gotoxy(block[i].X, block[i].Y); cout << "X";
			gotoxy(block[i].X + 1, block[i].Y); cout << "X";
			gotoxy(block[i].X + 2, block[i].Y); cout << "X";
			gotoxy(block[i].X + 3, block[i].Y); cout << "X";
			gotoxy(block[i].X, block[i].Y + 1); cout << "X";
			gotoxy(block[i].X + 1, block[i].Y + 1); cout << "X";
			gotoxy(block[i].X + 2, block[i].Y + 1); cout << "X";
			gotoxy(block[i].X + 3, block[i].Y + 1); cout << "X";
		}
}
void eraseBlock()
{
	for (int i = 0;i < 10;i++)
	{
			setcolor(0, 0);
			gotoxy(block[i].X, block[i].Y); cout << " ";
			gotoxy(block[i].X + 1, block[i].Y); cout << " ";
			gotoxy(block[i].X + 2, block[i].Y); cout << " ";
			gotoxy(block[i].X + 3, block[i].Y); cout << " ";
			gotoxy(block[i].X, block[i].Y + 1); cout << " ";
			gotoxy(block[i].X + 1, block[i].Y + 1); cout << " ";
			gotoxy(block[i].X + 2, block[i].Y + 1); cout << " ";
			gotoxy(block[i].X + 3, block[i].Y + 1); cout << " ";
	}
}
void randPoison() //สุ่มใหม่ไม่ลบอันเดิม
{
		for (int i = 0;i < 5;i++)
		{
			poison[i].X = rand() % 80;
			poison[i].Y = rand() % 30;
			gotoxy(poison[i].X, poison[i].Y);
			setcolor(4, 0);
			cout << "+";
		}
}
void erasePoison()
{
	for (int i = 0;i < 5;i++)
	{
		gotoxy(poison[i].X, poison[i].Y);
		setcolor(0, 0);
		cout << " ";
	}
}
void randFood() //สุ่มใหม่ไม่ลบอันเดิม
{
	for (int i = 0;i < 10;i++)
	{
		food[i].X = rand() % 80;
		food[i].Y = rand() % 30;
		gotoxy(food[i].X, food[i].Y);
		setcolor(6, 0);
		cout << "*";
	}
}
void eraseFood()
{
	for (int i = 0;i < 10;i++)
	{
		gotoxy(food[i].X, food[i].Y);
		setcolor(0, 0);
		cout << " ";
	}
}
void snakePlus() //หางไม่ขยับตาม
{
		gotoxy(x+m, y);
		setcolor(2, 0);
		cout << "o";
		setcolor(0, 0);
		m++;
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
		for (int j = 0;j <= 0;j++)
		{
			gotoxy(i, j);
			setcolor(8, 0);
			cout << "_";
		}
	}
	for (int i = 0;i <= 81;i++) //bottom wall
	{
		for (int j = 30;j <= 30;j++)
		{
			gotoxy(i, j);
			setcolor(8, 0);
			cout << "_";
		}
	}
	for (int i = 0;i <= 0;i++) //left wall
	{
		for (int j = 0;j <= 30;j++)
		{
			gotoxy(i, j);
			setcolor(8, 0);
			cout << "|\n";
		}
	}
	for (int i = 81;i <= 81;i++) //right wall
	{
		for (int j = 0;j <= 30;j++)
		{
			gotoxy(i, j);
			setcolor(8, 0);
			cout << "|\n";
		}
	}
}
void draw_snake(int x, int y)
{
	setcolor(2, 7);
	gotoxy(x, y);
	cout<<"o";
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
	setcolor(0, 0);
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