#include <scenes/ServerListScene.hpp>
#include <scenes/SceneManager.hpp>

ServerListScene::ServerListScene() : currentPage(0) {
    this->prepareResources();

    // Create info
    this->info = std::make_unique<Text>( "Searching", Vector2{0, 0}, Vector2{(float)GetScreenWidth() / 2, 200}, "minecraft-font", TITLE_SIZE, TEXT_SPACING, MY_ORANGE);
    this->info->tCenterOffset();

    // Create back button
    this->backButton = std::make_unique<TextButton>(
        Vector2{ (float)GetScreenWidth() / 2, (float)GetScreenHeight() - 200 }, 
        (Vector2)MENU_BUTTON_SIZE, 
        (Vector2)MENU_BUTTON_SHEET_OFFSET,
        "Back", 
        "button", 
        "minecraft-font",
        [this]() {
            // This is a function that will be called when the button is clicked
            // Go back to main menu scene
            SCENE_MANAGER.popScene();
        }
    );

    // Start looking for a servers
    // Set finished to false
    this->isFinished = false;
    this->getServersThread = std::async(std::launch::async, &ServerListScene::getAvailableServers, this);
}

ServerListScene::~ServerListScene() {
    this->cleanUp();
}

void ServerListScene::getAvailableServers() {
    // Start looking for a servers
    this->broadcastSocket.searchOnce();
    this->discoveredServers = this->broadcastSocket.getResults();

    // Create server buttonsghfh
    this->pages.clear();
    this->pages.resize(1);

    // padding
    static int padding = 2;

    // Create server buttons
        int col = 0;
        int maxCol = (int)GetScreenWidth() / (Vector2)MENU_BUTTON_SIZE.x;
        int row = 0;
        int maxRow = ((int)GetScreenHeight() - 500) / (Vector2)MENU_BUTTON_SIZE.y;
        int page = 0;
    // Starting pos
        Vector2 startingPos = Vector2{ (float) (Vector2)MENU_BUTTON_SIZE.x / 2 + (GetScreenWidth() - maxCol * (Vector2)MENU_BUTTON_SIZE.x) / 2 - padding * maxCol, (float)300 + (Vector2)MENU_BUTTON_SIZE.y / 2 };
    // Create button for each server
    for (auto& server : this->discoveredServers) {
        // Create button
        // Calc position
        if (col >= maxCol) {
            col = 0;
            row++;
            if (row >= maxRow) {
                row = 0;
                page++;
                this->pages.emplace_back(std::vector<std::unique_ptr<TextButton>>());
            }
        }

        // Create button
        // And add to proper page
        this->pages[page].emplace_back(
            std::make_unique<TextButton>(
                Vector2{ startingPos.x + col * (Vector2)MENU_BUTTON_SIZE.x + (padding * col), startingPos.y + row * (Vector2)MENU_BUTTON_SIZE.y + (padding * row) },
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
        
        col++;
    }

    // Text 
    this->info->tSetText("Found " + std::to_string(this->discoveredServers.size()) + " servers");

    this->isFinished = true;
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
    // Join thread if finished
    if (this->isFinished) {
        if(this->getServersThread.valid())
            this->getServersThread.get();
    }

    // Set cursor to default
    SetMouseCursor(MOUSE_CURSOR_DEFAULT);

    // Update buttons
    for (auto& page : pages)
        for (auto& server : page) 
            server->update(deltaTime);

    // Update back button
    this->backButton->update(deltaTime);

    M_BG.update(deltaTime);
}

void ServerListScene::draw() const {
    M_BG.draw();

    // Draw back button
    this->backButton->draw();

    // Draw title
    if (this->info)
        this->info->draw();

    // Draw servers
    if (this->currentPage < this->pages.size())
        for (auto& server : this->pages[this->currentPage])
            server->draw();
}

void ServerListScene::cleanUp() {
    // Join thread
    if (this->getServersThread.valid())
        this->getServersThread.get();
}
