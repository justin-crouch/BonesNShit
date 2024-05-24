#ifndef STYLE_H
#define STYLE_H

#include "raylib.h"
#include "raymath.h"

namespace Style{
	extern int SMALL_FONT;
	extern int MEDIUM_FONT;
	extern int LARGE_FONT;
	extern int VERY_LARGE_FONT;

	extern Vector2 BTN_SIZE_SMALL;
	extern Vector2 BTN_SIZE_MEDIUM;
	extern Vector2 BTN_SIZE_LARGE;

	void SetFontSizes(Vector2 screen);
}

#define BTN_FG			 (Color){ 0,	0,		0,		255	}
#define BTN_BG			 (Color){ 107,	190,	172,	255	}
#define BTN_HOVER		 (Color){ 99,	167,	238,	255	}

#define TEXT_COLOR		 (Color){ 0, 	0, 		0, 		255 }
#define TEXT_COLOR_HOVER (Color){ 0, 	0, 		0, 		255 }

#endif