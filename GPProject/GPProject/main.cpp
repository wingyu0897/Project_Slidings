#include <Windows.h>
#include <iostream>
#include <conio.h>
#include <list>
#include <io.h>
#include <fcntl.h>
#include "console.h"
#include "menu.h"
#include "mode.h"
#include "BranchPath.h"
#include "EffectTrail.h"
using namespace std;

#define WIDTH			30
#define HEIGHT			30

enum class MAP_TYPE
{
	ROAD = '1',
	WALL = '0',
};

int branchCount = 1;
int pathRange = 11;
int maxSpeed = 250;

MAP_TYPE map[HEIGHT][WIDTH];
POINT player{ WIDTH / 2, HEIGHT / 2 - 1 };
POINT mainPath{ WIDTH / 2, 0 };
POINT prevPath{ WIDTH / 2, 0 };
list<BranchPath> branches;
POINT secPath{ WIDTH / 2, 0 };
POINT secprevPath{ WIDTH / 2, 0 };
POINT moveDir{ 0, 0 };
bool isStraight = true;
bool isMoving = false;
int spawnRateStart = 300;
int spawnRate = 300;
int iMode = 0;
int score = 0;

clock_t t;

void Move(PPOINT newPos, int x = 0, int y = 0);

void Init();
void SetMode();
void Die();
void Update();
void Render();
void SetNextPath(PPOINT, PPOINT);
void SetNextPathSec(PPOINT, PPOINT);

int main()
{
	srand((unsigned int)time(NULL));
	CursorView(FALSE);

	branches.push_back(BranchPath(15, secPath));
	branchCount = 1;
	maxSpeed = 250;

	clock_t oldtime, curtime;
	oldtime = clock();
	t = clock();

	while (true)
	{
		Init();
		while (true)
		{
			system("cls");
			int iMenu = gameMenu();
			cout << iMenu;
			if (iMenu == 0)
			{
				system("cls");
				break;
			}
			else if (iMenu == 1)
			{
				iMode = gameMode();
			}
			else if (iMenu == 2)
			{
				system("cls");
				cout << "게임을 종료합니다." << endl;
				for (int i = 0; i < 3; i++)
				{
					cout << '\r' << 3 - i << "...";
					Sleep(1000);
				}
				return 0;
			}
		}

		SetMode();
		Render();
		Gotoxy(20, 15);
		for (int i = 0; i < 3; i++)
		{
			cout << '\r' << "\t\t\t" << 3 - i << "...";
			Sleep(1000);
		}

		while (true)
		{
			Update();
			Render();
			if (player.y > HEIGHT)
				break;
		}

		Sleep(2000);
		Die();

	}

	return 0;
}

void Init()
{
	player = { WIDTH / 2, HEIGHT / 2 - 1 };
	mainPath = { WIDTH / 2, 0 };
	prevPath = { WIDTH / 2, 0 };
	branches.clear();
	moveDir = { 0, 0 };
	isStraight = true;
	isMoving = false;
	score = 0;
	spawnRate = spawnRateStart;
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

void SetMode()
{
	branches.clear();
	if (iMode == 0)
	{
		branchCount = 1;
		maxSpeed = 250;
		pathRange = 9;
		branches.push_back(BranchPath(15, secPath));
	}
	else
	{
		branchCount = 0;
		maxSpeed = 150;
		pathRange = WIDTH + 1;
	}
}

void Die()
{
	SetColor(15, 15);
	Gotoxy(0, 0);
	for (int i = 0; i < HEIGHT; ++i)
	{
		for (int j = 0; j < WIDTH; ++j)
		{
			cout << "  ";
		}
		cout << '\n';
		Sleep(100);
	}
	SetColor(15, 0);
	Gotoxy(0, 0);
	for (int i = 0; i < HEIGHT; ++i)
	{
		for (int j = 0; j < WIDTH; ++j)
		{
			cout << "  ";
		}
		cout << '\n';
		Sleep(100);
	}

	system("cls");
	int a = _setmode(_fileno(stdout), _O_U16TEXT);
	wcout << L" ██████   █████  ███    ███ ███████      ██████  ██    ██ ███████ ██████ " << endl;
	wcout << L"██       ██   ██ ████  ████ ██          ██    ██ ██    ██ ██      ██   ██" << endl;
	wcout << L"██   ███ ███████ ██ ████ ██ █████       ██    ██ ██    ██ █████   ██████ " << endl;
	wcout << L"██    ██ ██   ██ ██  ██  ██ ██          ██    ██  ██  ██  ██      ██   ██" << endl;
	wcout << L" ██████  ██   ██ ██      ██ ███████      ██████    ████   ███████ ██   ██" << endl;
	_setmode(_fileno(stdout), a);
	cout << endl << endl << "\t\t\t" << "  Score " << score;
	int iContinue = gameOver();
	if (iContinue == 0)
	{
		system("cls");
	}
	else if (iContinue == 1)
	{
		system("cls");
		cout << "게임을 종료합니다." << endl;
		for (int i = 0; i < 3; i++)
		{
			cout << '\r' << 3 - i << "...";
			Sleep(1000);
		}
		exit(0);
	}
}

void Update()
{
	POINT newPos = player;

	if (isMoving)
	{
		Move(&newPos, moveDir.x, moveDir.y);
	}
	else
	{
		if (_kbhit())
		{
			char input = _getch();
			isMoving = true;
			switch (input)
			{
			case 72:
				moveDir = { 0, -1 };
				break;
			case 80:
				moveDir = { 0, 1 };
				break;
			case 75:
				moveDir = { -1, 0 };
				break;
			case 77:
				moveDir = { 1, 0 };
				break;
			default:
				isMoving = false;
				break;
			}
		}
	}

	player = newPos;

	if (clock() - t < spawnRate)
		return;

	score++;
	t = clock();

	MAP_TYPE line[WIDTH] = { MAP_TYPE::WALL };
	for (int i = HEIGHT - 1; i >= 1; --i)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			map[i][j] = map[i - 1][j];
		}
	}
	for (int i = 0; i < WIDTH; ++i)
	{
		map[0][i] = MAP_TYPE::WALL;
	}

	SetNextPath(&mainPath, &prevPath);

	int ran = rand() % 100 + 1;

	if (ran > 90 && branches.size() < branchCount)
	{
		BranchPath secPath{ rand() % 40 + 11 , mainPath };
		branches.push_back(secPath);
	}
	
	if (!branches.empty())
	{
		list<BranchPath>::iterator iter;
		for (iter = branches.begin(); iter != branches.end(); iter++)
		{
			SetNextPathSec(iter->GetPath(), iter->GetPrevPath());
			iter->SetLife(iter->GetLife() - 1);

			if (iter->GetLife() == 0)
			{
				iter = branches.erase(iter);
				if (iter == branches.end())
				{
					break;
				}
			}
		}
	}

	player.y++;
	spawnRate -= 1;
	spawnRate = spawnRate <= maxSpeed ? maxSpeed : spawnRate;
}

void Render()
{
	Gotoxy(0, 0);

	int a = rand() % 1000 + 1;
	for (int y = 0; y < HEIGHT; ++y)
	{
		for (int x = 0; x < WIDTH; ++x)
		{
			SetColor(0, 7);

			if (player.y == y && player.x == x)
			{
				SetColor(12, 15);
				cout << "◆";
			}
			else if (map[y][x] == MAP_TYPE::WALL)
			{
				if (a > 995)
					SetColor(15, 4);
				else
					SetColor(15, 0);
				cout << "  ";
			}
			else if (map[y][x] == MAP_TYPE::ROAD)
			{
				//SetColor(0, (rand() % 3) * 4 + 4);
				SetColor(0, 15);
				cout << "■";
			}
		}
		cout << endl;
	}
	Gotoxy(WIDTH * 2, 0);
	SetColor(15, 0);
	cout << "Score: " << score << endl;
}

void SetNextPath(PPOINT path, PPOINT prevPath)
{
	prevPath->x = path->x;
	prevPath->y = path->y;
	if (isStraight == true)
	{
		path->x = path->x;
	}
	else
	{
		int x = rand() % pathRange;
		path->x = path->x + x - (pathRange - 1) / 2;
		if (path->x < 0)
			path->x = 0;
		if (path->x >= WIDTH)
			path->x = WIDTH - 1;
	}

	for (int i = 0; i < WIDTH; ++i)
	{
		if (prevPath->x < path->x && i >= prevPath->x && i <= path->x)
		{
			map[0][i] = MAP_TYPE::ROAD;
		}
		else if (prevPath->x > path->x && i <= prevPath->x && i >= path->x)
		{
			map[0][i] = MAP_TYPE::ROAD;
		}
		else if (i == path->x)
		{
			map[0][i] = MAP_TYPE::ROAD;
		}
		else
		{
			map[0][i] = MAP_TYPE::WALL;
		}
	}

	isStraight = !isStraight;
}

void SetNextPathSec(PPOINT path, PPOINT prevPath)
{
	prevPath->x = path->x;
	prevPath->y = path->y;
	if (isStraight == false)
	{
		path->x = path->x;
	}
	else
	{
		int x;
		do
		{
			x = rand() % 11;
			x = path->x + x - 5;
		} while (abs(x - mainPath.x) < 2);
		path->x = x;
		if (path->x < 0)
			path->x = 0;
		if (path->x >= WIDTH)
			path->x = WIDTH - 1;
	}

	for (int i = 0; i < WIDTH; ++i)
	{
		if (prevPath->x < path->x && i >= prevPath->x && i <= path->x)
		{
			map[0][i] = MAP_TYPE::ROAD;
		}
		else if (prevPath->x > path->x && i <= prevPath->x && i >= path->x)
		{
			map[0][i] = MAP_TYPE::ROAD;
		}
		else if (i == path->x)
		{
			map[0][i] = MAP_TYPE::ROAD;
		}
	}
}

void Move(PPOINT newPos, int x, int y)
{
	newPos->x += x;
	newPos->y += y;

	if (newPos->x < 0 || newPos->y < 0 || newPos->x >= WIDTH || newPos->y >= HEIGHT || map[newPos->y][newPos->x] == MAP_TYPE::WALL)
	{
		newPos->x -= x;
		newPos->y -= y;
		isMoving = false;
	}

	if (newPos->y - 1 >= 0)
	{
		if (y == 0 && map[newPos->y - 1][newPos->x] == MAP_TYPE::ROAD)
		{
			isMoving = false;
		}
	}

	if (newPos->y + 1 <= HEIGHT)
	{
		if (y == 0 && map[newPos->y + 1][newPos->x] == MAP_TYPE::ROAD)
		{
			isMoving = false;
		}
	}

	if (newPos->x - 1 >= 0)
	{
		if (x == 0 && map[newPos->y][newPos->x - 1] == MAP_TYPE::ROAD)
		{
			isMoving = false;
		}
	}

	if (newPos->x + 1 <= WIDTH)
	{
		if (x == 0 && map[newPos->y][newPos->x + 1] == MAP_TYPE::ROAD)
		{
			isMoving = false;
		}
	}

	return;
}