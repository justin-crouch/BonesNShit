#include "dog.h"
using namespace Dog;

#define MAX_SCALE_COUNT	0.07f
#define MULT_SPEED		0.6f

// Stats and attribs of dog (main character)
Vector2 dog_vel = Vector2Zero();
Vector2 dog_pos = Vector2Zero();
Vector2 real_pos = Vector2Zero();
Vector2 dog_size = (Vector2){ 0.09f, 0.06f };
int health = DOG_MAX_HEALTH;
int score = 0;

Dog::States face_state = Dog::States::NEUTRAL;
float anim_counter = 0.0f;
float face_counter = 0.0f;
float mult = -MULT_SPEED;

Texture2D textures[Dog::States::length];
float scales[Dog::States::length];
float scales_cp[Dog::States::length];
Vector2 face_positions[Dog::States::length];

Texture2D bowl;

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

void Dog::LoadBowl() {bowl = LoadTexture("assets/bowl.png");}
void Dog::SetTexture(States state, Texture2D tex, float scale)
{
	textures[state] = tex;
	scales[state] = (scale > 0)? scale : scales[state];
}
Texture2D Dog::GetTexture(States state) {return textures[state];}

void Dog::SetScale(States state, float s) {scales[state] = s; scales_cp[state] = s;}
float Dog::GetScale(States state) {return scales[state];}

void Dog::SetFacePos(States state, Vector2 pos) {face_positions[state] = pos;}
Vector2 Dog::GetFacePos(States state) {return face_positions[state];}

void Dog::SetFaceState(States state, float time)
{
	face_state = state;
	anim_counter = 0.0f;
	mult = MULT_SPEED;

	if(time > 0.0f) face_counter = -time;
	else face_counter = 0.0f;
}
Dog::States Dog::GetFaceState() {return face_state;}

// Move dog based on given input
// Expects -1(left), 0(no movement), 1(right)
void Dog::Update(int input)
{
	// dog_vel = (Vector2){ input * DOG_MAX_SPD * GetFrameTime(), 0 };
	dog_vel = (Vector2){ input * DOG_MAX_SPD * GetFrameTime(), 0 };

	// Vector2 newpos = Vector2Add( dog_pos, dog_vel );
	Vector2 newpos;
	newpos.x = Clamp(dog_pos.x + dog_vel.x, 0.0f, 0.85f - GetSize().x/GetScreenWidth());
	newpos.y = (float)GetScreenHeight() - GetSize().y - 10;

    Dog::SetPos(newpos);

    anim_counter += GetFrameTime() * mult;
    if(anim_counter > MAX_SCALE_COUNT) {anim_counter = MAX_SCALE_COUNT; mult = -MULT_SPEED;}
    if(anim_counter < 0.0f) anim_counter = 0.0f;
    scales[face_state] = anim_counter + scales_cp[face_state];

    if(face_counter < 0.0f) face_counter += GetFrameTime();
    if(face_counter > 0.0f)
    {
    	face_counter = 0.0f;
    	SetFaceState(States::NEUTRAL);
    }
}

void Dog::Draw()
{
	float scale = 0.4f;
	switch(GetScreenWidth())
	{
	case 207:
		scale -= 0.22f;
		break;
	case 360:
		scale -= 0.20f;
		break;
	case 368:
		scale -= 0.18f;
		break;
	case 512:
		scale -= 0.16f;
		break;
	case 640:
		scale -= 0.14f;
		break;
	case 768:
		scale -= 0.12f;
		break;
	case 896:
		scale -= 0.10f;
		break;
	case 1024:
		scale -= 0.08f;
		break;
	case 1152:
		scale -= 0.02f;
		break;
	case 1280:
		scale -= 0.0f;
		break;
	default: break; 
	}
	Vector2 pos = Vector2Subtract( real_pos, (Vector2){bowl.width * scale * 0.17f, bowl.height * scale * 0.17f} );
	DrawTextureEx(bowl, pos, 0.0f, scale, (Color){255,255,255,255});
	// DrawRectangleV( real_pos, GetSize(), (Color){0, 255, 0, 255} );
}

// Reset stats of dog
void Dog::Reset()
{
	health = DOG_MAX_HEALTH;
	score = 0;
	face_state = Dog::States::NEUTRAL;
}