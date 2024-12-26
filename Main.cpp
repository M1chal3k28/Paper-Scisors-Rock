
#include <Config.hpp>
#include <scenes/Scenes.hpp>

int main() {
    // Initialization
    //--------------------------------------------------------------------------------------
    SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(GetMonitorWidth(GetCurrentMonitor()), GetMonitorHeight(GetCurrentMonitor()), "Rock-Paper-Scissors");
    SetWindowState(FLAG_FULLSCREEN_MODE);              // Set our game to run at 60 frames-per-second

    RESOURCE_MANAGER.loadTexture("backgroundSprites", "assets/backgroundSprite.png");

    SCENE_MANAGER.pushScene( SceneType::INTRO );
    //--------------------------------------------------------------------------------------

    // Main game loop
    while ( !WindowShouldClose() )    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        SCENE_MANAGER.update( GetFrameTime() );
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
            ClearBackground(RAYWHITE);

            SCENE_MANAGER.draw();
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------
    return 0;
}