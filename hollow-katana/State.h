#pragma once
#include <string>
#include <unordered_map>

class StateNode
{
public:
    StateNode() = default;
    virtual ~StateNode() = default;

    virtual void onEnter() {}
    virtual void onExit() {}
    virtual void update(float deltaTime) {}
private: 
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
    bool needInit=true;
    std::unordered_map<std::string, StateNode*> states;
    StateNode* currentState = nullptr;
};