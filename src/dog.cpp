#include "dog.h"
using namespace Dog;

// Stats and attribs of dog (main character)
Vector2 dog_vel = Vector2Zero();
Vector2 dog_pos = Vector2Zero();
Vector2 real_pos = Vector2Zero();
Vector2 dog_size = (Vector2){ 0.09f, 0.06f };
int health = DOG_MAX_HEALTH;
int score = 0;

// Getter/setter for pos, health, score, and size
void Dog::SetPos(Vector2 pos) {
	dog_pos = pos;

	real_pos.x = (float)GetScreenWidth() * pos.x;
	real_pos.y = pos.y;
}
Vector2 Dog::GetPos() {return real_pos;}

void Dog::SetHealth(int val) {health = val;}
int Dog::GetHealth() {return health;}

void Dog::SetScore(int val) {score = val;}
int Dog::GetScore() {return score;}

Vector2 Dog::GetSize() {return (Vector2){GetScreenWidth()*dog_size.x, GetScreenHeight()*dog_size.y};}

// Move dog based on given input
// Expects -1(left), 0(no movement), 1(right)
void Dog::Update(int input)
{
	// dog_vel = (Vector2){ input * DOG_MAX_SPD * GetFrameTime(), 0 };
	dog_vel = (Vector2){ input * DOG_MAX_SPD * GetFrameTime(), 0 };

	// Vector2 newpos = Vector2Add( dog_pos, dog_vel );
	Vector2 newpos;
	newpos.x = Clamp(dog_pos.x + dog_vel.x, 0.1f, 0.9f - GetSize().x/GetScreenWidth());
	newpos.y = (float)GetScreenHeight() - GetSize().y - 10;

    Dog::SetPos(newpos);
}

void Dog::Draw()
{
	DrawRectangleV( real_pos, GetSize(), (Color){0, 255, 0, 255} );
}

// Reset stats of dog
void Dog::Reset()
{
	health = DOG_MAX_HEALTH;
	score = 0;
}