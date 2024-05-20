#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <functional>

namespace GameState
{
	typedef enum {
	    MAIN,
	    OPTIONS,
	    EXIT,
	    
	    RESET,
	    PAUSE,
	    PLAY,
	    GAMEOVER,
	    length
	} States;

	typedef enum {
		ENTER,
		UPDATE,
		LEAVE,
		DRAW,
	} CallbackType;

	States GetState();
	void ForceState(States state);
	void ChangeState(States state);

	void SetStateCallback(States state, CallbackType type, std::function<void()> func);

	void Update();
	void Draw();
}

#endif