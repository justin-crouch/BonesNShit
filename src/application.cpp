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

float gameover_counter  = 0.0f;
int text_count          = 0;
int score_offset        = 0;
float score_counter     = 0.0f;

float base_bone_scale = 0.42f;
float base_heart_scale = 0.21f;
float bone_scale    = base_bone_scale;
float heart_scale   = base_heart_scale;

float base_neutral  = 0.2f;
float base_bone     = 0.25f;
float base_disgust  = 0.16f;
float base_yawn     = 0.20f;

extern "C" {
void SetScreen(int w, int h)
{
    float new_neutral, new_bone, new_disgust, new_yawn;
    // EM_ASM({ alert($0); alert($1); }, w, h);
    if(w < h)
    {
        SCREEN.x = 207;
        SCREEN.y = 368;

        bone_scale = base_bone_scale - 0.32f;
        heart_scale = base_heart_scale - 0.16f;
        
        new_neutral = base_neutral - 0.16f;
        new_bone    = base_bone - 0.16f;
        new_disgust = base_disgust - 0.15f;
        new_yawn    = base_yawn - 0.16f;
    }else if(h < 368)
    {
        // SCREEN.x = 500;
        // SCREEN.y = 500*0.5652;
        SCREEN.x = 360;
        SCREEN.y = 207;

        bone_scale = base_bone_scale - 0.32f;
        heart_scale = base_heart_scale - 0.16f;
        
        new_neutral = base_neutral - 0.16f;
        new_bone    = base_bone - 0.16f;
        new_disgust = base_disgust - 0.15f;
        new_yawn    = base_yawn - 0.16f;
    } else if(w >= 368 && w < 512)
    {
        SCREEN.x = 368;
        SCREEN.y = 207;

        bone_scale = base_bone_scale - 0.28f;
        heart_scale = base_heart_scale - 0.14f;
        
        new_neutral = base_neutral - 0.14f;
        new_bone    = base_bone - 0.14f;
        new_disgust = base_disgust - 0.14f;
        new_yawn    = base_yawn - 0.14f;
    } else if(w >= 512 && w < 640)
    {
        SCREEN.x = 512;
        SCREEN.y = 288;

        bone_scale = base_bone_scale - 0.24f;
        heart_scale = base_heart_scale - 0.12f;
        
        new_neutral = base_neutral - 0.12f;
        new_bone    = base_bone - 0.12f;
        new_disgust = base_disgust - 0.12f;
        new_yawn    = base_yawn - 0.12f;
    } else if(w >= 640 && w < 768)
    {
        SCREEN.x = 640;
        SCREEN.y = 360;

        bone_scale = base_bone_scale - 0.20f;
        heart_scale = base_heart_scale - 0.10f;
        
        new_neutral = base_neutral - 0.10f;
        new_bone    = base_bone - 0.10f;
        new_disgust = base_disgust - 0.10f;
        new_yawn    = base_yawn - 0.10f;
    } else if(w >= 768 && w < 896)
    {
        SCREEN.x = 768;
        SCREEN.y = 432;

        bone_scale = base_bone_scale - 0.16f;
        heart_scale = base_heart_scale - 0.08f;
        
        new_neutral = base_neutral - 0.08f;
        new_bone    = base_bone - 0.08f;
        new_disgust = base_disgust - 0.08f;
        new_yawn    = base_yawn - 0.08f;
    } else if(w >= 896 && w < 1024)
    {
        SCREEN.x = 896;
        SCREEN.y = 504;

        bone_scale = base_bone_scale - 0.12f;
        heart_scale = base_heart_scale - 0.06f;

        new_neutral = base_neutral - 0.06f;
        new_bone    = base_bone - 0.06f;
        new_disgust = base_disgust - 0.06f;
        new_yawn    = base_yawn - 0.06f;
    } else if(w >= 1024 && w < 1152)
    {
        SCREEN.x = 1024;
        SCREEN.y = 576;

        bone_scale = base_bone_scale - 0.08f;
        heart_scale = base_heart_scale - 0.04f;

        new_neutral = base_neutral - 0.04f;
        new_bone    = base_bone - 0.04f;
        new_disgust = base_disgust - 0.04f;
        new_yawn    = base_yawn - 0.04f;

    } else if(w >= 1152 && w < 1280)
    {
        SCREEN.x = 1152;
        SCREEN.y = 648;

        bone_scale = base_bone_scale - 0.04f;
        heart_scale = base_heart_scale - 0.02f;

        new_neutral = base_neutral - 0.02f;
        new_bone    = base_bone - 0.02f;
        new_disgust = base_disgust - 0.02f;
        new_yawn    = base_yawn - 0.02f;

    } else if(w >= 1280)
    {
        SCREEN.x = 1280;
        SCREEN.y = 720;

        bone_scale = base_bone_scale;
        heart_scale = base_heart_scale;

        new_neutral = base_neutral;
        new_bone    = base_bone;
        new_disgust = base_disgust;
        new_yawn    = base_yawn;
    }

    Dog::SetScale( Dog::States::NEUTRAL, new_neutral );
    Dog::SetScale( Dog::States::BONE, new_bone );
    Dog::SetScale( Dog::States::DISGUST, new_disgust );
    Dog::SetScale( Dog::States::YAWN, new_yawn );

    Dog::SetFacePos( Dog::States::NEUTRAL, (Vector2){(float)SCREEN.x-(float)SCREEN.x*0.16f, -(float)SCREEN.y*0.02f} );
    Dog::SetFacePos( Dog::States::BONE, (Vector2){(float)SCREEN.x-(float)SCREEN.x*0.16f, 0} );
    Dog::SetFacePos( Dog::States::DISGUST, (Vector2){(float)SCREEN.x-(float)SCREEN.x*0.16f, (float)SCREEN.y*0.02f} );
    Dog::SetFacePos( Dog::States::YAWN, (Vector2){(float)SCREEN.x-(float)SCREEN.x*0.16f, 0} );

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
    Texture2D heart = LoadTexture("assets/heart.png");
    
    // Set what certain collectables do
    Collectable::SetCallable(Collectable::Type::NORMAL  , [](){
        Dog::SetScore( Dog::GetScore()+1 );

        if(GetRandomValue(0, 100) < 20) Dog::SetFaceState( Dog::States::BONE, 1.0f );
        else if(GetRandomValue(0, 100) < 50) Dog::SetFaceState( Dog::States::YAWN, 1.0f );
    });
    Collectable::SetCallable(Collectable::Type::GOLDEN  , [](){
        Dog::SetScore( Dog::GetScore()+GOLDEN_AMNT );

        if(GetRandomValue(0, 100) < 20) Dog::SetFaceState( Dog::States::BONE, 1.0f );
        else if(GetRandomValue(0, 100) < 50) Dog::SetFaceState( Dog::States::YAWN, 1.0f );
    });
    Collectable::SetCallable(Collectable::Type::SHIT    , [](){
        Dog::SetHealth( Dog::GetHealth()-1 );
        Dog::SetFaceState( Dog::States::DISGUST, 1.0f );
    });

    // Update callbacks
    //--------------------------------------------------------------------------------------
    // Allow closing window if not on web build
    #ifndef PLATFORM_WEB
    GameState::SetStateCallback(GameState::States::EXIT, GameState::CallbackType::ENTER, [](){
        CloseWindow();
    });
    #endif

    GameState::SetStateCallback(GameState::States::INIT, GameState::CallbackType::UPDATE, [](){
        Dog::SetTexture(Dog::States::NEUTRAL, LoadTexture("assets/hubsi-neutral.png"));
        Dog::SetTexture(Dog::States::BONE, LoadTexture("assets/hubsi-bone.png"));
        Dog::SetTexture(Dog::States::DISGUST, LoadTexture("assets/hubsi-disgust.png"));
        Dog::SetTexture(Dog::States::YAWN, LoadTexture("assets/hubsi-yawn.png"));

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
                Dog::SetFaceState( Dog::States::BONE, 1.0f );
                break;
            case Dog::States::BONE:
                Dog::SetFaceState( Dog::States::DISGUST, 1.0f );
                break;
            case Dog::States::DISGUST:
                Dog::SetFaceState( Dog::States::YAWN, 1.0f );
                break;
            case Dog::States::YAWN:
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
    GameState::SetStateCallback(GameState::States::GAMEOVER, GameState::CallbackType::ENTER, [](){
        gameover_counter = 0;
        text_count = 0;
        score_offset = Dog::GetScore();
        Dog::SetFaceState( Dog::States::DISGUST );
    });
    // Update for GAMEOVER scene
    GameState::SetStateCallback(GameState::States::GAMEOVER, GameState::CallbackType::UPDATE, [](){
        gameover_counter += GetFrameTime();
        if(gameover_counter > 100.0f) gameover_counter = 100.0f;
        if(gameover_counter > 2.0f && (int)(gameover_counter*100)%4 == 0) text_count += 1;
        if(text_count >= 30)
        {
            text_count = 30;
            score_counter += GetFrameTime();
            if(score_counter >= 0.005f)
            {
                score_counter = 0.0f;
                score_offset -= 1;
            }
            if(score_offset < 0) score_offset = 0;
        }

        if(IsKeyPressed(KEY_R)) GameState::ChangeState( GameState::States::RESET );
    });
    //--------------------------------------------------------------------------------------

    // Draw callbacks
    //--------------------------------------------------------------------------------------
    // Draw main menu
    GameState::SetStateCallback(GameState::States::MAIN, GameState::CallbackType::DRAW, [](){
        // UI::Text("BONES N SH*T", (Vector2){(float)GetScreenWidth()/2.0f, Style::VERY_LARGE_FONT*3.0f}, UI::Presets::TEXT_TITLE);
        for(int i=0; i<100; i++)
        {
            float x = Remap(i, 0, 99, (float)GetScreenWidth()/2.0f - Style::VERY_LARGE_FONT*5.0f, (float)GetScreenWidth()/2.0f + Style::VERY_LARGE_FONT*4.0f);
            float y = Style::VERY_LARGE_FONT*5.0f;
            DrawCircleGradient(x, y, Style::VERY_LARGE_FONT*0.6f, (Color){107,190,172,6}, (Color){107,190,172,0});
        }

        UI::SetMode( UI::Modes::CENTER_RIGHT );
        UI::Text("BONES", (Vector2){(float)GetScreenWidth()/2.0f - Style::VERY_LARGE_FONT, Style::VERY_LARGE_FONT*5.0f}, UI::Presets::TEXT_TITLE, (Color){255, 255, 255, 255});
        
        UI::SetMode( UI::Modes::CENTER );
        UI::Text("N", (Vector2){(float)GetScreenWidth()/2.0f, Style::VERY_LARGE_FONT*5.0f}, UI::Presets::TEXT_TITLE, (Color){99, 167, 238, 255});
        
        UI::SetMode( UI::Modes::CENTER_LEFT );
        UI::Text("SH*T", (Vector2){(float)GetScreenWidth()/2.0f + Style::VERY_LARGE_FONT, Style::VERY_LARGE_FONT*5.0f}, UI::Presets::TEXT_TITLE, (Color){128, 105, 55, 255});


        UI::SetMode( UI::Modes::CENTER );
        if(UI::Button("Play", (Vector2){(float)GetScreenWidth()/2.0f, Style::VERY_LARGE_FONT*7.0f}, UI::Presets::BUTTON_MEDIUM))
            GameState::ChangeState( GameState::States::RESET );

        // Allow showing of exit button if not on web build
        #ifndef PLATFORM_WEB
        if(UI::Button("Exit", (Vector2){(float)GetScreenWidth()/2.0f, Style::VERY_LARGE_FONT*8.0f + Style::LARGE_FONT}, UI::Presets::BUTTON_LARGE, (Color){255, 0, 0, 255}))
            GameState::ChangeState( GameState::States::EXIT );
        #endif
    });

    // Draw dog, collectables, and stats when in play
    GameState::SetStateCallback(GameState::States::PLAY, GameState::CallbackType::DRAW, [&big_bone, &heart](){
        UI::SetMode( UI::Modes::CENTER );

        Dog::Draw();
        Collectable::Draw();
        
        // Grey side bars
        UI::SetMode( UI::Modes::TOP_LEFT );
        // UI::SimpleRect( (Rectangle){0,0, GetScreenWidth()*0.15f,(float)GetScreenHeight()}, (Color){191, 140, 197, 255} );
        UI::SetMode( UI::Modes::TOP_RIGHT );
        UI::SimpleRect( (Rectangle){(float)GetScreenWidth(),0, GetScreenWidth()*0.15f,(float)GetScreenHeight()}, (Color){191, 140, 197, 255} );

        // Score bone
        UI::SetMode( UI::Modes::TOP_RIGHT );
        // UI::SimpleRect( (Rectangle){ (float)GetScreenWidth()-(float)GetScreenWidth()*0.18f, (float)Style::LARGE_FONT, (float)GetScreenWidth()*0.1f, (float)Style::LARGE_FONT }, (Color){255,255,255,255} );
        DrawTextureEx( big_bone, (Vector2){(float)GetScreenWidth()-(float)GetScreenWidth()*0.15f-big_bone.width*bone_scale, -big_bone.height*bone_scale*0.25f}, 0.0f, bone_scale, (Color){255,255,255,255} );

        // Score and health
        UI::SetMode( UI::Modes::CENTER );
        UI::Text(std::to_string(Dog::GetScore()).c_str(), (Vector2){(float)GetScreenWidth()-(float)GetScreenWidth()*0.24f, big_bone.height*bone_scale*0.28f}, UI::Presets::TEXT_LARGE, (Color){0,0,0,255});
        
        for(int i=0; i<Dog::GetHealth(); i++)
            DrawTextureEx( heart, (Vector2){(float)GetScreenWidth()-(float)GetScreenWidth()*0.11f, GetScreenHeight()/2.5f + i*GetScreenHeight()*0.15f}, 0.0f, heart_scale, (Color){255,255,255,255} );

        UI::Face( Dog::GetTexture(Dog::GetFaceState()), Dog::GetFacePos(Dog::GetFaceState()), Dog::GetScale(Dog::GetFaceState()) );
        
        UI::SetMode( UI::Modes::TOP_LEFT );
        if(UI::Button("| |", (Vector2){0, (float)Style::SMALL_FONT}, UI::Presets::BUTTON_SMALL))
            GameState::ChangeState( GameState::States::PAUSE );
    });

    // Draw dog, collectables, and stats when paused
    GameState::SetStateCallback(GameState::States::PAUSE, GameState::CallbackType::DRAW, [&big_bone, &heart](){
        UI::SetMode( UI::Modes::CENTER );

        Dog::Draw();
        Collectable::Draw();

        // Grey side bars
        UI::SetMode( UI::Modes::TOP_LEFT );
        // UI::SimpleRect( (Rectangle){0,0, GetScreenWidth()*0.15f,(float)GetScreenHeight()}, (Color){191, 140, 197, 255} );
        UI::SetMode( UI::Modes::TOP_RIGHT );
        UI::SimpleRect( (Rectangle){(float)GetScreenWidth(),0, GetScreenWidth()*0.15f,(float)GetScreenHeight()}, (Color){191, 140, 197, 255} );

        // Score bone
        UI::SetMode( UI::Modes::TOP_RIGHT );
        // UI::SimpleRect( (Rectangle){ (float)GetScreenWidth()-(float)GetScreenWidth()*0.18f, (float)Style::LARGE_FONT, (float)GetScreenWidth()*0.1f, (float)Style::LARGE_FONT }, (Color){255,255,255,255} );
        DrawTextureEx( big_bone, (Vector2){(float)GetScreenWidth()-(float)GetScreenWidth()*0.15f-big_bone.width*bone_scale, -big_bone.height*bone_scale*0.25f}, 0.0f, bone_scale, (Color){255,255,255,255} );

        // Score and health
        UI::SetMode( UI::Modes::CENTER );
        UI::Text(std::to_string(Dog::GetScore()).c_str(), (Vector2){(float)GetScreenWidth()-(float)GetScreenWidth()*0.24f, big_bone.height*bone_scale*0.28f}, UI::Presets::TEXT_LARGE, (Color){0,0,0,255});
        
        for(int i=0; i<Dog::GetHealth(); i++)
            DrawTextureEx( heart, (Vector2){(float)GetScreenWidth()-(float)GetScreenWidth()*0.11f, GetScreenHeight()/2.5f + i*GetScreenHeight()*0.15f}, 0.0f, heart_scale, (Color){255,255,255,255} );

        UI::Face( Dog::GetTexture(Dog::GetFaceState()), Dog::GetFacePos(Dog::GetFaceState()), Dog::GetScale(Dog::GetFaceState()) );
        
        UI::SimpleRect((Rectangle){(float)GetScreenWidth()/2.0f, (float)GetScreenHeight()/2.0f, (float)GetScreenWidth()-10, (float)GetScreenHeight()-10}, (Color){0, 0, 0, 200});
        UI::Text("PAUSED", (Vector2){(float)GetScreenWidth()/2.0f, Style::VERY_LARGE_FONT*4.0f}, UI::Presets::TEXT_TITLE, (Color){255,255,255,255});
        
        if(UI::Button("Resume", (Vector2){(float)GetScreenWidth()/2.0f, (float)GetScreenHeight()/2.0f + Style::VERY_LARGE_FONT}, UI::Presets::BUTTON_MEDIUM))
            GameState::ChangeState( GameState::States::PLAY );
        if(UI::Button("Exit", (Vector2){(float)GetScreenWidth()/2.0f, (float)GetScreenHeight()/2.0f + Style::VERY_LARGE_FONT*2.0f}, UI::Presets::BUTTON_MEDIUM, (Color){255, 0, 0, 255}))
            GameState::ChangeState( GameState::States::MAIN );
    });

    // Draw gameover scene
    GameState::SetStateCallback(GameState::States::GAMEOVER, GameState::CallbackType::DRAW, [&big_bone](){
        if(gameover_counter < 1.0f){
            UI::SetMode( UI::Modes::CENTER );

            Dog::Draw();
            Collectable::Draw();
            
            // Grey side bars
            UI::SetMode( UI::Modes::TOP_LEFT );
            // UI::SimpleRect( (Rectangle){0,0, GetScreenWidth()*0.15f,(float)GetScreenHeight()}, (Color){191, 140, 197, 255} );
            UI::SetMode( UI::Modes::TOP_RIGHT );
            UI::SimpleRect( (Rectangle){(float)GetScreenWidth(),0, GetScreenWidth()*0.15f,(float)GetScreenHeight()}, (Color){191, 140, 197, 255} );

            // Score bone
            UI::SetMode( UI::Modes::TOP_RIGHT );
            DrawTextureEx( big_bone, (Vector2){(float)GetScreenWidth()-(float)GetScreenWidth()*0.15f-big_bone.width*bone_scale, -(float)GetScreenHeight()*0.07f}, 0.0f, bone_scale, (Color){255,255,255,255} );

            // Score and health
            UI::SetMode( UI::Modes::CENTER );
            UI::Text(std::to_string(Dog::GetScore()).c_str(), (Vector2){(float)GetScreenWidth()-(float)GetScreenWidth()*0.24f, (float)Style::LARGE_FONT*1.55f}, UI::Presets::TEXT_LARGE, (Color){0,0,0,255});
            
            UI::Face( Dog::GetTexture(Dog::GetFaceState()), Dog::GetFacePos(Dog::GetFaceState()), Dog::GetScale(Dog::GetFaceState()) );
            return;
        }

        for(int i=0; i<100; i++)
        {
            float x = Remap(i, 0, 99, (float)GetScreenWidth()/2.0f - Style::VERY_LARGE_FONT*4.0f, (float)GetScreenWidth()/2.0f + Style::VERY_LARGE_FONT*4.0f);
            float y = Style::VERY_LARGE_FONT*4.0f;
            DrawCircleGradient(x, y, Style::VERY_LARGE_FONT*0.6f, (Color){107,190,172,6}, (Color){107,190,172,0});
        }

        UI::SetMode( UI::Modes::CENTER );
        UI::Text("GAMEOVER", (Vector2){(float)GetScreenWidth()/2.0f, Style::VERY_LARGE_FONT*4.0f}, UI::Presets::TEXT_TITLE, (Color){251,101,175,255});

        UI::SetMode( UI::Modes::CENTER_RIGHT );
        std::string s = "Bones Collected: ";
        UI::Text(s.substr(0, text_count).c_str(), (Vector2){(float)GetScreenWidth()/2.0f+Style::VERY_LARGE_FONT*3.0f, Style::VERY_LARGE_FONT*6.0f}, UI::Presets::TEXT_LARGE);

        UI::SetMode( UI::Modes::CENTER_LEFT );
        if(score_offset != Dog::GetScore())
            UI::Text(std::to_string(Dog::GetScore() - score_offset).c_str(), (Vector2){(float)GetScreenWidth()/2.0f+Style::VERY_LARGE_FONT*3.0f, Style::VERY_LARGE_FONT*6.0f}, UI::Presets::TEXT_MEDIUM);

        UI::SetMode( UI::Modes::CENTER );
        if(UI::Button("Replay", (Vector2){(float)GetScreenWidth()/2.0f, (float)GetScreenHeight()/2.0f+Style::VERY_LARGE_FONT*2.0f}, UI::Presets::BUTTON_MEDIUM))
            GameState::ChangeState( GameState::States::RESET );
        if(UI::Button("Exit", (Vector2){(float)GetScreenWidth()/2.0f, (float)GetScreenHeight()/2.0f+Style::VERY_LARGE_FONT*3.0f}, UI::Presets::BUTTON_MEDIUM, (Color){255, 0, 0, 255}))
            GameState::ChangeState( GameState::States::MAIN );
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

        ClearBackground((Color){ 107, 190, 172, 255 });

        // BeginMode2D(viewport);

        GameState::Draw();
        
        // EndMode2D();

    EndDrawing();
    //----------------------------------------------------------------------------------
}
