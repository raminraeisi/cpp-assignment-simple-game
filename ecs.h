#ifndef ECS_H
#define ECS_H

#include <iostream>
#include <memory>
#include <vector>
#include <map>
#include <cassert>

using ComponentID = std::size_t;

namespace EcsInternal
{
    inline ComponentID getUniqueComponentID() noexcept
    {
        static ComponentID componentID{0u};
        return componentID++;
    }
}

namespace Ecs
{
    /**
     * Forward declaration of the used classes in this file
     * Component
     * Entity
     * EntityManager
     */
    class Component;
    class Entity;
    class EntityManager;

    /**
     * @brief getComponentTypeID generates an unique id for the given inherited and
     * version of a Component class to be used as an fixed index for that given type
     * while accessing the owned components of an entity in the Entity class
     *
     * @return returns the unique and always fixed index of the specialized
     *         Component class
     */
    template <typename T>
    inline ComponentID getComponentTypeID() noexcept
    {
        static_assert(std::is_base_of<Component, T>::value,
            "T must be inherited from Component class");

        static ComponentID typeID{EcsInternal::getUniqueComponentID()};
        return typeID;
    }

    /**
     * @brief The Component class
     */
    class Component
    {
    protected:
        /**
         * @brief entity_ holds the enclosed entity
         */
        Entity *entity_;

	public:
        /**
         * @brief constructor
         * @param entity pointer to the enclosed entity
         */
        explicit Component(Entity *entity);

        /**
         * @brief virtual desgructor
         */
        virtual ~Component();
    };

    /**
     * @brief The Entity class
     */
    class Entity
    {
    private:
        /**
         * @brief manager_ pointer to the enclosed EntityManager
         */
        EntityManager *manager_;

        /**
         * @brief components_ vector of unique pointers to the containing components
         */
        std::vector<std::unique_ptr<Component>> components_;

    public:
        /**
         * @brief constructor
         * @param manager pointer to the owner of EntityManager
         */
        explicit Entity(EntityManager *manager);

    public:
        /**
         * @brief addComponent will create and add a specialized type of Component class(T) to
         *        this entity
         * @param args variadic template arguments to be passed to the constructor of the
         *        T class
         * @return returns a reference to the created component of type T
         */
        template <typename T, typename... TArgs>
        T& addComponent(TArgs&&... args)
        {
            T* component = new T(this, std::forward<TArgs>(args)...);
            components_.at(getComponentTypeID<T>()) =  std::unique_ptr<Component>(component);
            return(*component);
        }

        /**
         * @brief hasComponent
         * @return returns true if the entity is containing a component of T type
         */
        template <typename T>
        bool hasComponent() const
        {
            return (components_[getComponentTypeID<T>()] != nullptr);
        }

        /**
         * @brief getComponent
         * @return returns the desired containing component of type T
         */
        template <typename T>
        T& getComponent() const
        {
            assert(hasComponent<T>());
            auto ptr(components_[getComponentTypeID<T>()].get());
            return *static_cast<T*>(ptr);
        }

        /**
         * @brief attack attacks an entity and reduces it's health
         * @param victim pointer to the entity to be attacked
         * @param healthReductionValue value by which the vicimt's health shall be reduced
         * @return
         */
        bool attack(Entity *victim, int healthReductionValue);

        /**
         * @brief manager
         * @return returns a pointer to the enclosed EntityManager
         */
        EntityManager* manager() {return (manager_);}
    };

    class EntityManager
    {
    private:
        /**
         * @brief maxComponentsCount_ holds the maximum allowed components of an entity
         */
        size_t maxComponentsCount_;

        /**
         * @brief entityNamesMap_ internal map of the <name, entity> pairs
         */
        std::map<std::string, Entity*> entityNamesMap_;

        /**
         * @brief entities_ internal vector of the created entities
         */
        std::vector<std::unique_ptr<Entity>> entities_;

    public:
        /**
         * @brief constructor
         * @param maxComponentsCount desired maximum allowed components count of an entity
         */
        EntityManager(size_t maxComponentsCount);

        /**
         * @brief updateComponentNamesMap inserts an item in the internal <name, map>
         *        map container by which we can get an entity using it's name later on
         * @param name entity's name
         * @param entity entity's pointer
         */
        void updateComponentNamesMap(const std::string &name, Entity *entity);

        /**
         * @brief maxComponentsCount
         * @return returns the maximum allowed components count inside an entity
         */
        size_t maxComponentsCount();


        /**
         * @brief createEntity creates a new entity
         * @return returns a reference to the newly created entity
         */
        Entity& createEntity();

        /**
         * @brief getEntityByName gets a pointer to an entity providing it's name if it is
         *        existing otherwise returns a nullptr
         * @param name the name of the desired entity
         * @return returns the pointer to the desired entity or a nullptr if not exists
         */
        Entity* getEntityByName(const std::string& name);

        /**
         * @brief entityDied a callback which is called each time an entity dies
         * @param entity pointer to the death entity
         */
        void entityDied(Entity* entity);
    };

}


#endif // ECS_H
