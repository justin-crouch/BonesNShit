#include "collectable.h"
using namespace Collectable;

std::bitset<MAX_COLLECTABLES> 			active;
Vector2 								positions[MAX_COLLECTABLES];
float 									speeds[MAX_COLLECTABLES];
Collectable::Type						types[MAX_COLLECTABLES];
std::function<void()>					callables[Collectable::Type::length];

int 									last = 0;
float 									cooldown_bone = 0.0f;
float 									cooldown_shit = 0.0f;
float 									cooldown_frozen = 0.0f;
std::bitset<Collectable::Type::length> 	collected_flags;

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

void Move(Vector2 bounds, Vector2 dog)
{
	Vector2 pos;
	float speed = 0.0f;
	Type collected = Type::NONE;
	
	// Loop through only active collectibles
	for(int i=0-1; i<last; i++)
	{
		// Freeze shit if needed
		speed = (collected_flags[Type::FROZEN] && types[i] == Type::SHIT) ? 0.0f : speeds[i];

		// Move collectible down
		positions[i].y += speed * GetFrameTime();
		pos = positions[i];

		// Remove collectible if at bottom of screen
		if(pos.y > bounds.y)
		{
			Collectable::Remove(i);
			continue;
		}

		// Check if collectible hits dog; continue to next collectible if no hit
		if(!CheckCollisionRecs( (Rectangle){pos.x, pos.y, 50.0f, 10.0f}, (Rectangle){dog.x, dog.y, 100.0f, 50.0f} )) continue;

		// Collect the collectable and despawn
		InitAbility(types[i]);
		Collectable::Remove(i);
	}
}

void Spawn(Vector2 bounds)
{
	// Update timer to spawn bone
	cooldown_bone += GetFrameTime();
	if(cooldown_bone >= SPAWN_RATE_BONE)
	{
		int rdm = GetRandomValue(0, 100);

		// Spawn specific bone with set chance
		if(rdm <= CHANCE_GOLDEN) 		Collectable::Add(bounds.x, Type::GOLDEN);
		else if(rdm <= CHANCE_FROZEN) 	Collectable::Add(bounds.x, Type::FROZEN);
		else if(rdm <= CHANCE_NORMAL) 	Collectable::Add(bounds.x, Type::NORMAL);

		cooldown_bone = 0.0f;
	}

	// Update timer to spawn shit; don't spawn if frozen flag is set
	cooldown_shit += GetFrameTime();
	if(cooldown_shit >= SPAWN_RATE_SHIT && !collected_flags[Type::FROZEN])
	{
		Collectable::Add(bounds.x, Type::SHIT);
		cooldown_shit = 0.0f;
	}
}


void Collectable::SetCallable(Type t, std::function<void()> func) {callables[t] = func;}

void Collectable::Add(int bound, Collectable::Type t)
{
	// Check if max collectables is reach
	if(last == MAX_COLLECTABLES) return;

	// Randomize parameters of collectible
	speeds[last] = GetRandomValue(MIN_FALL_SPD, MAX_FALL_SPD);
	positions[last].x = GetRandomValue(0, bound);
	positions[last].y = 20.0f;

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

void Collectable::Update(Vector2 bounds, Vector2 dog)
{
	// Physically move active collectibles;
	// Check collisions with dog OR in screen bounds
	Move(bounds, dog);

	// Update abilities that need timers
	DurationAbility();

	// Spawn new collectibles
	Spawn(bounds);
}

void Collectable::Draw()
{
	// Loop through only active collectibles
	for(int i=0; i<last; i++)
	{
		switch(types[i])
		{
		case Type::NORMAL:
			DrawRectangleV( positions[i], (Vector2){ 50.0f, 10.0f }, (Color){ 255, 255, 255, 255 } );
			break;

		case Type::FROZEN:
			DrawRectangleV( positions[i], (Vector2){ 50.0f, 10.0f }, (Color){ 100, 100, 255, 255 } );
			break;

		case Type::GOLDEN:
			DrawRectangleV( positions[i], (Vector2){ 50.0f, 10.0f }, (Color){ 255, 255, 0, 255 } );
			break;

		case Type::SHIT:
			DrawRectangleV( positions[i], (Vector2){ 50.0f, 10.0f }, (Color){ 255, 0, 255, 255 } );
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
}