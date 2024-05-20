#include "ui.h"

void DrawButton(const char* text, Rectangle rect, int font_size, Color fg, Color bg, Color hover)
{

	Rectangle collider = (Rectangle){
		rect.x, 
		rect.y, 
		(MeasureText(text, font_size) < rect.width ? rect.width : ((float)MeasureText(text, font_size)+10)), 
		rect.height
	};

	Color onhover = CheckCollisionPointRec( GetMousePosition(), collider ) ? hover : bg;

	DrawRectangleRec(collider, onhover);
	DrawText(text, collider.x + collider.width/2.0f - MeasureText(text, font_size)/2.0f, collider.y + collider.height/2.0f - font_size/2.0f, font_size, fg);
}

void UI::Play(const char* score, const char* health)
{
	DrawText(score, SCREEN.x/2.0f, 40, 40, (Color){255, 255, 255, 255});
	DrawText(health, SCREEN.x/2.0f, 80, 40, (Color){255, 255, 255, 255});

	DrawButton("Button", (Rectangle){40, 40, 300, 80}, 20, (Color){255,255,255,255}, (Color){0,255,0,255}, (Color){255,0,0,255});
}