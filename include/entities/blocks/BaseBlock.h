#pragma once
#include "StaticEntity.h"

namespace Arkanoid {

    enum class BlockType {
        Standard,
        Rock,
        Special
    };

    class BaseBlock : public StaticEntity {
    public:
        virtual ~BaseBlock() = default;

        // Entity interface
        EntityType getType() const override;

        // Block specific interface
        virtual void hit() = 0;
        virtual bool isDestroyed() const = 0;
        virtual int getPoints() const = 0;
        virtual BlockType getBlockType() const = 0;
        virtual sf::Color getColor() const = 0;

    protected:
        BaseBlock() = default;
    };

} // namespace Arkanoid
