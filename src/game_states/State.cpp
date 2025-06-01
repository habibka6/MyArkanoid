#include "State.h"
#include "GameEngine.h" // Теперь включаем здесь

namespace Arkanoid {
    State::State(GameEngine& engine)
        : engine(engine)
    {
    }
}