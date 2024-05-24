#ifndef DOG_H
#define DOG_H

#include "raylib.h"
#include "raymath.h"
#include "config.h"

namespace Dog
{
	typedef enum{
		NEUTRAL,
		BONE,
		DISGUST,
		YAWN,

		length,
	} States;

	void SetPos(Vector2 pos);
	Vector2 GetPos();
	
	void SetHealth(int val);
	int GetHealth();

	void SetScore(int val);
	int GetScore();

	Vector2 GetSize();

	void SetTexture(States state, Texture2D tex, float scale=0);
	Texture2D GetTexture(States state);

	void SetScale(States state, float s);
	float GetScale(States state);

	void SetFacePos(States state, Vector2 pos);
	Vector2 GetFacePos(States state);

	void SetFaceState(States state, float time=-1.0f);
	States GetFaceState();

	void Update(int input);
	void Draw();
	void Reset();
}

#endif
