#include "raylib.h"
#include "raymath.h"

#include "dog.h"
#include "collectable.h"
#include "gamestate.h"
#include "ui.h"
#include "config.h"

#include <string>

#define PLATFORM_WEB
#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

void UpdateDrawFrame(void);

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    InitWindow(SCREEN.x, SCREEN.y, "Collectables N Shit");
    
    // Set what certain collectables do
    Collectable::SetCallable(Collectable::Type::NORMAL  , [](){ Dog::SetScore( Dog::GetScore()+1 );             });
    Collectable::SetCallable(Collectable::Type::GOLDEN  , [](){ Dog::SetScore( Dog::GetScore()+GOLDEN_AMNT );   });
    Collectable::SetCallable(Collectable::Type::SHIT    , [](){
        Dog::SetHealth( Dog::GetHealth()-1 );
        if(Dog::GetHealth() <= 0) GameState::ChangeState(GameState::States::RESET);
    });


    // Update callbacks
    //--------------------------------------------------------------------------------------
    // Allow closing window if not on web build
    #ifndef PLATFORM_WEB
    GameState::SetStateCallback(GameState::States::EXIT, GameState::CallbackType::ENTER, [](){
        CloseWindow();
    });
    #endif

    // Reset game, then start playing
    GameState::SetStateCallback(GameState::States::RESET, GameState::CallbackType::UPDATE, [](){
        Dog::Reset();
        Dog::SetPos( (Vector2){ SCREEN.x/2.0f, SCREEN.y-Dog::GetSize().y-10 } );

        Collectable::Reset();

        GameState::ChangeState(GameState::States::PLAY);
    });

    // Update player and collectables when in play
    GameState::SetStateCallback(GameState::States::PLAY, GameState::CallbackType::UPDATE, [](){
        if(IsKeyPressed(KEY_P)) 
        {
            GameState::ChangeState( GameState::States::PAUSE );
            return;
        }

        Dog::Update(IsKeyDown(KEY_D) - IsKeyDown(KEY_A));
        Collectable::Update(SCREEN, Dog::GetPos());
    });

    // Check for key input on pause menu
    GameState::SetStateCallback(GameState::States::PAUSE, GameState::CallbackType::UPDATE, [](){
        if(IsKeyPressed(KEY_P)) GameState::ChangeState( GameState::States::PLAY );
    });
    //--------------------------------------------------------------------------------------

    // Draw callbacks
    //--------------------------------------------------------------------------------------
    // Draw main menu
    GameState::SetStateCallback(GameState::States::MAIN, GameState::CallbackType::DRAW, [](){
        UI::SetMode( UI::Modes::CENTER );
        UI::Text("BONES N SH*T", (Vector2){SCREEN.x/2.0f, 200}, UI::Presets::TEXT_TITLE);

        if(UI::Button("Play", (Vector2){SCREEN.x/2.0f, 300}, UI::Presets::BUTTON_MEDIUM))
            GameState::ChangeState( GameState::States::RESET );

        // Allow showing of exit button if not on web build
        #ifndef PLATFORM_WEB
        if(UI::Button("Exit", (Vector2){SCREEN.x/2.0f, 400}, UI::Presets::BUTTON_LARGE))
            GameState::ChangeState( GameState::States::EXIT );
        #endif
    });

    // Draw dog, collectables, and stats when in play
    GameState::SetStateCallback(GameState::States::PLAY, GameState::CallbackType::DRAW, [](){
        UI::SetMode( UI::Modes::CENTER );

        Dog::Draw();
        Collectable::Draw();

        UI::Text(std::to_string(Dog::GetScore()).c_str(), (Vector2){SCREEN.x/2.0f, 40}, UI::Presets::TEXT_LARGE);
        UI::Text(std::to_string(Dog::GetHealth()).c_str(), (Vector2){SCREEN.x/2.0f, 80}, UI::Presets::TEXT_SMALL);

        UI::SetMode( UI::Modes::TOP_LEFT );
        if(UI::Button("Pause", (Vector2){20, 20}, UI::Presets::BUTTON_SMALL))
            GameState::ChangeState( GameState::States::PAUSE );
    });

    // Draw dog, collectables, and stats when paused
    GameState::SetStateCallback(GameState::States::PAUSE, GameState::CallbackType::DRAW, [](){
        UI::SetMode( UI::Modes::CENTER );

        Dog::Draw();
        Collectable::Draw();

        UI::Text(std::to_string(Dog::GetScore()).c_str(), (Vector2){SCREEN.x/2.0f, 40}, UI::Presets::TEXT_LARGE);
        UI::Text(std::to_string(Dog::GetHealth()).c_str(), (Vector2){SCREEN.x/2.0f, 80}, UI::Presets::TEXT_SMALL);

        UI::SimpleRect((Rectangle){SCREEN.x/2.0f, SCREEN.y/2.0f, SCREEN.x-10, SCREEN.y-10}, (Color){0, 0, 0, 200});
        UI::Text("PAUSED", (Vector2){SCREEN.x/2.0f, 150}, UI::Presets::TEXT_TITLE);
        if(UI::Button("Resume", (Vector2){SCREEN.x/2.0f, SCREEN.y/2.0f + 80}, UI::Presets::BUTTON_SMALL))
            GameState::ChangeState( GameState::States::PLAY );
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

        GameState::Draw();

    EndDrawing();
    //----------------------------------------------------------------------------------
}