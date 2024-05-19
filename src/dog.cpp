#include "dog.h"
using namespace Dog;

// Stats and attribs of dog (main character)
Vector2 dog_vel = Vector2Zero();
Vector2 dog_pos = Vector2Zero();
Vector2 dog_size = (Vector2){ 100.0f, 50.0f };
int health = 3;
int score = 0;

// Getter/setter for pos, health, score, and size
void Dog::SetPos(Vector2 pos) {dog_pos = pos;}
Vector2 Dog::GetPos() {return dog_pos;}

void Dog::SetHealth(int val) {health = val;}
int Dog::GetHealth() {return health;}

void Dog::SetScore(int val) {score = val;}
int Dog::GetScore() {return score;}

Vector2 Dog::GetSize() {return dog_size;}

// Move dog based on given input
// Expects -1(left), 0(no movement), 1(right)
void Dog::Update(int input)
{
	dog_vel = (Vector2){ input * DOG_MAX_SPD * GetFrameTime(), 0 };
    Dog::SetPos(Vector2Add( dog_pos, dog_vel ));
}

void Dog::Draw()
{
	DrawRectangleV( dog_pos, dog_size, (Color){0, 255, 0, 255} );
}

// Reset stats of dog
void Dog::Reset()
{
	health = 3;
	score = 0;
}