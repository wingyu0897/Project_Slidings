#pragma once
#include <Windows.h>

class EffectTrail
{
public:
	EffectTrail(int, POINT);
	~EffectTrail();

public:
	int GetLife() { return life; }
	void SetLife(int input) { life = input; }
	PPOINT GetPosition() { return &position; }
	void SetPosition(POINT point) { position = point; }

private:
	POINT position;
	int life;
};

