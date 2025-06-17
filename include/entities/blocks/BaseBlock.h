#pragma once
#include "Entity.h"

namespace Arkanoid {

    // Типы блоков в игре
    enum class BlockType {
        Standard,
        Rock
    };

    // Базовый абстрактный класс для всех типов блоков
    class BaseBlock : public Entity {
    public:
        virtual ~BaseBlock() = default;

     
        EntityType getType() const override;

      
        virtual void hit() = 0;
        virtual bool isDestroyed() const = 0;
        virtual int getPoints() const = 0;
        virtual BlockType getBlockType() const = 0;

    protected:
        BaseBlock() = default;
    };

} 
