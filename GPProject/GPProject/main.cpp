#include <Windows.h>
#include <iostream>
#include <conio.h>
using namespace std;

#define WIDTH	25
#define HEIGHT	25

enum class MAP_TYPE
{
	ROAD = '1',
	WALL = '0',
};

void SetColor(int color, int bgcolor)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (bgcolor << 4) | color);
}

void CursorView()
{
	CONSOLE_CURSOR_INFO cursorInfo = { 0, };
	cursorInfo.dwSize = 50; //Ä¿¼­ ±½±â (1 ~ 100)
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
int spawnRate = 250;

clock_t t;

void Move(PPOINT newPos, int x = 0, int y = 0);

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

	if (_kbhit())
	{
		char input = _getch();

		switch (input)
		{
		case 72:
			Move(&newPos, 0, -1);
			break;
		case 80:
			Move(&newPos, 0, 1);
			break;
		case 75:
			Move(&newPos, -1, 0);
			break;
		case 77:
			Move(&newPos, 1, 0);
			break;
		}
	}

	//if (GetAsyncKeyState(VK_UP) & 0x8000)
	//{
	//	Move(&newPos, 0, -1);
	//}
	//if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	//{
	//	Move(&newPos, 0, 1);
	//}
	//if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	//{
	//	Move(&newPos, -1, 0);
	//}
	//if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	//{
	//	Move(&newPos, 1, 0);
	//}

	player = newPos;

	if (clock() - t < spawnRate)
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
	spawnRate -= 1;
	spawnRate = spawnRate <= 150 ? 150 : spawnRate;
}

void Render()
{
	gotoxy(0, 0);

	for (int y = 0; y < HEIGHT; ++y)
	{
		for (int x = 0; x < WIDTH; ++x)
		{
			SetColor(0, 7);

			if (player.y == y && player.x == x)
			{
				SetColor(15, 0);
				cout << "¡ß";
			}
			else if (map[y][x] == MAP_TYPE::WALL)
			{
				SetColor(15, 0);
				cout << "  ";
			}
			else if (map[y][x] == MAP_TYPE::ROAD)
			{
				SetColor(0, (rand() % 5) * 4);
				cout << "¡á";
			}

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

void Move(PPOINT newPos, int x, int y)
{
	while (true)
	{
		newPos->x += x;
		newPos->y += y;

		if (newPos->x < 0 || newPos->y < 0 || newPos->x >= WIDTH || newPos->y >= HEIGHT || map[newPos->y][newPos->x] == MAP_TYPE::WALL)
		{
			newPos->x -= x;
			newPos->y -= y;
			break;
		}
	}

	return;
}