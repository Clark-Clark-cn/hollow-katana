#pragma once
#include <string>
#include <unordered_map>

class StateMachine;

class StateNode
{
    friend class StateMachine;
    std::string name;
public:
    StateNode() = default;
    virtual ~StateNode() = default;

    virtual void enter() {}
    virtual void exit() {}
    virtual void update(float deltaTime) {}
};

class StateMachine
{
public:
    StateMachine();
    ~StateMachine();

    void registerState(const std::string& id, StateNode* state);
    void setEntry(const std::string& id);
    void switchTo(const std::string& id);

    void update(float deltaTime);

private:
    bool need_init=true;
    std::unordered_map<std::string, StateNode*> states;
    StateNode* current_state = nullptr;
};