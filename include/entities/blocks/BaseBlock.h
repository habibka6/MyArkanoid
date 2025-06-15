#pragma once
#include "Entity.h"

namespace Arkanoid {

    enum class BlockType {
        Standard,
        Rock,
        Special
    };

    class BaseBlock : public Entity {
    public:
        virtual ~BaseBlock() = default;

     
        EntityType getType() const override;

      
        virtual void hit() = 0;
        virtual bool isDestroyed() const = 0;
        virtual int getPoints() const = 0;
        virtual BlockType getBlockType() const = 0;
        virtual sf::Color getColor() const = 0;

    protected:
        BaseBlock() = default;
    };

} 
