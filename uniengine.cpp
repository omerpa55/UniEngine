#include <UniEngine/UniEngine.hpp>
#include <UniEngine/Extensions/3D/FirstPerson.hpp>

int main() {
    Game::initGame();

    Game::Window window = Game::createWindowAndMakeReady(800, 600, "Pencere");

    Game::enable3D();

    Game::createMainPipeline(nullptr, nullptr);

    Game::createCamera3D(
        {0.0f, 0.0f, 3.0f},
        {0.0f, 0.0f, -1.0f},
        {0.0f, 1.0f, 0.0f}
    );

    FPS::activate(window);

    Game::Color color;
    Game::createColorRGB(color, 255, 0, 0);

    Game::objectData3D plane = Game::createObject3D(
        Objects::plane,
        Game::TRIANGLES,
        color,
        { 0, 0, 0 }
    );

    Game::setBackgroundColor(0, 255, 255);

    while (Game::getWindowStatus(window) == Game::ACTIVE) {
        Game::preloadRate();

        FPS::getWASD(window);
        Game::drawObject3D(plane);

        Game::makeNewRate(window);
    }

    Game::byeToAll(window);
    
    return 0;
}
