#include <iostream>
#include <conio.h>
#include <io.h>
#include <fcntl.h>
#include "console.h"
#include "menu.h"
using namespace std;

int gameMenu()
{
	int a = _setmode(_fileno(stdout), _O_U16TEXT);
	wcout << L"███████ ██      ██ ██████  ██ ███    ██  ██████  ███████ ██" << endl;
	wcout << L"██      ██      ██ ██   ██ ██ ████   ██ ██       ██      ██" << endl;
	wcout << L"███████ ██      ██ ██   ██ ██ ██ ██  ██ ██   ███ ███████ ██" << endl;
	wcout << L"     ██ ██      ██ ██   ██ ██ ██  ██ ██ ██    ██      ██	  " << endl;
	wcout << L"███████ ███████ ██ ██████  ██ ██   ████  ██████  ███████ ██" << endl;
	_setmode(_fileno(stdout), a);

	int x = 25;
	int y = 10;
	Gotoxy(x, y);
	cout << "게임 시작" << endl;
	Gotoxy(x, y + 1);
	cout << "게임 모드" << endl;
	Gotoxy(x, y + 2);
	cout << "종료 하기" << endl;

	Gotoxy(x - 2, y);
	cout << " ";
	Gotoxy(x - 2, y);
	cout << ">";

	while (true)
	{
		int iKey = keyController();
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
			if (y < 12)
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
			return y - 10;
		}
		break;
		}

		Sleep(100);
	}

	return -1;
}

int keyController()
{
	/*if (GetAsyncKeyState(VK_UP) & 0x8000)
		return (int)KEY::UP;
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
		return (int)KEY::DOWN;
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
		return (int)KEY::SPACE;*/
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

int gameOver()
{
	int x = 25;
	int y = 10;
	Gotoxy(x, y);
	cout << "계속 하기" << endl;
	Gotoxy(x, y + 1);
	cout << "종료 하기" << endl;

	Gotoxy(x - 2, y);
	cout << " ";
	Gotoxy(x - 2, y);
	cout << ">";

	while (true)
	{
		int iKey = keyController();
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
			return y - 10;
		}
		break;
		}

		Sleep(100);
	}
}
