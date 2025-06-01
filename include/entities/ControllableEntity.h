#pragma once
#include <SFML/Graphics.hpp>
#include "MovableEntity.h"
namespace Arkanoid {
    class ControllableEntity : public MovableEntity {
    public:
        virtual ~ControllableEntity() = default;

        virtual void handleInput(float deltaTime) = 0;

    protected:
        ControllableEntity() = default;
    };


} // namespace Arkanoid
