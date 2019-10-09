#include <iostream>
#include <thread>
#include <memory.h>
#include <chrono>

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



    /// implementing a test case to check if the operability is correct,
    /// without any user input and by utilising the specified attack timers of orc and
    /// dragon entities, the hero's health shall reach zero in about 21000 miliseconds, so
    /// the following use case is to test that
    auto startTime = std::chrono::steady_clock::now();

    auto remainingTime = 21000;
    while(remainingTime >= 0)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));

        auto endTime = std::chrono::steady_clock::now();
        auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime-startTime).count();
        startTime = endTime;

        remainingTime -= elapsedTime;
        std::cout << "remaining time ... " << remainingTime << " ms ... " << std::endl;
    }

    int heroRemainingHealth = hero.getComponent<HealthComponent>().currentHealth();
    std::cout << std::endl << "hero remaining health = " << heroRemainingHealth;

    if (heroRemainingHealth > 0)
        std::cout << std::endl << "!!!   Test Failed   !!!" << std::endl;
    else if (heroRemainingHealth == 0)
        std::cout << std::endl << "!!!   Test Passed   !!!" << std::endl;


}
