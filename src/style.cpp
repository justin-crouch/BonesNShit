#include "style.h"
using namespace Style;

int Style::SMALL_FONT      = 8;
int Style::MEDIUM_FONT     = 16;
int Style::LARGE_FONT      = 24;
int Style::VERY_LARGE_FONT = 32;

Vector2 Style::BTN_SIZE_SMALL  = (Vector2){    (float)100,    (float)8  +   (float)8   };
Vector2 Style::BTN_SIZE_MEDIUM = (Vector2){    (float)200,    (float)16 +   (float)8   };
Vector2 Style::BTN_SIZE_LARGE  = (Vector2){    (float)300,    (float)24  +   (float)8   };

void Style::SetFontSizes(Vector2 Screen)
{
    if(Screen.y < 368)
    {
        SMALL_FONT      = 4;
        MEDIUM_FONT     = 8;
        LARGE_FONT      = 16;
        VERY_LARGE_FONT = 24;
    } else if(Screen.x >= 368 && Screen.x < 512)
    {
        SMALL_FONT      = 8;
	    MEDIUM_FONT     = 16;
	    LARGE_FONT      = 24;
	    VERY_LARGE_FONT = 40;
    } else if(Screen.x >= 512 && Screen.x < 1024)
    {
        SMALL_FONT      = 16;
        MEDIUM_FONT     = 24;
        LARGE_FONT      = 32;
        VERY_LARGE_FONT = 40;
    } else if(Screen.x >= 1024)
    {
        SMALL_FONT      = 24;
        MEDIUM_FONT     = 32;
        LARGE_FONT      = 40;
        VERY_LARGE_FONT = 52;
    }
    BTN_SIZE_SMALL  = (Vector2){    (float)100,    (float)SMALL_FONT  +   (float)8   };
    BTN_SIZE_MEDIUM = (Vector2){    (float)200,    (float)MEDIUM_FONT +   (float)8   };
    BTN_SIZE_LARGE  = (Vector2){    (float)300,    (float)LARGE_FONT  +   (float)8   };
}