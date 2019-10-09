#include "components.h"

/**
 ***** NameComponent Class Definitions *****
 */

NameComponent::NameComponent(Entity *entity, const std::string& name)
    : Component(entity), name_(name)
{
    entity->manager()->updateComponentNamesMap(name, entity);
}

const std::string& NameComponent::name()
{
    return(name_);
}


/**
 ***** HealthComponent Class Definitions *****
 */

HealthComponent::HealthComponent(Entity *entity, int initialHealth)
    : Component(entity), health_(initialHealth)
{

}

void HealthComponent::reduceHealth(int amount)
{
    assert(amount);
    health_ = std::max(health_ - amount, 0);

    if (!health_ && entity_->hasComponent<PlayerComponent>())
    {
        entity_->manager()->entityDied(entity_);
    }
}

bool HealthComponent::isAlive()
{
    return(health_ > 0);
}

int HealthComponent::currentHealth()
{
    return(health_);
}


/**
 ***** PlayerComponent Class Definitions *****
 */

PlayerComponent::PlayerComponent(Entity* entity)
    : Component(entity)
{
}


/**
 ***** AttackTimerComponent Class Definitions *****
 */

AttackTimerComponent::AttackTimerComponent(Entity *entity, Entity *victim, int reductionValue, unsigned int interval)
    : Component(entity)
{
    if(entity->hasComponent<HealthComponent>() && victim->hasComponent<HealthComponent>())
    {
        std::thread thread([=]()
        {
            bool bothAlive{true};

            while(bothAlive)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(interval));
                bothAlive = entity->attack(victim, reductionValue);
            }

        });
        thread.detach();
    }

}

