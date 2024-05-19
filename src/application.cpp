#include "raylib.h"
#include "raymath.h"

#include "dog.h"
#include "collectable.h"
#include "gamestate.h"
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

    // Set what certain game states do
    // Reset game, then start playing
    GameState::SetStateCallback(GameState::States::RESET, GameState::CallbackType::UPDATE, [](){
        Dog::Reset();
        Dog::SetPos( (Vector2){ SCREEN.x/2.0f, SCREEN.y-Dog::GetSize().y-10 } );

        Collectable::Reset();

        GameState::ChangeState(GameState::States::PLAY);
    });

    // Update player and collectables when in play
    GameState::SetStateCallback(GameState::States::PLAY, GameState::CallbackType::UPDATE, [](){
        if(IsKeyPressed(KEY_SPACE)) 
        {
            GameState::ChangeState( GameState::States::RESET );
            return;
        }

        Dog::Update(IsKeyDown(KEY_D) - IsKeyDown(KEY_A));
        Collectable::Update(SCREEN, Dog::GetPos());
    });
    // Draw dog, collectables, and stats when in play
    GameState::SetStateCallback(GameState::States::PLAY, GameState::CallbackType::DRAW, [](){
        Dog::Draw();
        Collectable::Draw();

        DrawText(std::to_string(Dog::GetScore()).c_str(), SCREEN.x/2.0f, 40, 40, (Color){255, 255, 255, 255});
        DrawText(std::to_string(Dog::GetHealth()).c_str(), SCREEN.x/2.0f, 80, 40, (Color){255, 255, 255, 255});
    });

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