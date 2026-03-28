#include <UniEngine/UniEngine.hpp>

int main() {
    Game::initGame();

    Game::Window window = Game::createWindowAndMakeReady(800, 600, "Pencere");

    Game::createMainPipeline(nullptr, nullptr);

    Game::Color color;
    Game::createColorRGB(color, 255, 0, 0);

    Game::objectData2D plane = Game::createObject2D(
        Objects::plane,
        Game::TRIANGLES,
        color,
        { 0, 0, 0 }
    );

    Game::setBackgroundColor(0, 255, 255);

    while (Game::getWindowStatus(window) == Game::ACTIVE) {
        Game::preloadRate();

        Game::drawObject2D(plane, Game::TRIANGLES);

        Game::makeNewRate(window);
    }

    Game::byeToAll(window);
    
    return 0;
}
