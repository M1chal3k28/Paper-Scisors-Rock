#include <scenes/SceneManager.hpp>
#include <scenes/Scenes.hpp>

SceneManager &SceneManager::getInstance() {
    static SceneManager instance;
    return instance;
}

SceneManager::~SceneManager() {
    while(!this->scenesStack.empty()) this->popScene();
}

void SceneManager::pushScene(const SceneType::Value &scene)  {
    switch ( scene ) {
        case SceneType::INTRO:
            scenesStack.push( std::make_unique<IntroScene>() );
            break;
        case SceneType::SETUP:
            scenesStack.push( std::make_unique<SetupScene>() );
            break;
        case SceneType::HOST_LOBBY:
            scenesStack.push( std::make_unique<HostLobbyScene>() );
            break;
        // case SceneType::SERVER_SELECTION:
        //     scenesStack.push( std::make_unique<ServerListScene>() );
        //     break;
        case SceneType::GAME_SCENE:
            scenesStack.push( std::make_unique<GameScene>() );
            break;
        case SceneType::LOADING_SCENE:
            scenesStack.push( std::make_unique<GameLoadingScene>() );
            break;
    }
}

void SceneManager::pushScene( std::unique_ptr<Scene> scene ) {
    scenesStack.push( std::move( scene ) );
}

void SceneManager::popScene() {
    if ( !scenesStack.empty() )
        scenesStack.pop();
}

void SceneManager::update(float deltaTime) {
    if ( !scenesStack.empty() )
        scenesStack.top()->update( deltaTime );
}

void SceneManager::draw() const {
    if ( !scenesStack.empty() )
        scenesStack.top()->draw();
}
