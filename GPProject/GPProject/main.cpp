#include <Windows.h>
#include <iostream>
using namespace std;

#define WIDTH	40
#define HEIGHT	40

enum class MAP_TYPE
{
	ROAD = '1',
	WALL = '0',
};

void CursorView()
{
	CONSOLE_CURSOR_INFO cursorInfo = { 0, };
	cursorInfo.dwSize = 1; //Ä¿¼­ ±½±â (1 ~ 100)
	cursorInfo.bVisible = FALSE; //Ä¿¼­ Visible TRUE(º¸ÀÓ) FALSE(¼û±è)
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

void gotoxy(int x, int y)
{
	COORD Cur;
	Cur.X = x;
	Cur.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Cur);
}

MAP_TYPE map[HEIGHT][WIDTH];
POINT player{ WIDTH / 2, HEIGHT / 2 - 1 };
POINT mainPath{ WIDTH / 2, 0 };
POINT prevPath{ WIDTH / 2, 0 };
bool a = false;

clock_t t;

void Init();
void Update();
void Render();
void SetNextPath();

int main()
{
	srand((unsigned int)time(NULL));
	CursorView();

	clock_t oldtime, curtime;
	oldtime = clock();
	t = clock();

	Init();

	while (true)
	{
		Update();
		Render();
		while (true)
		{
			curtime = clock();
			if (curtime - oldtime >= 33)
			{
				oldtime = curtime;
				break;
			}
		}
	}

	return 0;
}

void Init()
{
	for (int i = HEIGHT - 1; i >= HEIGHT / 2; --i)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			map[i][j] = MAP_TYPE::WALL;
		}
	}
	for (int i = HEIGHT / 2 - 1; i >= 0; --i)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			if (j == WIDTH / 2)
				map[i][j] = MAP_TYPE::ROAD;
			else
				map[i][j] = MAP_TYPE::WALL;
		}
	}
}

void Update()
{
	POINT newPos = player;

	if (GetAsyncKeyState(VK_UP) & 0x8000)
		newPos.y--;
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
		newPos.y++;
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		newPos.x--;
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		newPos.x++;

	newPos.x = newPos.x < 0 ? 0 : newPos.x >= WIDTH ? WIDTH - 1 : newPos.x;
	newPos.y = newPos.y < 0 ? 0 : newPos.y >= HEIGHT ? HEIGHT - 1 : newPos.y;

	if (map[newPos.y][newPos.x] != MAP_TYPE::WALL)
	{
		player = newPos;
	}

	if (clock() - t < 10)
		return;

	t = clock();

	MAP_TYPE line[WIDTH] = { MAP_TYPE::WALL };
	for (int i = HEIGHT - 1; i >= 1; --i)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			map[i][j] = map[i - 1][j];
		}
	}
	SetNextPath();
	for (int i = 0; i < WIDTH; ++i)
	{
		if (prevPath.x < mainPath.x && i >= prevPath.x && i <= mainPath.x)
		{
			map[0][i] = MAP_TYPE::ROAD;
		}
		else if (prevPath.x > mainPath.x && i <= prevPath.x && i >= mainPath.x)
		{
			map[0][i] = MAP_TYPE::ROAD;
		}
		else if (i == mainPath.x)
		{
			map[0][i] = MAP_TYPE::ROAD;
		}
		else
		{
			map[0][i] = MAP_TYPE::WALL;
		}
	}
	player.y++;
}

void Render()
{
	gotoxy(0, 0);

	for (int y = 0; y < HEIGHT; ++y)
	{
		for (int x = 0; x < WIDTH; ++x)
		{
			if (player.y == y && player.x == x)
				cout << "£À";
			else if (map[y][x] == MAP_TYPE::WALL)
				cout << "¡á";
			else if (map[y][x] == MAP_TYPE::ROAD)
				cout << "  ";

		}
		cout << endl;
	}
}

void SetNextPath()
{
	prevPath = mainPath;
	if (a == false)
	{
		mainPath.x = mainPath.x;
	}
	else
	{
		mainPath.x = rand() % WIDTH;
	}
	a = !a;
}