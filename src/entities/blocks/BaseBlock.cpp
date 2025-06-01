#include "BaseBlock.h"

namespace Arkanoid {

    // Реализация BaseBlock::getType
    EntityType BaseBlock::getType() const {
        return EntityType::Block;
    }

} // namespace Arkanoid
