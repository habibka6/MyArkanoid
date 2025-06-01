#pragma once
#include "Entity.h"
namespace Arkanoid {
    class StaticEntity : public Entity {
    public:
        virtual ~StaticEntity() = default;

        void update(float deltaTime) override = 0;

    protected:
        StaticEntity() = default;
    };
}