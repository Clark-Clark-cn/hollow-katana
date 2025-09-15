#pragma once
#include "timer.h"
#include "State.h"

class EnemyAimState : public StateNode
{
public:
    EnemyAimState();
    ~EnemyAimState() = default;

    void enter() override;
    void update(float delta) override;

private:
    Timer timer;
};
class EnemyDashInAirState : public StateNode
{
public:
    EnemyDashInAirState()=default;
    ~EnemyDashInAirState() = default;

    void enter() override;
    void update(float delta) override;
    void exit() override;

private:
    const float SPEED_DASH = 1500.0f;
};
class EnemyDashOnFloorState : public StateNode
{
public:
    EnemyDashOnFloorState();
    ~EnemyDashOnFloorState() = default;

    void enter() override;
    void update(float delta) override;

private:
    const float SPEED_DASH = 1000.0f;

Timer timer;
};
class EnemyDeadState : public StateNode
{
public:
    EnemyDeadState()=default;
    ~EnemyDeadState() = default;

    void enter() override;
};
class EnemyFallState : public StateNode
{
public:
    EnemyFallState()=default;
    ~EnemyFallState() = default;

    void enter() override;
    void update(float delta) override;
};
class EnemyIdleState : public StateNode
{
public:
    EnemyIdleState();
    ~EnemyIdleState() = default;

    void enter() override;
    void update(float delta) override;
    void exit() override;

private:
    Timer timer;
};
class EnemyJumpState : public StateNode
{
public:
    EnemyJumpState() = default;
    ~EnemyJumpState() = default;

    void enter() override;
    void update(float delta) override;

private:
    const float SPEED_JUMP = 1000.0f;
};
class EnemyRunState : public StateNode
{
public:
    EnemyRunState() = default;
    ~EnemyRunState() = default;

    void enter() override;
    void update(float delta) override;
    void exit() override;

private:
    const float MIN_DIS = 350.0f;
    const float SPEED_RUN = 500.0f;
};
class EnemySquatState : public StateNode
{
public:
    EnemySquatState();
    ~EnemySquatState() = default;

    void enter() override;
    void update(float delta) override;
    void exit() override;

private:
    Timer timer;
};
class EnemyThrowBarbState : public StateNode
{
public:
    EnemyThrowBarbState();
    ~EnemyThrowBarbState() = default;

    void enter() override;
    void update(float delta) override;

private:
    Timer timer;
};
class EnemyThrowSilkState : public StateNode
{
public:
    EnemyThrowSilkState();
    ~EnemyThrowSilkState() = default;

    void enter() override;
    void update(float delta) override;

private:
    Timer timer;
};
class EnemyThrowSwordState : public StateNode
{
public:
    EnemyThrowSwordState();
    ~EnemyThrowSwordState() = default;

    void enter() override;
    void update(float delta) override;

private:
    Timer timerThrow;
    Timer timerSwitch;
};