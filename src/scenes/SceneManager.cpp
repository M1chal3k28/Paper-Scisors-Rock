#include <SceneManager.hpp>

SceneManager &SceneManager::getInstance() {
    static SceneManager instance;
    return instance;
}

void SceneManager::pushScene(const SceneType::Value &scene)  {
    switch ( scene ) {
        case SceneType::INTRO:
            scenesStack.push( std::make_unique<IntroScene>() );
            break;
    }
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
