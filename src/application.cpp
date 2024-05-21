#include "raylib.h"
#include "raymath.h"

#include <iostream>

#define PLATFORM_WEB
#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

#include "config.h"
Vector2 SCREEN = (Vector2){ 100, 100 };

extern "C" {
void SetScreen(int w, int h)
{
    EM_ASM({ Module.print($0, $1); }, w, h);
    SCREEN.x = w;
    SCREEN.y = h;

    SetWindowSize(w, h);

    EM_ASM({
        resizeCanvas($0, $1);
    }, w, h);
}
}

#include "dog.h"
#include "collectable.h"
#include "gamestate.h"
#include "ui.h"

#include <string>

void UpdateDrawFrame(void);
Camera2D viewport;

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(SCREEN.x, SCREEN.y, "Collectables N Shit");

    EM_ASM({ init() });
    
    // Set what certain collectables do
    Collectable::SetCallable(Collectable::Type::NORMAL  , [](){ Dog::SetScore( Dog::GetScore()+1 );             });
    Collectable::SetCallable(Collectable::Type::GOLDEN  , [](){ Dog::SetScore( Dog::GetScore()+GOLDEN_AMNT );   });
    Collectable::SetCallable(Collectable::Type::SHIT    , [](){ Dog::SetHealth( Dog::GetHealth()-1 );           });

    // Update callbacks
    //--------------------------------------------------------------------------------------
    // Allow closing window if not on web build
    #ifndef PLATFORM_WEB
    GameState::SetStateCallback(GameState::States::EXIT, GameState::CallbackType::ENTER, [](){
        CloseWindow();
    });
    #endif

    GameState::SetStateCallback(GameState::States::INIT, GameState::CallbackType::UPDATE, [](){
        

        GameState::ChangeState(GameState::States::MAIN);
    });

    #ifdef PLATFORM_WEB
    GameState::SetStateCallback(GameState::States::MAIN, GameState::CallbackType::ENTER, [](){
        
    });
    #endif
    GameState::SetStateCallback(GameState::States::MAIN, GameState::CallbackType::UPDATE, [](){
        if(IsKeyPressed(KEY_P)) GameState::ChangeState( GameState::States::RESET );
        if(IsKeyPressed(KEY_SPACE)) EM_ASM({ init(); });
    });

    // Reset game, then start playing
    GameState::SetStateCallback(GameState::States::RESET, GameState::CallbackType::UPDATE, [](){
        Dog::Reset();
        Dog::SetPos( (Vector2){ (float)GetScreenWidth()/2.0f, (float)GetScreenHeight()-Dog::GetSize().y-10 } );

        Collectable::Reset();

        GameState::ChangeState(GameState::States::PLAY);
    });

    // Update player and collectables when in play
    GameState::SetStateCallback(GameState::States::PLAY, GameState::CallbackType::UPDATE, [](){
        if(IsKeyPressed(KEY_P))     GameState::ChangeState( GameState::States::PAUSE );
        if(Dog::GetHealth() <= 0)   GameState::ChangeState( GameState::States::GAMEOVER );
        if(IsKeyPressed(KEY_SPACE))   GameState::ChangeState( GameState::States::GAMEOVER );

        Dog::Update(IsKeyDown(KEY_D) - IsKeyDown(KEY_A));
        Collectable::Update((Vector2){(float)GetScreenWidth(), (float)GetScreenHeight()}, Dog::GetPos());
    });

    // Check for key input on pause menu
    GameState::SetStateCallback(GameState::States::PAUSE, GameState::CallbackType::UPDATE, [](){
        if(IsKeyPressed(KEY_P)) GameState::ChangeState( GameState::States::PLAY );
    });

    // Update for GAMEOVER scene
    GameState::SetStateCallback(GameState::States::GAMEOVER, GameState::CallbackType::UPDATE, [](){
        if(IsKeyPressed(KEY_R)) GameState::ChangeState( GameState::States::RESET );
    });
    //--------------------------------------------------------------------------------------

    // Draw callbacks
    //--------------------------------------------------------------------------------------
    // Draw main menu
    GameState::SetStateCallback(GameState::States::MAIN, GameState::CallbackType::DRAW, [](){
        UI::SetMode( UI::Modes::CENTER );
        UI::Text("BONES N SH*T", (Vector2){(float)GetScreenWidth()/2.0f, (float)GetScreenHeight() * 0.1f}, UI::Presets::TEXT_TITLE);

        if(UI::Button("Play", (Vector2){(float)GetScreenWidth()/2.0f, (float)GetScreenHeight() * 0.2f}, UI::Presets::BUTTON_MEDIUM))
            GameState::ChangeState( GameState::States::RESET );

        // Allow showing of exit button if not on web build
        #ifndef PLATFORM_WEB
        if(UI::Button("Exit", (Vector2){(float)GetScreenWidth()/2.0f, (float)GetScreenHeight() * 0.3f}, UI::Presets::BUTTON_LARGE))
            GameState::ChangeState( GameState::States::EXIT );
        #endif
    });

    // Draw dog, collectables, and stats when in play
    GameState::SetStateCallback(GameState::States::PLAY, GameState::CallbackType::DRAW, [](){
        UI::SetMode( UI::Modes::CENTER );

        Dog::Draw();
        Collectable::Draw();

        UI::Text(std::to_string(Dog::GetScore()).c_str(), (Vector2){(float)GetScreenWidth()/2.0f, 40}, UI::Presets::TEXT_LARGE);
        UI::Text(std::to_string(Dog::GetHealth()).c_str(), (Vector2){(float)GetScreenWidth()/2.0f, 80}, UI::Presets::TEXT_SMALL);

        UI::SetMode( UI::Modes::CENTER_LEFT );
        UI::Text(std::to_string(Collectable::GetVals(0)/100000.0f).c_str(), (Vector2){(float)GetScreenWidth()/1.5f, 40}, UI::Presets::TEXT_SMALL);
        UI::Text(std::to_string(Collectable::GetVals(1)/100000.0f).c_str(), (Vector2){(float)GetScreenWidth()/1.5f, 80}, UI::Presets::TEXT_SMALL);
        UI::Text(std::to_string(Collectable::GetVals(2)/100000.0f).c_str(), (Vector2){(float)GetScreenWidth()/1.5f, 120}, UI::Presets::TEXT_SMALL);

        UI::SetMode( UI::Modes::TOP_LEFT );
        if(UI::Button("Pause", (Vector2){20, 20}, UI::Presets::BUTTON_SMALL))
            GameState::ChangeState( GameState::States::PAUSE );
    });

    // Draw dog, collectables, and stats when paused
    GameState::SetStateCallback(GameState::States::PAUSE, GameState::CallbackType::DRAW, [](){
        UI::SetMode( UI::Modes::CENTER );

        Dog::Draw();
        Collectable::Draw();

        UI::Text(std::to_string(Dog::GetScore()).c_str(), (Vector2){(float)GetScreenWidth()/2.0f, 40}, UI::Presets::TEXT_LARGE);
        UI::Text(std::to_string(Dog::GetHealth()).c_str(), (Vector2){(float)GetScreenWidth()/2.0f, 80}, UI::Presets::TEXT_SMALL);

        UI::SimpleRect((Rectangle){(float)GetScreenWidth()/2.0f, (float)GetScreenHeight()/2.0f, (float)GetScreenWidth()-10, (float)GetScreenHeight()-10}, (Color){0, 0, 0, 200});
        UI::Text("PAUSED", (Vector2){(float)GetScreenWidth()/2.0f, 150}, UI::Presets::TEXT_TITLE);
        if(UI::Button("Resume", (Vector2){(float)GetScreenWidth()/2.0f, (float)GetScreenHeight()/2.0f + 80}, UI::Presets::BUTTON_SMALL))
            GameState::ChangeState( GameState::States::PLAY );
    });

    // Draw gameover scene
    GameState::SetStateCallback(GameState::States::GAMEOVER, GameState::CallbackType::DRAW, [](){
        UI::SetMode( UI::Modes::CENTER );
        UI::Text("GAMEOVER", (Vector2){(float)GetScreenWidth()/2.0f, 150}, UI::Presets::TEXT_TITLE);

        UI::SetMode( UI::Modes::CENTER_RIGHT );
        UI::Text("Bones Collected: ", (Vector2){(float)GetScreenWidth()/1.7f, 300}, UI::Presets::TEXT_LARGE);

        UI::SetMode( UI::Modes::CENTER_LEFT );
        UI::Text(std::to_string(Dog::GetScore()).c_str(), (Vector2){(float)GetScreenWidth()/1.7f, 300}, UI::Presets::TEXT_MEDIUM);

        UI::SetMode( UI::Modes::CENTER );
        if(UI::Button("Replay", (Vector2){(float)GetScreenWidth()/2.0f, (float)GetScreenHeight()/2.0f + 80}, UI::Presets::BUTTON_SMALL))
            GameState::ChangeState( GameState::States::RESET );
    });
    //--------------------------------------------------------------------------------------
    
    SetTargetFPS(60);                   // Set our game to run at 60 frames-per-second

    #if defined(PLATFORM_WEB)
        emscripten_set_main_loop(UpdateDrawFrame, 0, 1);

    #else
        //--------------------------------------------------------------------------------------

        // Main game loop
        while (!WindowShouldClose())        // Detect window close button or ESC key
            UpdateDrawFrame();

    #endif

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

void UpdateDrawFrame(void)
{
    // Update
    //----------------------------------------------------------------------------------
    GameState::Update();

    //----------------------------------------------------------------------------------

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();

        ClearBackground(BLACK);

        // BeginMode2D(viewport);

        GameState::Draw();
        
        // EndMode2D();

    EndDrawing();
    //----------------------------------------------------------------------------------
}
