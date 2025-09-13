#pragma once

#include "character.h"

class Player : public Character
{
public:
    enum class AttackDir
    {
        Up, Down, Left, Right
    };

private:
    Timer timer_roll_cd;
    bool is_rolling = false;
    bool is_roll_cd_comp = true;

    Timer timer_attack_cd;
    bool is_attacking = false;
    bool is_attack_cd_comp = true;

    bool is_left_key_down = false;
    bool is_right_key_down = false;
    bool is_jump_key_down = false;
    bool is_roll_key_down = false;
    bool is_attack_key_down = false;

    Animation animation_slash_up;
    Animation animation_slash_down;
    Animation animation_slash_left;
    Animation animation_slash_right;
    AttackDir attack_dir = AttackDir::Right;
    Animation *current_slash_animation = nullptr;

    bool is_jump_vfx_visible = false;
    Animation animation_jump_vfx;
    bool is_land_vfx_visible = false;
    Animation animation_land_vfx;

	const float CD_ROLL = 0.75f;
    const float CD_ATTACK = 0.5f;
    const float SPEED_RUN = 300.0f;
    const float SPEED_JUMP = 780.0f;
    const float SPEED_ROLL = 800.0f;

public:
    Player();
    ~Player();

    bool input(const ExMessage& msg) override;
    void update(float delta) override;
    void draw() override;

    void hurt() override;

    void setRolling(bool flag)
    {
        is_rolling = flag;
    }
    bool getRolling() const
    {
        return is_rolling;
    }
    bool canRoll() const
    {
        return is_roll_cd_comp && !is_rolling && is_roll_key_down;
    }

    void setAttacking(bool flag)
    {
        is_attacking = flag;
    }
    bool canAttack() const
    {
        return is_attack_cd_comp && !is_attacking && is_attack_key_down;
    }
    bool getAttacking() const
    {
        return is_attacking;
    }
    bool canJump() const
    {
        return isOnFloor() && is_jump_key_down;
    }
    int getMoveAxis() const
    {
        return is_right_key_down - is_left_key_down;
    }

    AttackDir getAttackDir() const
    {
        return attack_dir;
    }

    void jump();
    void land();
    void roll();
    void attack();

private:
    void updateAttackDir(int x, int y);
};