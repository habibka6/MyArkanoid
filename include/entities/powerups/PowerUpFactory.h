#pragma once
#include <functional>
#include <unordered_map>
#include <memory>
#include "PowerUp.h"

namespace Arkanoid {

    // Централизованная фабрика бонусов. Реализует паттерн "Factory"
    class PowerUpFactory {
    public:
        // Функция для создания бонуса
        using Creator = std::function<std::unique_ptr<PowerUp>(float, float)>;
        
        // Заполняет список доступных бонусов при создании фабрики
        PowerUpFactory();

        std::unique_ptr<PowerUp> createPowerUp(PowerUpType type, float x, float y) const;

    private:
        // Хранит соответствие типа бонуса и функции его создания
        std::unordered_map<PowerUpType, Creator> creators;
    };

} // namespace Arkanoid
