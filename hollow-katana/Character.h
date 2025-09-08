#pragma once

#include "Vector2.h"
#include "Animation.h"
#include "Collision.h"
#include "State.h"

#include <string>
#include <unordered_map>
#include <graphics.h>


class Character
{
public:
    Character()=default;
    ~Character()=default;

    int getHp()const{
        return hp;
    }
    void setPos(const Vector2& pos){
        this->pos=pos;
    }
    const Vector2& getPos()const{
        return pos;
    }
    void setVelocity(const Vector2& velocity){
        this->velocity=velocity;
    }
    const Vector2& getVelocity()const{
        return velocity;
    }
    Vector2 getLogicCenter()const{
        return Vector2(pos.x,pos.y-logicH/2);
    }
    void setEnableGravity(bool enable){
        this->enableG=enable;
    }
    CollisionBox* getHitBox()const{
        return hitBox;
    }
    CollisionBox* getHurtBox()const{
        return hurtBox;
    }
    bool isOnGround()const{
        return pos.y>=FLOOR_Y;
    }
    float getFloorY()const{
        return FLOOR_Y;
    }
    void makeInvulnerable(){
        isInvulnerable=true;
        timerInvulnerableStatus.restart();
    }

    virtual void update(int delta);
    virtual void draw();
    virtual void input(const ExMessage& msg);
    virtual void onHurt();
    void switchState(const std::string& id);
    void setAnimation(const std::string& name);
protected:
    struct AnimationGroup{
        Animation left;
        Animation right;
    };
    const float FLOOR_Y = 620.0f;
    const float GRAVITY = 980*2;

    int hp=10;
    Vector2 pos;
    Vector2 velocity;
    float logicH=0;
    bool isFacingRight=true;
    StateMachine stateMachine;
    bool enableG=true;
    bool isInvulnerable=false;
    Timer timerInvulnerableBlink;
    Timer timerInvulnerableStatus;
    bool isBlinkVisible=true;
    CollisionBox* hitBox=nullptr;
    CollisionBox* hurtBox=nullptr;
    AnimationGroup* currentAnimation=nullptr;
    std::unordered_map<std::string, AnimationGroup> animations;
};