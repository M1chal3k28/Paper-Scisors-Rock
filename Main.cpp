
#include <Config.hpp>
#include <Scenes.hpp>

int main() {
    // Initialization
    //--------------------------------------------------------------------------------------
    InitWindow(GetMonitorWidth(GetCurrentMonitor()), GetMonitorHeight(GetCurrentMonitor()), "Rock-Paper-Scissors");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second

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