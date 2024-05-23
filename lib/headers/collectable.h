#ifndef COLLECTABLE_H
#define COLLECTABLE_H

#include "raylib.h"
#include "raymath.h"
#include "config.h"

#include <bitset>
#include <functional>

namespace Collectable
{
	enum Type {
		NORMAL,
		FROZEN,
		GOLDEN,
		SHIT,

		NONE,
		length
	};

	void Init();

	int GetVals(int type);

	void SetCallable(Type t, std::function<void()> func);

	void Add(int bound, Type t);
	void Remove(int idx);

	void Update(Vector2 bounds, Vector2 dog_pos, Vector2 dog_size);
	void Draw();
	void Reset();
}

#endif
