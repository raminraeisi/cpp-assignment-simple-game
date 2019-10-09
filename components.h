#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "ecs.h"

#include <cassert>
#include <string>
#include <thread>
#include <atomic>

using namespace Ecs;

/**
 * @brief The NameComponent class which holds the name of the owner entity
 */
class NameComponent : public Component
{
private:
    std::string name_;
public:
    /**
     * @brief NameComponent
     * @param entity pointer to the owner entity
     * @param name desired name to be assigned to entity
     */
    NameComponent(Entity *entity, const std::string& name);

    const std::string& name();
};

/**
 * @brief The HealthComponent class which holds the lives of an owner entity
 * and also provides the relevant operations on lives of the owner entity
 */
class HealthComponent : public Component
{
private:
    /**
     * @brief health_ holding the current health value of the parent entity
     * it has been defined as atomic since it might be accessed concurrently via
     * different threads
     */
    std::atomic<int> health_;

public:
    /**
     * @brief HealthComponent Constructor
     * @param entity pointer to the owner entity
     * @param initialHealth initial health value to be assigned to
     */
    HealthComponent(Entity *entity, int initialHealth);

public:
    /**
     * @brief reduceHealth reduces the health of the owner entity
     * @param amount desired value by which the health will be reduced
     */
    void reduceHealth(int amount);

    /**
     * @brief isAlive
     * @return returns true if the owner entity is still alive otherwise false
     */
    bool isAlive();

    /**
     * @brief currentHealth
     * @return returns the remaining health value of the entity
     */
    int currentHealth();
};

/**
 * @brief The PlayerComponent class which will be added to the player entities
 */
class PlayerComponent : public Component
{
public:
    PlayerComponent(Entity *entity);
};

/**
 * @brief The AttackTimer Component class which
 * utilizes a thread to create the desired periodic timer on which
 * the victim entity ig going to be attacked by the attacker entity
 */
class AttackTimerComponent : public Component
{
public:
    /**
     * @brief AttackTimerComponent Constructor
     * @param entity pointer to the desired attacker entity
     * @param victim pointer to the desired victim entity
     * @param reductionValue desired value by which the vicim's health will be reduced on each interval
     * @param interval desired interval(ms) of the periodic attack timer
     */
    AttackTimerComponent(Entity *entity, Entity *victim, int reductionValue, unsigned int interval);
};


#endif // COMPONENTS_H
