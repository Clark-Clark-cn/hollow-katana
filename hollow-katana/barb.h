#pragma once
#include "animation.h"
#include "Collision.h"

class Barb
{
public:
    Barb();
    ~Barb();

    void update(float delta);
    void draw();
    void setPosition(const Vector2 &position)
    {
        this->basePosition = position;
        this->currentPosition = position;
    }
    bool checkValid() const
    {
        return isValid;
    }

private:
    enum class Stage
    {
        Idle,
        Aim,
        Dash,
        Break,
    };

const float SPEED_DASH = 1500.0f;
    Timer timerIdle;
    Timer timerAim;
    int diffPeriod = 0;
    bool isValid = true;
    float totalDeltaTime = 0;
    
    Vector2 velocity;
    Vector2 basePosition;
    Vector2 currentPosition;

    Animation animationLoose;
    Animation animationBreak;
    Animation *currentAnimation = nullptr;

    Stage stage = Stage::Idle;
    CollisionBox *collisionBox = nullptr;

	void onBreak();
};
