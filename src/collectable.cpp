#include "collectable.h"
using namespace Collectable;

#define ROT_SPEED	1.0f

Vector2 size_perc = (Vector2){ 0.05f, 0.03f };

std::bitset<MAX_COLLECTABLES> 			active;
Vector2 								positions[MAX_COLLECTABLES];
float 									speeds[MAX_COLLECTABLES];
Collectable::Type						types[MAX_COLLECTABLES];
std::function<void()>					callables[Collectable::Type::length];

int		last = 0;
float	cooldown_bone = 0.0f;
float	cooldown_shit = 0.0f;
float	cooldown_frozen = 0.0f;

int 	speed_mod		= 100000;
int		bone_spawn_mod 	= 0;
int		shit_spawn_mod 	= 0;
int		bone_spawn_rate = GetRandomValue(SPAWN_RATE_BONE_MIN, SPAWN_RATE_BONE_MAX);
int		shit_spawn_rate = GetRandomValue(SPAWN_RATE_SHIT_MIN, SPAWN_RATE_SHIT_MAX);

std::bitset<Collectable::Type::length> 	collected_flags;

Texture2D c_textures[Collectable::Type::length];
float rotations[MAX_COLLECTABLES];

int Collectable::GetVals(int type)
{
	switch(type)
	{
	case 0:
		return bone_spawn_mod;
		break;
	case 1:
		return shit_spawn_mod;
		break;
	case 2:
		return speed_mod;
		break;
	default: return 0;
	}
}


void InitAbility(Collectable::Type t)
{
	// Set collectible flag to true (USED primarily with duration abilities)
	collected_flags.set(t);

	// Perform one time event with collected collectible
	switch(t)
	{
	case Type::NORMAL:
		collected_flags.reset(Type::NORMAL);
		callables[Type::NORMAL]();
		break;

	case Type::FROZEN:
		cooldown_frozen = 0.0f;
		break;

	case Type::GOLDEN:
		collected_flags.reset(Type::GOLDEN);
		callables[Type::GOLDEN]();
		break;

	case Type::SHIT:
		collected_flags.reset(Type::SHIT);
		callables[Type::SHIT]();
		break;

	default:
		break;
	}
}

void DurationAbility()
{
	// If frozen collected flag set, then count frozen time; reset when needed
	if(collected_flags[Type::FROZEN])
	{
		cooldown_frozen += GetFrameTime();
		if(cooldown_frozen >= FROZEN_DURATION){
			collected_flags.reset(Type::FROZEN);
			cooldown_frozen = 0.0f;
		}
	}
}

void Move(Vector2 bounds, Vector2 dog_pos, Vector2 dog_size)
{
	Vector2 pos;
	float speed = 0.0f;
	Type collected = Type::NONE;

	Vector2 size = (Vector2){ GetScreenWidth()*size_perc.x, GetScreenHeight()*size_perc.y };
	
	// Loop through only active collectibles
	for(int i=0-1; i<last; i++)
	{
		if(!(types[i]==Type::SHIT && collected_flags[Type::FROZEN])) rotations[i] += ROT_SPEED;
		if(rotations[i] > 360.0f) rotations[i] -= 360.0f;

		// Freeze shit if needed
		speed = (collected_flags[Type::FROZEN] && types[i] == Type::SHIT) ? 0.0f : speeds[i];

		// Move collectible down
		positions[i].y += speed * (speed_mod/100000.0f) * GetFrameTime();
		pos = positions[i];

		// Remove collectible if at bottom of screen
		if(pos.y > bounds.y)
		{
			Collectable::Remove(i);
			continue;
		}

		// Check if collectible hits dog; continue to next collectible if no hit
		if(!CheckCollisionRecs( (Rectangle){pos.x, pos.y, size.x, size.y}, (Rectangle){dog_pos.x, dog_pos.y, dog_size.x, dog_size.y} )) continue;

		// Collect the collectable and despawn
		InitAbility(types[i]);
		Collectable::Remove(i);
	}
}

void Spawn(Vector2 bounds)
{
	// Update timer to spawn bone
	cooldown_bone += GetFrameTime();
	if(cooldown_bone >= bone_spawn_rate/100000.0f)
	{
		bone_spawn_rate = GetRandomValue(SPAWN_RATE_BONE_MIN - bone_spawn_mod, SPAWN_RATE_BONE_MAX - bone_spawn_mod);

		int rdm = GetRandomValue(0, 100);

		// Spawn specific bone with set chance
		if(rdm <= CHANCE_GOLDEN) 		Collectable::Add(bounds.x, Type::GOLDEN);
		else if(rdm <= CHANCE_FROZEN) 	Collectable::Add(bounds.x, Type::FROZEN);
		else if(rdm <= CHANCE_NORMAL) 	Collectable::Add(bounds.x, Type::NORMAL);

		cooldown_bone = 0.0f;
	}

	// Update timer to spawn shit; don't spawn if frozen flag is set
	cooldown_shit += GetFrameTime();
	if(cooldown_shit >= shit_spawn_rate/100000.0f && !collected_flags[Type::FROZEN])
	{
		shit_spawn_rate = GetRandomValue(SPAWN_RATE_SHIT_MIN - shit_spawn_mod, SPAWN_RATE_SHIT_MAX - shit_spawn_mod);

		Collectable::Add(bounds.x, Type::SHIT);
		cooldown_shit = 0.0f;
	}
}


void Collectable::Init()
{
	c_textures[Type::NORMAL] = LoadTexture("assets/bone-white.png");
	c_textures[Type::FROZEN] = LoadTexture("assets/bone-blue.png");
	c_textures[Type::GOLDEN] = LoadTexture("assets/bone-gold.png");
	c_textures[Type::SHIT] = LoadTexture("assets/poop.png");
}

void Collectable::SetCallable(Type t, std::function<void()> func) {callables[t] = func;}

void Collectable::Add(int bound, Collectable::Type t)
{
	// Check if max collectables is reach
	if(last == MAX_COLLECTABLES) return;

	// Randomize parameters of collectible
	speeds[last] = GetRandomValue(MIN_FALL_SPD, MAX_FALL_SPD);
	// positions[last].x = GetRandomValue(0.0f, bound);
	positions[last].x = GetRandomValue( 0.0f, GetScreenWidth() - GetScreenWidth()*size_perc.x - GetScreenWidth()*0.15f );
	positions[last].y = 20.0f;
	rotations[last] = (float)GetRandomValue(0, 360);

	// Set type of collectible, and make active
	types[last] = t;
	active.set(last);
	
	last++;
}

void Collectable::Remove(int idx)
{
	// Check bounds
	if(!(idx >= 0 && idx < MAX_COLLECTABLES)) return;
	if(last == 0) return;

	// Reset idx position and disable activity; lessen index of last active collectable
	positions[idx].x = 0.0f;
	positions[idx].y = 0.0f;
	rotations[idx] = 0.0f;
	active.reset(idx);
	last--;

	// Check if idx is at the end of collectables
	if(idx == MAX_COLLECTABLES-1) return;

	// Move inactive collectible towards the end;
	// Ensures only active collectibles are in the front of each array, respecting relativity b/w each array
	for(int i=idx; i<MAX_COLLECTABLES-1; i++)
	{
		if(!active[i + 1]) break;

		// Set current position w/ the right position
		positions[i].x = positions[i + 1].x;
		positions[i].y = positions[i + 1].y;
		positions[i + 1].x = 0.0f;
		positions[i + 1].y = 0.0f;

		rotations[i] = rotations[i + 1];
		// rotations[i] = 0.0f;

		// Set current active w/ the right active
		active.set(i);
		active.reset(i + 1);

		// Set current type w/ the right type
		types[i] = types[i + 1];
		types[i + 1] = Type::NONE;

		// Set current speed w/ the right speed
		speeds[i] = speeds[i + 1];
		speeds[i + 1] = 0.0f;
	}
}

void Collectable::Update(Vector2 bounds, Vector2 dog_pos, Vector2 dog_size)
{
	// Physically move active collectibles;
	// Check collisions with dog OR in screen bounds
	Move(bounds, dog_pos, dog_size);

	// Update abilities that need timers
	DurationAbility();

	// Spawn new collectibles
	Spawn(bounds);

	bone_spawn_mod += (bone_spawn_mod < RATE_MOD_BONE_MAX) ? RATE_MOD_BONE : 0;
	shit_spawn_mod += (shit_spawn_mod < RATE_MOD_SHIT_MAX) ? RATE_MOD_SHIT : 0;
	speed_mod += (speed_mod < SPEED_MOD_MAX) ? SPEED_MOD : 0;
}

void Collectable::Draw()
{
	Vector2 size = (Vector2){ (float)GetScreenWidth()*size_perc.x, (float)GetScreenHeight()*size_perc.y };
	float scale = 0.17f;
	float rot = 0.0f;

	switch(GetScreenWidth())
	{
    case 368:
    	scale -= 0.14f;
    	break;
    case 512:
    	scale -= 0.12f;
    	break;
    case 640:
    	scale -= 0.10f;
    	break;
    case 768:
    	scale -= 0.08f;
    	break;
    case 896:
    	scale -= 0.06f;
    	break;
    case 1024:
    	scale -= 0.04f;
    	break;
    case 1152:
    	scale -= 0.02f;
    	break;
    case 1280:
    	scale -= 0.0f;;
    	break;
    default:
    	scale = 0.06;
    	break;
    }

	// Loop through only active collectibles
	for(int i=0; i<last; i++)
	{
		rot = rotations[i];
		switch(types[i])
		{
		case Type::NORMAL:
			DrawTexturePro(c_textures[Type::NORMAL], (Rectangle){0,0,(float)c_textures[Type::NORMAL].width,(float)c_textures[Type::NORMAL].height}, (Rectangle){positions[i].x,positions[i].y,(float)c_textures[Type::NORMAL].width*scale,(float)c_textures[Type::NORMAL].height*scale}, (Vector2){c_textures[Type::NORMAL].width*(scale*0.5f),c_textures[Type::NORMAL].height*(scale*0.5f)}, rot, (Color){255,255,255,255});
			// DrawTextureEx( c_textures[Type::NORMAL], Vector2Subtract(positions[i], (Vector2){15.0f, 35.0f}), rot, scale, (Color){255,255,255,255} );
			// DrawRectangleV( positions[i], size, (Color){ 255, 255, 255, 255 } );
			break;

		case Type::FROZEN:
			// DrawRectangleV( positions[i], size, (Color){ 100, 100, 255, 255 } );
			// DrawTextureEx( c_textures[Type::FROZEN], Vector2Subtract(positions[i], (Vector2){15.0f, 35.0f}), rot, scale, (Color){255,255,255,255} );
			DrawTexturePro(c_textures[Type::FROZEN], (Rectangle){0,0,(float)c_textures[Type::NORMAL].width,(float)c_textures[Type::NORMAL].height}, (Rectangle){positions[i].x,positions[i].y,(float)c_textures[Type::NORMAL].width*scale,(float)c_textures[Type::NORMAL].height*scale}, (Vector2){c_textures[Type::NORMAL].width*(scale*0.5f),c_textures[Type::NORMAL].height*(scale*0.5f)}, rot, (Color){255,255,255,255});
			break;

		case Type::GOLDEN:
			// DrawRectangleV( positions[i], size, (Color){ 255, 255, 0, 255 } );
			// DrawTextureEx( c_textures[Type::GOLDEN], Vector2Subtract(positions[i], (Vector2){15.0f, 35.0f}), rot, scale, (Color){255,255,255,255} );
			DrawTexturePro(c_textures[Type::GOLDEN], (Rectangle){0,0,(float)c_textures[Type::NORMAL].width,(float)c_textures[Type::NORMAL].height}, (Rectangle){positions[i].x,positions[i].y,(float)c_textures[Type::NORMAL].width*scale,(float)c_textures[Type::NORMAL].height*scale}, (Vector2){c_textures[Type::NORMAL].width*(scale*0.5f),c_textures[Type::NORMAL].height*(scale*0.5f)}, rot, (Color){255,255,255,255});
			break;

		case Type::SHIT:
			DrawTexturePro(c_textures[Type::SHIT], (Rectangle){0,0,(float)c_textures[Type::SHIT].width,(float)c_textures[Type::SHIT].height}, (Rectangle){positions[i].x + (float)c_textures[Type::SHIT].width*scale*0.5f,positions[i].y,(float)c_textures[Type::SHIT].width*scale,(float)c_textures[Type::SHIT].height*scale}, (Vector2){c_textures[Type::SHIT].width*(scale*0.5f),c_textures[Type::SHIT].height*(scale*0.5f)}, rot, (Color){255,255,255,255});
			// DrawRectangleV( positions[i], size, (Color){ 255, 0, 255, 255 } );
			break;

		default:
			break;
		}
	}
}

void Collectable::Reset()
{
	// Reset all collectibles to inactive
	for(int i=0; i<active.size(); i++)
		active.reset(i);

	// Reset all collected collectibles to false
	for(int i=0; i<collected_flags.size(); i++)
		collected_flags.reset(i);

	// Reset all counters
	last = 0;
	cooldown_shit = 0.0f;
	cooldown_bone = 0.0f;
	cooldown_frozen = 0.0f;

	speed_mod = 100000;
	bone_spawn_mod = 0;
	shit_spawn_mod = 0;
	bone_spawn_rate = GetRandomValue(SPAWN_RATE_BONE_MIN, SPAWN_RATE_BONE_MAX);
	shit_spawn_rate = GetRandomValue(SPAWN_RATE_SHIT_MIN, SPAWN_RATE_SHIT_MAX);
}