#include <scenes/ServerListScene.hpp>

ServerListScene::ServerListScene() : currentPage(0) {
    this->prepareResources();

    // Start looking for a servers
    this->broadcastSocket.searchOnce();
    this->discoveredServers = this->broadcastSocket.getResults();

    this->pages.resize(1);

    // Create server buttons
        int col = 0;
        int maxCol = (int)GetScreenWidth() / (Vector2)MENU_BUTTON_SIZE.x;
        int row = 0;
        int maxRow = (int)GetScreenHeight() - 500 / (Vector2)MENU_BUTTON_SIZE.y;
        int page = 0;
    // Starting pos
        Vector2 startingPos = Vector2{ (float)(Vector2)MENU_BUTTON_SIZE.x, (float)300 + (Vector2)MENU_BUTTON_SIZE.y / 2 };
    for (auto& server : this->discoveredServers) {
        // Create button
        // Calc position
        if (col >= maxCol) {
            col = 0;
            row++;
            if (row >= maxRow) {
                row = 0;
                page++;
            }
        }

        // Create button
        // And add to proper page
        this->pages[page].emplace_back(
            std::make_unique<TextButton>(
                Vector2{ startingPos.x + col * (Vector2)MENU_BUTTON_SIZE.x, startingPos.y + row * (Vector2)MENU_BUTTON_SIZE.y },
                (Vector2)MENU_BUTTON_SIZE,
                (Vector2)MENU_BUTTON_SHEET_OFFSET,
                server.first,
                "button",
                "minecraft-font",
                [this]() {
                    // This is a function that will be called when the button is clicked
                }
            )
        );
    }

    // Text 
    this->info = std::make_unique<Text>( "Available Servers", Vector2{0, 0}, Vector2{(float)GetScreenWidth() / 2, 200}, "minecraft-font", TITLE_SIZE, TEXT_SPACING, MY_ORANGE);
    this->info->tCenterOffset();
}

ServerListScene::~ServerListScene() {
    this->cleanUp();
}

void ServerListScene::prepareResources() {
    // Init background if not done already
    M_BG;
    
    // Load button png sprite sheet
    RESOURCE_MANAGER.loadTexture("button", "assets/buttons.png");
    // Load font
    RESOURCE_MANAGER.loadFont("minecraft-font", "assets/Minecraft.ttf");
}

void ServerListScene::update(float deltaTime) {
    // Set cursor to default
    SetMouseCursor(MOUSE_CURSOR_DEFAULT);

    // Update buttons
    for (auto& page : pages)
        for (auto& server : page) 
            server->update(deltaTime);

    M_BG.update(deltaTime);
}

void ServerListScene::draw() const {
    M_BG.draw();

    // Draw title
    this->info->draw();

    // Draw servers
    for (auto& server : this->pages[this->currentPage])
        server->draw();
}

void ServerListScene::cleanUp() {
}
