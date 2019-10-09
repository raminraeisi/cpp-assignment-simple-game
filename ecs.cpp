#include "ecs.h"

#include <cassert>
#include "components.h"

namespace Ecs
{


    /**
     * Component Class Definition
     */
    Component::Component(Entity *entity)
        : entity_(entity)
    {

    }
    Component::~Component()
    {

    }

    /**
     * Entity Class Definition
     */

    Entity::Entity(EntityManager *manager)
        : manager_(manager)
    {
        size_t maxComponents = manager->maxComponentsCount();

        components_.reserve(maxComponents);
        for(size_t i = 0; i < maxComponents; ++i)
            components_.emplace_back(nullptr);
    }

    bool Entity::attack(Entity *victim, int healthReductionValue)
    {
        bool bothAlive = false;
        if (victim &&
              this->hasComponent<NameComponent>() && this->hasComponent<HealthComponent>() &&
                victim->hasComponent<NameComponent>() && victim->hasComponent<HealthComponent>() )
        {

            auto& attackerNameComp = this->getComponent<NameComponent>();
            auto& attackerHealthComp = this->getComponent<HealthComponent>();

            auto& victimNameComp = victim->getComponent<NameComponent>();
            auto& victimHealthComp = victim->getComponent<HealthComponent>();

            if ((bothAlive = attackerHealthComp.isAlive() && victimHealthComp.isAlive()))
            {
                victimHealthComp.reduceHealth(healthReductionValue);

                std::cout << attackerNameComp.name() << " hit " <<
                             victimNameComp.name() << ". " <<
                             victimNameComp.name() << " health is " <<
                             victimHealthComp.currentHealth() << "." << std::endl;
            }
        }
        return(bothAlive);
    }


    /**
     * EntityManager Class Definition
     */

    EntityManager::EntityManager(size_t maxComponentsCount)
        : maxComponentsCount_(maxComponentsCount)
    {

    }

    void EntityManager::updateComponentNamesMap(const std::string &name, Entity *entity)
    {
        assert(entityNamesMap_.find(name) == entityNamesMap_.end());
        entityNamesMap_.insert(std::make_pair(name, entity));
    }

    Entity& EntityManager::createEntity()
    {
        Entity *entity = new Entity(this);
        entities_.emplace_back(std::unique_ptr<Entity>(entity));
        return(*entity);
    }

    size_t EntityManager::maxComponentsCount()
    {
        return(maxComponentsCount_);
    }

    Entity* EntityManager::getEntityByName(const std::string &name)
    {
        auto pair = entityNamesMap_.find(name);
        if (pair != entityNamesMap_.end())
            return pair->second;
        else return nullptr;
    }

    void EntityManager::entityDied(Entity* entity)
    {
        if (entity->hasComponent<PlayerComponent>() && entity->hasComponent<NameComponent>())
            std::cout << "Player " << entity->getComponent<NameComponent>().name() << " lost the game." << std::endl;
    }

}
