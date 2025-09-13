#include "State.h"

StateMachine::StateMachine() = default;
StateMachine::~StateMachine() = default;

void StateMachine::registerState(const std::string& id, StateNode* state) {
    states[id] = state;
    states[id]->name = id;
}

void StateMachine::setEntry(const std::string& id) {
    current_state = states[id];
}

void StateMachine::switchTo(const std::string& id) {
    if(current_state)current_state->exit();
    current_state = states[id];
    if(current_state)current_state->enter();
}

void StateMachine::update(float deltaTime) {
    if (current_state) {
        if (need_init) {
            current_state->enter();
            need_init = false;
        }
        current_state->update(deltaTime);
    }
}