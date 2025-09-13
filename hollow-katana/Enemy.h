#pragma once
#include "barb.h"
#include "sword.h"
#include "Character.h"

class Enemy : public Character
{
public:
    Enemy();
    ~Enemy();
    void update(float delta) override;
    void draw() override;

    void hurt() override;
    void setFacingLeft(bool flag)
    {
        is_facing_left = flag;
    }
    bool isFacingLeft() const
    {
        return is_facing_left;
    }
    void setDashingInAir(bool flag)
    {
        is_dashing_in_air = flag;
    }
    bool getDashingInAir() const
    {
        return is_dashing_in_air;
    }
    void setDashingOnFloor(bool flag)
    {
        is_dashing_on_floor = flag;
    }
    bool getDashingOnFloor() const
    {
        return is_dashing_on_floor;
    }
    void setThrowingSilk(bool flag)
    {
        is_throwing_silk = flag;
        collision_box_silk->setEnabled(flag);
    }
    bool getThrowingSilk() const
    {
        return is_throwing_silk;
    }
    void throwBarbs();
    void throwSword();
    void dash();
    void onThrowSilk();

private:
    bool is_throwing_silk = false;
    bool is_dashing_in_air = false;
    bool is_dashing_on_floor = false;

    Animation animation_silk;
    AnimationGroup animation_dash_in_air_vfx;
    AnimationGroup animation_dash_on_floor_vfx;
    Animation *current_dash_animation = nullptr;

    std::vector<Barb *> barbList;
    std::vector<Sword *> swordList;
    CollisionBox *collision_box_silk = nullptr;
};