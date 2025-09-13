#pragma once

#include "Timer.h"
#include "State.h"

class PlayerAttackState : public StateNode
{
public:
    PlayerAttackState();
    ~PlayerAttackState() = default;

    void enter() override;
    void update(float delta) override;
    void exit() override;

private:
    Timer timer;

static void updateHitBoxPosition();
};
class PlayerDeadState : public StateNode
{
public:
    PlayerDeadState();
    ~PlayerDeadState() = default;

    void enter() override;
    void update(float delta) override;

private:
    Timer timer;
};

class PlayerFallState : public StateNode
{
public:
    PlayerFallState() = default;
    ~PlayerFallState() = default;

    void enter() override;
    void update(float delta) override;
};
class PlayerIdleState : public StateNode
{
public:
    PlayerIdleState() = default;
    ~PlayerIdleState() = default;

    void enter() override;
    void update(float delta) override;
};
class PlayerJumpState : public StateNode
{
public:
    PlayerJumpState() = default;
    ~PlayerJumpState() = default;

    void enter() override;
    void update(float delta) override;
};
class PlayerRollState : public StateNode
{
public:
    PlayerRollState();
    ~PlayerRollState() = default;

    void enter() override;
    void update(float delta) override;
    void exit() override;

private:
    Timer timer;
};

class PlayerRunState : public StateNode
{
public:
    PlayerRunState() = default;
    ~PlayerRunState() = default;

    void enter() override;
    void update(float delta) override;
    void exit() override;
};
