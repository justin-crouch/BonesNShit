#include "raylib.h"
#include "raymath.h"

#define PLATFORM_WEB
#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

#include "config.h"
Vector2 SCREEN = (Vector2){ 100, 100 };

#include "style.h"
#include "dog.h"

float bone_scale = 0.42f;

extern "C" {
void SetScreen(int w, int h)
{
    // EM_ASM({ Module.print($0, $1); }, w, h);
    if(w < h)
    {
        SCREEN.x = 207;
        SCREEN.y = 360;
        Dog::SetScale( Dog::States::NEUTRAL, 0.05f );
        Dog::SetScale( Dog::States::BONE, 0.2f );
    }else if(h < 368)
    {
        SCREEN.x = 500;
        SCREEN.y = 500*0.5652;
        Dog::SetScale( Dog::States::NEUTRAL, 0.2f );
        Dog::SetScale( Dog::States::BONE, 0.2f );
    } else if(w >= 368 && w < 512)
    {
        SCREEN.x = 368;
        SCREEN.y = 207;
        Dog::SetScale( Dog::States::NEUTRAL, 0.2f );
        Dog::SetScale( Dog::States::BONE, 0.2f );
    } else if(w >= 512 && w < 640)
    {
        SCREEN.x = 512;
        SCREEN.y = 288;
        Dog::SetScale( Dog::States::NEUTRAL, 0.2f );
        Dog::SetScale( Dog::States::BONE, 0.2f );
    } else if(w >= 640 && w < 768)
    {
        SCREEN.x = 640;
        SCREEN.y = 360;
        Dog::SetScale( Dog::States::NEUTRAL, 0.2f );
        Dog::SetScale( Dog::States::BONE, 0.2f );
    } else if(w >= 768 && w < 896)
    {
        SCREEN.x = 768;
        SCREEN.y = 432;
        Dog::SetScale( Dog::States::NEUTRAL, 0.2f );
        Dog::SetScale( Dog::States::BONE, 0.2f );
    } else if(w >= 896 && w < 1024)
    {
        SCREEN.x = 896;
        SCREEN.y = 504;
        Dog::SetScale( Dog::States::NEUTRAL, 0.2f );
        Dog::SetScale( Dog::States::BONE, 0.2f );
    } else if(w >= 1024 && w < 1152)
    {
        SCREEN.x = 1024;
        SCREEN.y = 576;
        bone_scale = 0.38f;

        Dog::SetScale( Dog::States::NEUTRAL, 0.2f );
        Dog::SetScale( Dog::States::BONE, 0.2f );
    } else if(w >= 1152 && w < 1280)
    {
        SCREEN.x = 1152;
        SCREEN.y = 648;
        bone_scale = 0.4f;

        Dog::SetScale( Dog::States::NEUTRAL, 0.19f );
        Dog::SetScale( Dog::States::BONE, 0.23f );

    } else if(w >= 1280)
    {
        SCREEN.x = 1280;
        SCREEN.y = 720;
        bone_scale = 0.42f;

        Dog::SetScale( Dog::States::NEUTRAL, 0.2f );
        Dog::SetScale( Dog::States::BONE, 0.25f );
    }

    Dog::SetFacePos( Dog::States::NEUTRAL, (Vector2){(float)SCREEN.x-(float)SCREEN.x*0.16f, -(float)SCREEN.y*0.02f} );
    Dog::SetFacePos( Dog::States::BONE, (Vector2){(float)SCREEN.x-(float)SCREEN.x*0.16f, 0} );

    SetWindowSize(SCREEN.x, SCREEN.y);
    Style::SetFontSizes(SCREEN);

    EM_ASM({ resizeCanvas($0, $1); }, SCREEN.x, SCREEN.y);
}
}


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
    EM_ASM({ init() });
    InitWindow(SCREEN.x, SCREEN.y, "Bones N Shit");

    Texture2D big_bone = LoadTexture("assets/bone-white.png");
    
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
        if(SCREEN.x < 368)
        {
            Dog::SetTexture(Dog::States::NEUTRAL, LoadTexture("assets/hubsi-neutral.png"));
            Dog::SetTexture(Dog::States::BONE, LoadTexture("assets/hubsi-bone.png"));
        } else if(SCREEN.x >= 368 && SCREEN.x < 512)
        {
            Dog::SetTexture(Dog::States::NEUTRAL, LoadTexture("assets/hubsi-neutral.png"));
            Dog::SetTexture(Dog::States::BONE, LoadTexture("assets/hubsi-bone.png"));
        } else if(SCREEN.x >= 512 && SCREEN.x < 640)
        {
            Dog::SetTexture(Dog::States::NEUTRAL, LoadTexture("assets/hubsi-neutral.png"));
            Dog::SetTexture(Dog::States::BONE, LoadTexture("assets/hubsi-bone.png"));
        } else if(SCREEN.x >= 640 && SCREEN.x < 768)
        {
            Dog::SetTexture(Dog::States::NEUTRAL, LoadTexture("assets/hubsi-neutral.png"));
            Dog::SetTexture(Dog::States::BONE, LoadTexture("assets/hubsi-bone.png"));
        } else if(SCREEN.x >= 768 && SCREEN.x < 896)
        {
            Dog::SetTexture(Dog::States::NEUTRAL, LoadTexture("assets/hubsi-neutral.png"));
            Dog::SetTexture(Dog::States::BONE, LoadTexture("assets/hubsi-bone.png"));
        } else if(SCREEN.x >= 896 && SCREEN.x < 1024)
        {
            Dog::SetTexture(Dog::States::NEUTRAL, LoadTexture("assets/hubsi-neutral.png"));
            Dog::SetTexture(Dog::States::BONE, LoadTexture("assets/hubsi-bone.png"));
        } else if(SCREEN.x >= 1024 && SCREEN.x < 1152)
        {
            Dog::SetTexture(Dog::States::NEUTRAL, LoadTexture("assets/hubsi-neutral.png"));
            Dog::SetTexture(Dog::States::BONE, LoadTexture("assets/hubsi-bone.png"));
        } else if(SCREEN.x >= 1152 && SCREEN.x < 1280)
        {
            Dog::SetTexture(Dog::States::NEUTRAL, LoadTexture("assets/hubsi-neutral.png"));
            Dog::SetTexture(Dog::States::BONE, LoadTexture("assets/hubsi-bone.png"));
        } else if(SCREEN.x >= 1280)
        {
            Dog::SetTexture(Dog::States::NEUTRAL, LoadTexture("assets/hubsi-neutral.png"));
            Dog::SetTexture(Dog::States::BONE, LoadTexture("assets/hubsi-bone.png"));
        }

        Collectable::Init();

        GameState::ChangeState(GameState::States::MAIN);
    });

    #ifdef PLATFORM_WEB
    GameState::SetStateCallback(GameState::States::MAIN, GameState::CallbackType::ENTER, [](){
        
    });
    #endif
    GameState::SetStateCallback(GameState::States::MAIN, GameState::CallbackType::UPDATE, [](){
        if(IsKeyPressed(KEY_P)) GameState::ChangeState( GameState::States::RESET );
    });

    // Reset game, then start playing
    GameState::SetStateCallback(GameState::States::RESET, GameState::CallbackType::UPDATE, [](){
        Dog::Reset();
        Dog::SetPos( (Vector2){ 0.5f, (float)GetScreenHeight()-Dog::GetSize().y-10 } );

        Collectable::Reset();

        GameState::ChangeState(GameState::States::PLAY);
    });


    GameState::SetStateCallback(GameState::States::PLAY, GameState::CallbackType::ENTER, [](){
        // EM_ASM({ setImg($0) }, 0);
    });
    // Update player and collectables when in play
    GameState::SetStateCallback(GameState::States::PLAY, GameState::CallbackType::UPDATE, [](){
        if(IsKeyPressed(KEY_P))     GameState::ChangeState( GameState::States::PAUSE );
        if(Dog::GetHealth() <= 0)   GameState::ChangeState( GameState::States::GAMEOVER );
        if(IsKeyPressed(KEY_SPACE))
        {
            switch(Dog::GetFaceState())
            {
            case Dog::States::NEUTRAL:
                Dog::SetFaceState( Dog::States::BONE );
                break;
            case Dog::States::BONE:
                Dog::SetFaceState( Dog::States::NEUTRAL );
                break;
            default: break;
            }
        }

        Dog::Update(IsKeyDown(KEY_D) - IsKeyDown(KEY_A));
        Collectable::Update((Vector2){(float)GetScreenWidth(), (float)GetScreenHeight()}, Dog::GetPos(), Dog::GetSize());
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
        UI::Text("BONES N SH*T", (Vector2){(float)GetScreenWidth()/2.0f, Style::VERY_LARGE_FONT*3.0f}, UI::Presets::TEXT_TITLE);

        if(UI::Button("Play", (Vector2){(float)GetScreenWidth()/2.0f, Style::VERY_LARGE_FONT*5.0f}, UI::Presets::BUTTON_MEDIUM))
            GameState::ChangeState( GameState::States::RESET );

        // Allow showing of exit button if not on web build
        #ifndef PLATFORM_WEB
        if(UI::Button("Exit", (Vector2){(float)GetScreenWidth()/2.0f, Style::VERY_LARGE_FONT*5.0f + Style::LARGE_FONT}, UI::Presets::BUTTON_LARGE))
            GameState::ChangeState( GameState::States::EXIT );
        #endif
    });

    // Draw dog, collectables, and stats when in play
    GameState::SetStateCallback(GameState::States::PLAY, GameState::CallbackType::DRAW, [&big_bone](){
        UI::SetMode( UI::Modes::CENTER );

        Dog::Draw();
        Collectable::Draw();
        
        // Grey side bars
        UI::SetMode( UI::Modes::TOP_LEFT );
        UI::SimpleRect( (Rectangle){0,0, GetScreenWidth()*0.15f,(float)GetScreenHeight()}, (Color){100, 100, 100, 255} );
        UI::SetMode( UI::Modes::TOP_RIGHT );
        UI::SimpleRect( (Rectangle){(float)GetScreenWidth(),0, GetScreenWidth()*0.15f,(float)GetScreenHeight()}, (Color){100, 100, 100, 255} );

        // Score bone
        UI::SetMode( UI::Modes::TOP_RIGHT );
        // UI::SimpleRect( (Rectangle){ (float)GetScreenWidth()-(float)GetScreenWidth()*0.18f, (float)Style::LARGE_FONT, (float)GetScreenWidth()*0.1f, (float)Style::LARGE_FONT }, (Color){255,255,255,255} );
        DrawTextureEx( big_bone, (Vector2){(float)GetScreenWidth()-(float)GetScreenWidth()*0.33f, -(float)GetScreenHeight()*0.07f}, 0.0f, bone_scale, (Color){255,255,255,255} );

        // Score and health
        UI::SetMode( UI::Modes::CENTER );
        UI::Text(std::to_string(Dog::GetScore()).c_str(), (Vector2){(float)GetScreenWidth()-(float)GetScreenWidth()*0.24f, (float)Style::LARGE_FONT*1.55f}, UI::Presets::TEXT_LARGE, (Color){0,0,0,255});
        
        for(int i=0; i<Dog::GetHealth(); i++)
        {
            // UI::Text(std::to_string(Dog::GetHealth()).c_str(), (Vector2){(float)GetScreenWidth()-(float)GetScreenWidth()*0.05f, (float)Style::SMALL_FONT}, UI::Presets::TEXT_SMALL, (Color){255,0,0,255});
            UI::SimpleRect( (Rectangle){ (float)GetScreenWidth()-(float)GetScreenWidth()*0.075f, GetScreenHeight()/2.5f + i*GetScreenHeight()*0.15f, GetScreenWidth()*0.05f, GetScreenWidth()*0.05f }, (Color){255, 0, 0, 255} );
        }

        UI::Face( Dog::GetTexture(Dog::GetFaceState()), Dog::GetFacePos(Dog::GetFaceState()), Dog::GetScale(Dog::GetFaceState()) );
        
        UI::SetMode( UI::Modes::CENTER_LEFT );
        // UI::Text(std::to_string(Collectable::GetVals(0)/100000.0f).c_str(), (Vector2){(float)GetScreenWidth()/1.5f, 40}, UI::Presets::TEXT_SMALL);
        // UI::Text(std::to_string(Collectable::GetVals(1)/100000.0f).c_str(), (Vector2){(float)GetScreenWidth()/1.5f, 80}, UI::Presets::TEXT_SMALL);
        // UI::Text(std::to_string(Collectable::GetVals(2)/100000.0f).c_str(), (Vector2){(float)GetScreenWidth()/1.5f, 120}, UI::Presets::TEXT_SMALL);

        UI::SetMode( UI::Modes::TOP_LEFT );
        if(UI::Button("| |", (Vector2){0, (float)Style::SMALL_FONT}, UI::Presets::BUTTON_SMALL))
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
