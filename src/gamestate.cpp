#include "gamestate.h"
using namespace GameState;

// Current game state
GameState::States game = GameState::States::MAIN;

// Callbacks for wanted game states
std::function<void()> enters[GameState::States::length];
std::function<void()> updates[GameState::States::length];
std::function<void()> leaves[GameState::States::length];
std::function<void()> draws[GameState::States::length];

States GameState::GetState() {return game;}
void GameState::ForceState(States state) {game = state;}
void GameState::ChangeState(GameState::States state)
{
	// Call leaving callback for current state
	if(leaves[game]) leaves[game]();

	game = state;

	// Call entering callback for new state
	if(enters[game]) enters[game]();
}


void GameState::SetStateCallback(GameState::States state, GameState::CallbackType type, std::function<void()> func)
{
	// Set the callback for given state based on the type of callback
	switch(type)
	{
	case CallbackType::ENTER:
		enters[state] = func;
		break;
	case CallbackType::UPDATE:
		updates[state] = func;
		break;
	case CallbackType::LEAVE:
		leaves[state] = func;
		break;
	case CallbackType::DRAW:
		draws[state] = func;
		break;
	}
}

void GameState::Update()
{
	// If current state has update, call it
	if(!updates[game]) return;
	updates[game]();
}
void GameState::Draw()
{
	// If current state has draw, call it
	if(!draws[game]) return;
	draws[game]();
}