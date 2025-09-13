#pragma once
#include "player.h"
#include <cmath>
#include "ResourcesManager.h"
#include "playerStateNodes.h"
#include "bulletTimeManager.h"

Player::Player()
{
    is_facing_left = false;
    position = {250, 200};
    logic_height = 120;

    hit_box->setSize({150, 150});
    hurt_box->setSize({40, 80});

    hit_box->setLayerSrc(CollisionLayer::None);
    hit_box->setLayerDst(CollisionLayer::Enemy);

    hurt_box->setLayerSrc(CollisionLayer::Player);
    hurt_box->setLayerDst(CollisionLayer::None);

    hit_box->setEnabled(false);
    hurt_box->setOnCollision([&](){ decreaseHp(); });

    timer_roll_cd.setWaitTime(CD_ROLL);
    timer_roll_cd.setOneShot(true);
    timer_roll_cd.setCallback([&](){ is_roll_cd_comp = true; });

    timer_attack_cd.setWaitTime(CD_ATTACK);
    timer_attack_cd.setOneShot(true);
    timer_attack_cd.setCallback([&](){ is_attack_cd_comp = true; });
    {
        AnimationGroup& animationAttack = animation_pool["attack"];

        Animation &animationAttackLeft = animationAttack.left;
        animationAttackLeft.setInterval(0.05f);
        animationAttackLeft.setLoop(false);
        animationAttackLeft.setAnchorMode(Animation::AnchorMode::BottomCentered);
        animationAttackLeft.addFrame(ResourcesManager::getInstance()->getImage("player_attack_left"), 5);

        Animation &animationAttackRight = animationAttack.right;
        animationAttackRight.setInterval(0.05f);
        animationAttackRight.setLoop(false);
        animationAttackRight.setAnchorMode(Animation::AnchorMode::BottomCentered);
        animationAttackRight.addFrame(ResourcesManager::getInstance()->getImage("player_attack_right"), 5);

        AnimationGroup& animationDead = animation_pool["dead"];

        Animation &animationDeadLeft = animationDead.left;
        animationDeadLeft.setInterval(0.1f);
        animationDeadLeft.setLoop(false);
        animationDeadLeft.setAnchorMode(Animation::AnchorMode::BottomCentered);
        animationDeadLeft.addFrame(ResourcesManager::getInstance()->getImage("player_dead_left"), 6);

        Animation &animationDeadRight = animationDead.right;
        animationDeadRight.setInterval(0.1f);
        animationDeadRight.setLoop(false);
        animationDeadRight.setAnchorMode(Animation::AnchorMode::BottomCentered);
        animationDeadRight.addFrame(ResourcesManager::getInstance()->getImage("player_dead_right"), 6);

        AnimationGroup &animationFall = animation_pool["fall"];

        Animation &animationFallLeft = animationFall.left;
        animationFallLeft.setInterval(0.15f);
        animationFallLeft.setLoop(true);
        animationFallLeft.setAnchorMode(Animation::AnchorMode::BottomCentered);
        animationFallLeft.addFrame(ResourcesManager::getInstance()->getImage("player_fall_left"), 5);

        Animation &animationFallRight = animationFall.right;
        animationFallRight.setInterval(0.15f);
        animationFallRight.setLoop(true);
        animationFallRight.setAnchorMode(Animation::AnchorMode::BottomCentered);
        animationFallRight.addFrame(ResourcesManager::getInstance()->getImage("player_fall_right"), 5);

        AnimationGroup &animationIdle = animation_pool["idle"];

        Animation &animationIdleLeft = animationIdle.left;
        animationIdleLeft.setInterval(0.15f);
        animationIdleLeft.setLoop(true);
        animationIdleLeft.setAnchorMode(Animation::AnchorMode::BottomCentered);
        animationIdleLeft.addFrame(ResourcesManager::getInstance()->getImage("player_idle_left"), 5);

        Animation &animationIdleRight = animationIdle.right;
        animationIdleRight.setInterval(0.15f);
        animationIdleRight.setLoop(true);
        animationIdleRight.setAnchorMode(Animation::AnchorMode::BottomCentered);
        animationIdleRight.addFrame(ResourcesManager::getInstance()->getImage("player_idle_right"), 5);

        AnimationGroup &animationJump = animation_pool["jump"];
        Animation &animationJumpLeft = animationJump.left;
        animationJumpLeft.setInterval(0.15f);
        animationJumpLeft.setLoop(false);
        animationJumpLeft.setAnchorMode(Animation::AnchorMode::BottomCentered);
        animationJumpLeft.addFrame(ResourcesManager::getInstance()->getImage("player_jump_left"), 5);

        Animation &animationJumpRight = animationJump.right;
        animationJumpRight.setInterval(0.15f);
        animationJumpRight.setLoop(false);
        animationJumpRight.setAnchorMode(Animation::AnchorMode::BottomCentered);
        animationJumpRight.addFrame(ResourcesManager::getInstance()->getImage("player_jump_right"), 5);

        AnimationGroup &animationRoll = animation_pool["roll"];

        Animation &animationRollLeft = animationRoll.left;
        animationRollLeft.setInterval(0.05f);
        animationRollLeft.setLoop(false);
        animationRollLeft.setAnchorMode(Animation::AnchorMode::BottomCentered);
        animationRollLeft.addFrame(ResourcesManager::getInstance()->getImage("player_roll_left"), 7);

        Animation &animationRollRight = animationRoll.right;
        animationRollRight.setInterval(0.05f);
        animationRollRight.setLoop(false);
        animationRollRight.setAnchorMode(Animation::AnchorMode::BottomCentered);
        animationRollRight.addFrame(ResourcesManager::getInstance()->getImage("player_roll_right"), 7);

        AnimationGroup &animationRun = animation_pool["run"];

        Animation &animationRunLeft = animationRun.left;
        animationRunLeft.setInterval(0.075f);
        animationRunLeft.setLoop(true);
        animationRunLeft.setAnchorMode(Animation::AnchorMode::BottomCentered);
        animationRunLeft.addFrame(ResourcesManager::getInstance()->getImage("player_run_left"), 10);

        Animation &animationRunRight = animationRun.right;
        animationRunRight.setInterval(0.075f);
        animationRunRight.setLoop(true);
        animationRunRight.setAnchorMode(Animation::AnchorMode::BottomCentered);
        animationRunRight.addFrame(ResourcesManager::getInstance()->getImage("player_run_right"), 10);
    }
    {
        animation_slash_up.setInterval(0.07f);
        animation_slash_up.setLoop(false);
        animation_slash_up.setAnchorMode(Animation::AnchorMode::Centered);
        animation_slash_up.addFrame(ResourcesManager::getInstance()->getImage("player_vfx_attack_up"), 5);

        animation_slash_down.setInterval(0.07f);
        animation_slash_down.setLoop(false);
        animation_slash_down.setAnchorMode(Animation::AnchorMode::Centered);
        animation_slash_down.addFrame(ResourcesManager::getInstance()->getImage("player_vfx_attack_down"), 5);

        animation_slash_left.setInterval(0.07f);
        animation_slash_left.setLoop(false);
        animation_slash_left.setAnchorMode(Animation::AnchorMode::Centered);
        animation_slash_left.addFrame(ResourcesManager::getInstance()->getImage("player_vfx_attack_left"), 5);

        animation_slash_right.setInterval(0.07f);
        animation_slash_right.setLoop(false);
        animation_slash_right.setAnchorMode(Animation::AnchorMode::Centered);
        animation_slash_right.addFrame(ResourcesManager::getInstance()->getImage("player_vfx_attack_right"), 5);

        animation_jump_vfx.setInterval(0.05f);
        animation_jump_vfx.setLoop(false);
        animation_jump_vfx.setAnchorMode(Animation::AnchorMode::BottomCentered);
        animation_jump_vfx.addFrame(ResourcesManager::getInstance()->getImage("player_vfx_jump"), 5);
        animation_jump_vfx.setOnComplete([&](){is_jump_vfx_visible=false;});

        animation_land_vfx.setInterval(0.1f);
        animation_land_vfx.setLoop(false);
        animation_land_vfx.setAnchorMode(Animation::AnchorMode::BottomCentered);
        animation_land_vfx.addFrame(ResourcesManager::getInstance()->getImage("player_vfx_land"), 2);
        animation_land_vfx.setOnComplete([&](){ is_land_vfx_visible = false; });
    }
    {
        state_machine.registerState("attack",new PlayerAttackState());
        state_machine.registerState("dead",new PlayerDeadState());
        state_machine.registerState("fall",new PlayerFallState());
        state_machine.registerState("idle",new PlayerIdleState());
        state_machine.registerState("jump",new PlayerJumpState());
        state_machine.registerState("roll",new PlayerRollState());
        state_machine.registerState("run",new PlayerRunState());

        state_machine.setEntry("idle");
    }
}

Player::~Player() = default;
extern bool isPaused;

bool Player::input(const ExMessage& msg)
{
    if (hp <= 0)return false;

    switch (msg.message)
    {
    case WM_KEYDOWN:
        switch (msg.vkcode)
        {
        case 'A':
        case VK_LEFT:
            is_left_key_down = true;
            break;
        case 'D':
        case VK_RIGHT:
            is_right_key_down = true;
            break;
        case 'W':
        case VK_UP:
            is_jump_key_down = true;
            break;
        case 'S':
        case VK_DOWN:
            is_roll_key_down = true;
            break;
        default:return false;
        }
        break;
    case WM_KEYUP:
        switch (msg.vkcode)
        {
        case 'A':
        case VK_LEFT:
            is_left_key_down = false;
            break;
        case 'D':
        case VK_RIGHT:
            is_right_key_down = false;
            break;
        case 'W':
        case VK_UP:
        case VK_SPACE:
            is_jump_key_down = false;
            break;
        case 'S':
        case VK_DOWN:
            is_roll_key_down = false;
            break;
        default:return false;
        }
        break;
    case WM_LBUTTONDOWN:
        is_attack_key_down = true;
        updateAttackDir(msg.x, msg.y);
        break;
    case WM_LBUTTONUP:
        is_attack_key_down = false;
        break;
    case WM_RBUTTONDOWN:
        playAudio(L"bullet_time",false);
        BulletTimeManager::getInstance()->setStatus(BulletTimeManager::Status::Entering);
        break;
    case WM_RBUTTONUP:
        playAudio(L"bullet_time",false);
        BulletTimeManager::getInstance()->setStatus(BulletTimeManager::Status::Exiting);
        break;
	default:return false;
    }
}

void Player::update(float delta)
{
    if (hp > 0 && !is_rolling)
        velocity.x = getMoveAxis() * SPEED_RUN;

    if (getMoveAxis() != 0)
        is_facing_left = (getMoveAxis() < 0);

    timer_roll_cd.update(delta);
    timer_attack_cd.update(delta);
    animation_jump_vfx.update(delta);
    animation_land_vfx.update(delta);

    if (is_attacking)
    {
        current_slash_animation->setPosition(getLogicCenter());
        current_slash_animation->update(delta);
    }
    Character::update(delta);
}

void Player::draw()
{
    if (is_jump_vfx_visible)
        animation_jump_vfx.draw();
    if (is_land_vfx_visible)
        animation_land_vfx.draw();
    Character::draw();
    if (is_attacking)
        current_slash_animation->draw();
}

void Player::hurt()
{
    playAudio(L"player_hurt", false);
}

void Player::jump()
{
    velocity.y -= SPEED_JUMP;
    is_jump_vfx_visible = true;
    animation_jump_vfx.setPosition(position);
    animation_jump_vfx.reset();
}

void Player::land()
{
    is_land_vfx_visible = true;
    animation_land_vfx.setPosition(position);
    animation_land_vfx.reset();
}

void Player::roll()
{
    timer_roll_cd.restart();
    is_roll_cd_comp = false;
    velocity.x = is_facing_left ? -SPEED_ROLL : SPEED_ROLL;
}

void Player::attack()
{
    timer_attack_cd.restart();
    is_attack_cd_comp = false;

    switch (attack_dir)
    {
    case Player::AttackDir::Up:
        current_slash_animation = &animation_slash_up;
        break;
    case Player::AttackDir::Down:
        current_slash_animation = &animation_slash_down;
        break;
    case Player::AttackDir::Left:
        current_slash_animation = &animation_slash_left;
        break;
    case Player::AttackDir::Right:
        current_slash_animation = &animation_slash_right;
        break;
    }
    current_slash_animation->setPosition(getLogicCenter());
    current_slash_animation->reset();
}

void Player::updateAttackDir(int x,int y){
    static constexpr float PI=3.141592654f;
    float angle=std::atan2(y-position.y,x-position.x);

    if (angle >= -PI / 4 && angle < PI / 4)attack_dir = AttackDir::Right;
    else if (angle >= PI / 4 && angle < 3 * PI / 4)attack_dir = AttackDir::Down;
    else if ((angle >= 3 * PI / 4 && angle <= PI) || (angle >= -PI && angle < -3 * PI / 4))attack_dir = AttackDir::Left;
    else attack_dir = AttackDir::Up;
}