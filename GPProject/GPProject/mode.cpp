#include <iostream>
#include <conio.h>
#include "console.h"
#include "mode.h"
using namespace std;

int curMode = 0;

int gameMode()
{
	system("cls");
	int x = 25;
	int y = 10;
	Gotoxy(x, y);
	cout << (curMode == 0 ? "브랜치 모드" : "스피드 모드") << endl;
	Gotoxy(x, y + 1);
	cout << "뒤로 가기" << endl;

	Gotoxy(x - 2, y);
	cout << " ";
	Gotoxy(x - 2, y);
	cout << ">";

	while (true)
	{
		int iKey = keyCont();
		switch (iKey)
		{
		case VK_UP:
		{
			if (y > 10)
			{
				Gotoxy(x - 2, y);
				cout << " ";
				Gotoxy(x - 2, --y);
				cout << ">";
			}
		}
		break;
		case VK_DOWN:
		{
			if (y < 11)
			{
				Gotoxy(x - 2, y);
				cout << " ";
				Gotoxy(x - 2, ++y);
				cout << ">";
			}
		}
		break;
		case VK_SPACE:
		{
			if (y - 10 == 1)
			{
				return curMode;
			}
			Gotoxy(x, y);
			curMode = curMode == 0 ? 1 : 0;
			cout << (curMode == 0 ? "브랜치 모드" : "스피드 모드") << endl;
		}
		break;
		}

		Sleep(100);
	}

	return -1;
}

int keyCont()
{
	int iKey = _getch();
	if (iKey == 224)
	{
		iKey = _getch();
		switch (iKey)
		{
		case 72:
			return VK_UP;
			break;
		case 80:
			return VK_DOWN;
			break;
		}
	}
	else if (iKey == 32)
		return VK_SPACE;
	Sleep(200);
	return -1;
}
