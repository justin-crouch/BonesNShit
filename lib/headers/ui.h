#ifndef UI_H
#define UI_H

#include "raylib.h"
#include "raymath.h"
#include "config.h"
#include "style.h"

namespace UI
{
	typedef enum{
		BUTTON_SMALL,
		BUTTON_MEDIUM,
		BUTTON_LARGE,

		TEXT_SMALL,
		TEXT_MEDIUM,
		TEXT_LARGE,
		TEXT_TITLE,
	} Presets;

	typedef enum{
		CENTER,
		TOP_LEFT,
	} Modes;

	void SetMode(Modes mode);

	bool Button(const char* text, Vector2 position, Presets preset);
	bool ButtonEx(const char* text, Rectangle rect, int font_size, Color fg, Color bg, Color hover);

	void Text(const char* text, Vector2 position, Presets preset);
	void TextEx(const char* text, Vector2 position, int font_size, Color color);

	void SimpleRect(Rectangle rect, Color color);
}

#endif
