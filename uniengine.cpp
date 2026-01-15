#include <UniEngine.hpp>
#include <iostream>
#include <ostream>

int main() {
    Entity::Entity3D player = Registry::createEntity3D(Entity::PLAYER_3D, { 0.0f, 0.0f, 0.0f });

    std::cout << "PLAYER ID -> " << player.id << std::endl;
    std::cout << "PLAYER PARENT -> " << player.parentId << std::endl;
}