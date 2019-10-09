#include <iostream>
#include <thread>
#include <memory.h>
#include <chrono>
#include <sstream>

#include "components.h"
#include "ecs.h"


int main()
{
    /// creates the EntityManager instace
    Ecs::EntityManager entityManager(5);

    /// creating the hero game entity and adding the name & health components to it
    auto& hero(entityManager.createEntity());
    hero.addComponent<NameComponent>("hero");
    hero.addComponent<HealthComponent>(40);
    hero.addComponent<PlayerComponent>();


    /// creating the orc game entity and adding the name, health and attack timer components to it
    auto& orc(entityManager.createEntity());
    orc.addComponent<NameComponent>("orc");
    orc.addComponent<HealthComponent>(7);
    orc.addComponent<AttackTimerComponent>(&hero, 1, 1300);

    /// creating the dragon game entity and adding the name, health and attack timer components to it
    auto& dragon(entityManager.createEntity());
    dragon.addComponent<NameComponent>("dragon");
    dragon.addComponent<HealthComponent>(20);
    dragon.addComponent<AttackTimerComponent>(&hero, 3, 2500);

    std::string line, word;
    std::vector<std::string> words;

    /// getting the user input and attack the requested game entity
    while(std::getline(std::cin, line))
    {
        words.clear();
        std::stringstream sstream(line);
        while(sstream >> word)
            words.push_back(word);

        if ((words.size() == 2) && (!words[0].compare("attack")))
        {
            hero.attack(entityManager.getEntityByName(words[1]), 2);
        }
    }


}
