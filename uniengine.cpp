#include <UniEngine.hpp>

int main() {
    Game::initGame();

    Game::Window window = Game::createWindowAndMakeReady(800, 600, "Pencere");

    Game::createMainPipeline(nullptr, nullptr);

    while (Game::getWindowStatus(window) == Game::CLOSING) {
        Game::makeNewRate(window);
    }

    Game::byeToAll(window);
    
    return 0;
}
