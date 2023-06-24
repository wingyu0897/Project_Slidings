#pragma once
#include<Windows.h>
enum class COLOR
{
	BLACK, BLUE, GREEN, SKYBLUE, RED,
	VIOLET, YELLOW, LIGHT_GRAY, GRAY,
	LIGHT_BLUE, LIGHT_GREEN,
	MINT, LIGHT_RED, LIGHT_VIOLET,
	LIGHT_YELLOW, WHITE
};
void FullScreen();
BOOL Gotoxy(int x, int y);
void SetCursor(bool, DWORD);
void SetColor(int, int);
int GetColor();
int GetbgColor();
void CursorView(BOOL);