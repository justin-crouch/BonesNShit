#include "ui.h"

UI::Modes draw_mode = UI::Modes::TOP_LEFT;

void UI::SetMode(Modes mode) {draw_mode = mode;}

bool UI::Button(const char* text, Vector2 position, Presets preset)
{
	Rectangle rect{position.x, position.y};
	int font_size;
	Color fg, bg, hover;

	switch(preset)
	{
	case Presets::BUTTON_SMALL:
		rect.width = Style::BTN_SIZE_SMALL.x;
		rect.height = Style::BTN_SIZE_SMALL.y;
		font_size = Style::SMALL_FONT;
		fg = BTN_FG;
		bg = BTN_BG;
		hover = BTN_HOVER;
		break;

	case Presets::BUTTON_MEDIUM:
		rect.width = Style::BTN_SIZE_MEDIUM.x;
		rect.height = Style::BTN_SIZE_MEDIUM.y;
		font_size = Style::MEDIUM_FONT;
		fg = BTN_FG;
		bg = BTN_BG;
		hover = BTN_HOVER;
		break;

	case Presets::BUTTON_LARGE:
		rect.width = Style::BTN_SIZE_LARGE.x;
		rect.height = Style::BTN_SIZE_LARGE.y;
		font_size = Style::LARGE_FONT;
		fg = BTN_FG;
		bg = BTN_BG;
		hover = BTN_HOVER;
		break;

	default:
		return false;
	}

	return ButtonEx(text, rect, font_size, fg, bg, hover);
}
bool UI::ButtonEx(const char* text, Rectangle rect, int font_size, Color fg, Color bg, Color hover)
{
	Rectangle collider = (Rectangle){
		rect.x, 
		rect.y, 
		(MeasureText(text, font_size) < rect.width ? rect.width : ((float)MeasureText(text, font_size)+10)), 
		rect.height
	};

	switch(draw_mode)
	{
	case Modes::CENTER:
		collider.x -= collider.width/2.0f;
		collider.y -= collider.height/2.0f;
		break;
	case Modes::TOP_LEFT:
		break;
	case Modes::CENTER_RIGHT:
		break;
	case Modes::CENTER_LEFT:
		break;
	}

	Vector2 mouse_pos = GetMousePosition();
	Color onhover = CheckCollisionPointRec( mouse_pos, collider ) ? hover : bg;

	DrawRectangleRec(collider, onhover);
	DrawText(text, collider.x + collider.width/2.0f - MeasureText(text, font_size)/2.0f, collider.y + collider.height/2.0f - font_size/2.0f, font_size, fg);

	return (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) ? CheckCollisionPointRec( mouse_pos, collider ) : false);
}

void UI::Text(const char* text, Vector2 position, Presets preset)
{
	int font_size;
	Color color;

	switch(preset)
	{
	case Presets::TEXT_SMALL:
		font_size = Style::SMALL_FONT;
		color = TEXT_COLOR;
		break;

	case Presets::TEXT_MEDIUM:
		font_size = Style::MEDIUM_FONT;
		color = TEXT_COLOR;
		break;

	case Presets::TEXT_LARGE:
		font_size = Style::LARGE_FONT;
		color = TEXT_COLOR;
		break;

	case Presets::TEXT_TITLE:
		font_size = Style::VERY_LARGE_FONT;
		color = TEXT_COLOR;
		break;

	default:
		return;
	}

	TextEx(text, position, font_size, color);
}
void UI::TextEx(const char* text, Vector2 position, int font_size, Color color)
{
	Vector2 offset = (Vector2){ 0, 0 };
	switch(draw_mode)
	{
	case Modes::CENTER:
		offset.x = -MeasureText(text, font_size)/2.0f;
		offset.y = -font_size/2.0f;
		break;
	case Modes::TOP_LEFT:
		break;
	case Modes::CENTER_RIGHT:
		offset.x = -MeasureText(text, font_size);
		offset.y = -font_size/2.0f;
		break;
	case Modes::CENTER_LEFT:
		offset.y = -font_size/2.0f;
		break;
	}

	DrawText(text, position.x+offset.x, position.y+offset.y, font_size, color);
}

void UI::SimpleRect(Rectangle rect, Color color)
{
	Vector2 offset = (Vector2){ 0, 0 };
	switch(draw_mode)
	{
	case Modes::CENTER:
		offset.x = rect.width/2.0f;
		offset.y = rect.height/2.0f;
		break;
	case Modes::TOP_LEFT:
		break;
	case Modes::CENTER_RIGHT:
		break;
	case Modes::CENTER_LEFT:
		break;
	}

	rect.x -= offset.x;
	rect.y -= offset.y;
	DrawRectangleRec(rect, color);
}