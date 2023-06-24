#pragma once
#include <Windows.h>

enum class MAP_TYPE;
class BranchPath
{
public:
	BranchPath(int life, POINT pathStart);
	~BranchPath();

public:
	int		GetLife() { return life; }
	void	SetLife(int v) { life = v; }
	PPOINT	GetPath() { return &path; }
	PPOINT	GetPrevPath() { return &prevPath; }

private:
	int		life;
	POINT	path;
	POINT	prevPath;
};

