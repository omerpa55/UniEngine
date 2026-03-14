#include <UniEngine.hpp>

int main() {
    Game::initGame();

    Game::Window window = Game::createWindowAndMakeReady(800, 600, "Pencere");

    Game::createMainPipeline(nullptr, nullptr);

    Game::Color color;
    Game::createColorRGB(color, 255, 0, 0);

    Game::objectData plane = Game::createObject(Objects::plane, Game::TRIANGLES, color);

    while (Game::getWindowStatus(window) == Game::ACTIVE) {
        Game::preloadRate();

        Game::drawObject(plane, Game::TRIANGLES);

        Game::makeNewRate(window);
    }

    Game::byeToAll(window);
    
    return 0;
}
