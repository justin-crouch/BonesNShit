#ifndef DOG_H
#define DOG_H

#include "raylib.h"
#include "raymath.h"
#include "config.h"

namespace Dog
{
	void SetPos(Vector2 pos);
	Vector2 GetPos();
	
	void SetHealth(int val);
	int GetHealth();

	void SetScore(int val);
	int GetScore();

	Vector2 GetSize();

	void Update(int input);
	void Draw();
	void Reset();
}

#endif
