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
    Character();
    ~Character();

    int getHp()const{
        return hp;
    }
    void setPosition(const Vector2& position){
        this->position=position;
    }
    const Vector2& getPosition()const{
        return position;
    }
    void setVelocity(const Vector2& velocity){
        this->velocity=velocity;
    }
    const Vector2& getVelocity()const{
        return velocity;
    }
    Vector2 getLogicCenter()const{
        return Vector2(position.x,position.y-logic_height/2);
    }
    void setEnableGravity(bool enable){
        this->enable_gravity=enable;
    }
    CollisionBox* getHitBox()const{
        return hit_box;
    }
    CollisionBox* getHurtBox()const{
        return hurt_box;
    }
    bool isOnFloor()const{
        return position.y>=FLOOR_Y;
    }
    float getFloorY()const{
        return FLOOR_Y;
    }
    
    void makeInvulnerable(){
        is_invulnerable=true;
        timer_invulnerable_status.restart();
    }

    virtual void update(float delta);
    virtual void draw();
    virtual bool input(const ExMessage& msg);
    virtual void decreaseHp();
    virtual void hurt();
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
    Vector2 position;
    Vector2 velocity;
    float logic_height=0;
    bool is_facing_left=true;
    StateMachine state_machine;
    bool enable_gravity=true;
    bool is_invulnerable=false;
    Timer timer_invulnerable_blink;
    Timer timer_invulnerable_status;
    bool is_blink_invisible=true;
    CollisionBox* hit_box=nullptr;
    CollisionBox* hurt_box=nullptr;
    AnimationGroup* current_animation=nullptr;
    std::unordered_map<std::string, AnimationGroup> animation_pool;
};