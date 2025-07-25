#ifndef UI_H
#define UI_H

#include "raylib.h"
#include "raymath.h"
#include "config.h"
#include "style.h"

namespace UI
{
	typedef struct ClickHover{
		bool clicked=false;
		bool hovered=false;
	} ClickHover;

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
		TOP_RIGHT,
		CENTER_RIGHT,
		CENTER_LEFT,
	} Modes;

	void SetButtonAudio(Sound sound);

	void SetMode(Modes mode);

	ClickHover Button(const char* text, Vector2 position, Presets preset, Color hover=BTN_HOVER);
	ClickHover ButtonEx(const char* text, Rectangle rect, int font_size, Color fg, Color bg, Color hover);

	void Text(const char* text, Vector2 position, Presets preset, Color color = TEXT_COLOR);
	void TextEx(const char* text, Vector2 position, int font_size, Color color);

	void SimpleRect(Rectangle rect, Color color);

	void Face(Texture2D tex, Vector2 pos, float scale);
}

#endif
