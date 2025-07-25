#include "ui.h"

UI::Modes draw_mode = UI::Modes::TOP_LEFT;

void UI::SetMode(Modes mode) {draw_mode = mode;}

UI::ClickHover UI::Button(const char* text, Vector2 position, Presets preset, Color hover)
{
	Rectangle rect{position.x, position.y};
	int font_size;
	Color fg, bg;

	switch(preset)
	{
	case Presets::BUTTON_SMALL:
		rect.width = Style::BTN_SIZE_SMALL.x;
		rect.height = Style::BTN_SIZE_SMALL.y;
		font_size = Style::SMALL_FONT;
		fg = BTN_FG;
		bg = BTN_BG;
		break;

	case Presets::BUTTON_MEDIUM:
		rect.width = Style::BTN_SIZE_MEDIUM.x;
		rect.height = Style::BTN_SIZE_MEDIUM.y;
		font_size = Style::MEDIUM_FONT;
		fg = BTN_FG;
		bg = BTN_BG;
		break;

	case Presets::BUTTON_LARGE:
		rect.width = Style::BTN_SIZE_LARGE.x;
		rect.height = Style::BTN_SIZE_LARGE.y;
		font_size = Style::LARGE_FONT;
		fg = BTN_FG;
		bg = BTN_BG;
		break;

	default:
		return ClickHover{false, false};
	}

	return ButtonEx(text, rect, font_size, fg, bg, hover);
}
UI::ClickHover UI::ButtonEx(const char* text, Rectangle rect, int font_size, Color fg, Color bg, Color hover)
{
	Rectangle collider = (Rectangle){
		rect.x, 
		rect.y, 
		(MeasureText(text, font_size) < rect.width ? rect.width : ((float)MeasureText(text, font_size)+10)), 
		rect.height
	};

	bool hovered = false;
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
	default:
		break;
	}

	Vector2 mouse_pos = GetMousePosition();
	Color onhover = bg;
	Color text_color = fg;
	if(CheckCollisionPointRec( mouse_pos, collider ))
	{
		onhover = hover;
		text_color = TEXT_COLOR_HOVER;
		hovered = true;
	}

	DrawRectangleRec(collider, (Color){0,0,0,255});
	DrawRectangleRec( (Rectangle){collider.x+3, collider.y+3, collider.width-6, collider.height-6} , onhover);
	DrawText(text, collider.x + collider.width/2.0f - MeasureText(text, font_size)/2.0f, collider.y + collider.height/2.0f - font_size/2.0f, font_size, text_color);

	return ClickHover{(IsMouseButtonReleased(MOUSE_BUTTON_LEFT) ? CheckCollisionPointRec( mouse_pos, collider ) : false), hovered};
}

void UI::Text(const char* text, Vector2 position, Presets preset, Color color)
{
	int font_size;
	
	switch(preset)
	{
	case Presets::TEXT_SMALL:
		font_size = Style::SMALL_FONT;
		break;

	case Presets::TEXT_MEDIUM:
		font_size = Style::MEDIUM_FONT;
		break;

	case Presets::TEXT_LARGE:
		font_size = Style::LARGE_FONT;
		break;

	case Presets::TEXT_TITLE:
		font_size = Style::VERY_LARGE_FONT;
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
	default:
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
	case Modes::TOP_RIGHT:
		offset.x = rect.width;
		break;
	case Modes::CENTER_RIGHT:
		break;
	case Modes::CENTER_LEFT:
		break;
	default:
		break;
	}

	rect.x -= offset.x;
	rect.y -= offset.y;
	DrawRectangleRec(rect, color);
}

void UI::Face(Texture2D tex, Vector2 pos, float scale)
{
	DrawCircleGradient(GetScreenWidth()-(float)GetScreenWidth()*0.075f, GetScreenHeight()*0.15f, GetScreenWidth()*0.1f, (Color){255,255,255,150}, (Color){255,255,255,0});
    DrawTextureEx( tex, pos, 0.0f, scale, (Color){255,255,255,255} );
}
