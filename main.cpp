
#include <Config.hpp>
#include <scenes/Scenes.hpp>

int main() {
    // Initialization
    //--------------------------------------------------------------------------------------
    GAME; // <- This is the game object (initialize)
        // Init random number generator
        srand(time(NULL));
        //----------------------------------------------------------------------------------

    // Set config flag to enable V-Sync
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    SetConfigFlags(FLAG_VSYNC_HINT);
    // InitWindow(GetMonitorWidth(GetCurrentMonitor()), GetMonitorHeight(GetCurrentMonitor()), "Rock-Paper-Scissors");    // Set full screen
    InitWindow(1920, 1080, "Rock-Paper-Scissors");    // Set full screen
    // SetWindowState(FLAG_FULLSCREEN_MODE);

    // Initialize scenes
    SCENE_MANAGER.pushScene( SceneType::INTRO );
    //--------------------------------------------------------------------------------------

    // Main game loop
    while ( !WindowShouldClose() )    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // Update currentScene
        try {
            SCENE_MANAGER.update( GetFrameTime() );
        } catch (const std::exception& e) {
            MessageBox((HWND)GetWindowHandle(), e.what(), "Error", MB_OK | MB_ICONERROR );
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
            ClearBackground(RAYWHITE);
            // Draw current scene
            SCENE_MANAGER.draw();
        EndDrawing();
        //----------------------------------------------------------------------------------

        DrawFPS(0, 0);
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
