#pragma once
#include "Collision.h"
#include "animation.h"


class Sword
{
public:
    Sword(const Vector2& position,bool moveLeft);
    ~Sword();

    void update(float delta);
    void draw();
    bool checkValid() const
    {
        return isValid;
    }
    private:
    const float SPEED_MOVE=1250.0f;

    Vector2 position;
	Vector2 velocity;
	Animation animation;
	bool isValid=true;
	CollisionBox*collisionBox=nullptr;

};