#include "State.h"

StateMachine::StateMachine() = default;
StateMachine::~StateMachine() = default;

void StateMachine::registerState(const std::string& id, StateNode* state) {
    states[id] = state;
}

void StateMachine::setEntry(const std::string& id) {
    currentState = states[id];
}

void StateMachine::switchTo(const std::string& id) {
    if(currentState)currentState->onExit();
    currentState = states[id];
    if(currentState)currentState->onEnter();
}

void StateMachine::update(float deltaTime) {
    if (currentState) {
        if (needInit) {
            currentState->onEnter();
            needInit = false;
        }
        currentState->update(deltaTime);
    }
}