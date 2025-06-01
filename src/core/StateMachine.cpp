#include "StateMachine.h"
#include "State.h"

namespace Arkanoid {

    void StateMachine::pushState(std::unique_ptr<State> state) {
        if (!states.empty()) {
            states.top()->pause();
        }

        state->enter();
        states.push(std::move(state));
    }

    void StateMachine::popState() {
        if (!states.empty()) {
            states.top()->exit();
            states.pop();

            if (!states.empty()) {
                states.top()->resume();
            }
        }
    }

    void StateMachine::changeState(std::unique_ptr<State> state) {
        if (!states.empty()) {
            states.top()->exit();
            states.pop();
        }

        state->enter();
        states.push(std::move(state));
    }

    void StateMachine::clearStates() {
        while (!states.empty()) {
            states.top()->exit();
            states.pop();
        }
    }

    void StateMachine::update(float deltaTime) {
        if (!states.empty()) {
            states.top()->update(deltaTime);
        }
    }

    void StateMachine::render(sf::RenderWindow& window) {
        if (!states.empty()) {
            states.top()->render(window);
        }
    }

    void StateMachine::handleEvent(const sf::Event& event) {
        if (!states.empty()) {
            states.top()->handleEvent(event);
        }
    }

    bool StateMachine::isEmpty() const {
        return states.empty();
    }

    State* StateMachine::getCurrentState() const {
        return states.empty() ? nullptr : states.top().get();
    }

} // namespace Arkanoid
