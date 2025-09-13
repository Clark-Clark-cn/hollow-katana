#include "enemy.h"
#include "characterManager.h"
#include "resourcesManager.h"
#include "Collision.h"
#include "enemyStateNodes.h"
 
Enemy::Enemy()
{
    is_facing_left = true;
    position = {1050, 200};
    logic_height = 150;

    hit_box->setSize({50, 80});
    hit_box->setSize({100, 180});

    hit_box->setLayerSrc(CollisionLayer::None);
    hit_box->setLayerDst(CollisionLayer::Player);

    hurt_box->setLayerSrc(CollisionLayer::Enemy);
    hurt_box->setLayerDst(CollisionLayer::None);
    hurt_box->setOnCollision([&](){ decreaseHp(); });
    collision_box_silk = CollisionSystem::getInstance()->createBox();
    collision_box_silk->setLayerSrc(CollisionLayer::None);
    collision_box_silk->setLayerDst(CollisionLayer::Player);
    collision_box_silk->setSize({225, 225});
    collision_box_silk->setEnabled(false);
    {
        AnimationGroup &animationAim = animation_pool["aim"];

        Animation &animationAimLeft = animationAim.left;
        animationAimLeft.setInterval(0.05f);
        animationAimLeft.setLoop(false);
        animationAimLeft.setAnchorMode(Animation::AnchorMode::BottomCentered);
        animationAimLeft.addFrame(ResourcesManager::getInstance()->getAtlas("enemy_aim_left"));

        Animation &animationAimRight = animationAim.right;
        animationAimRight.setInterval(0.05f);
        animationAimRight.setLoop(false);
        animationAimRight.setAnchorMode(Animation::AnchorMode::BottomCentered);
        animationAimRight.addFrame(ResourcesManager::getInstance()->getAtlas("enemy_aim_right"));
    }
    {
        AnimationGroup &animationDashInAir = animation_pool["dash_in_air"];

        Animation &animationDashInAirLeft = animationDashInAir.left;
        animationDashInAirLeft.setInterval(0.05f);
        animationDashInAirLeft.setLoop(true);
        animationDashInAirLeft.setAnchorMode(Animation::AnchorMode::BottomCentered);
        animationDashInAirLeft.addFrame(ResourcesManager::getInstance()->getAtlas("enemy_dash_in_air_left"));

        Animation &animationDashInAirRight = animationDashInAir.right;
        animationDashInAirRight.setInterval(0.05f);
        animationDashInAirRight.setLoop(true);
        animationDashInAirRight.setAnchorMode(Animation::AnchorMode::BottomCentered);
        animationDashInAirRight.addFrame(ResourcesManager::getInstance()->getAtlas("enemy_dash_in_air_right"));
    }
    {
        AnimationGroup &animationDashOnFloor = animation_pool["dash_on_floor"];

        Animation &animationDashOnFloorLeft = animationDashOnFloor.left;
        animationDashOnFloorLeft.setInterval(0.05f);
        animationDashOnFloorLeft.setLoop(true);
        animationDashOnFloorLeft.setAnchorMode(Animation::AnchorMode::BottomCentered);
        animationDashOnFloorLeft.addFrame(ResourcesManager::getInstance()->getAtlas("enemy_dash_on_floor_left"));

        Animation &animationDashOnFloorRight = animationDashOnFloor.right;
        animationDashOnFloorRight.setInterval(0.05f);
        animationDashOnFloorRight.setLoop(true);
        animationDashOnFloorRight.setAnchorMode(Animation::AnchorMode::BottomCentered);
        animationDashOnFloorRight.addFrame(ResourcesManager::getInstance()->getAtlas("enemy_dash_on_floor_right"));
    }
    {
        AnimationGroup &animationFall = animation_pool["fall"];

        Animation &animationFallLeft = animationFall.left;
        animationFallLeft.setInterval(0.1f);
        animationFallLeft.setLoop(true);
        animationFallLeft.setAnchorMode(Animation::AnchorMode::BottomCentered);
        animationFallLeft.addFrame(ResourcesManager::getInstance()->getAtlas("enemy_fall_left"));

        Animation &animationFallRight = animationFall.right;
        animationFallRight.setInterval(0.1f);
        animationFallRight.setLoop(true);
        animationFallRight.setAnchorMode(Animation::AnchorMode::BottomCentered);
        animationFallRight.addFrame(ResourcesManager::getInstance()->getAtlas("enemy_fall_right"));
    }
    {
        AnimationGroup &animationIdle = animation_pool["idle"];

        Animation &animationIdleLeft = animationIdle.left;
        animationIdleLeft.setInterval(0.1f);
        animationIdleLeft.setLoop(true);
        animationIdleLeft.setAnchorMode(Animation::AnchorMode::BottomCentered);
        animationIdleLeft.addFrame(ResourcesManager::getInstance()->getAtlas("enemy_idle_left"));

        Animation &animationIdleRight = animationIdle.right;
        animationIdleRight.setInterval(0.1f);
        animationIdleRight.setLoop(true);
        animationIdleRight.setAnchorMode(Animation::AnchorMode::BottomCentered);
        animationIdleRight.addFrame(ResourcesManager::getInstance()->getAtlas("enemy_idle_right"));
        {
            AnimationGroup &animationJump = animation_pool["jump"];

            Animation &animationJumpLeft = animationJump.left;
            animationJumpLeft.setInterval(0.1f);
            animationJumpLeft.setLoop(false);
            animationJumpLeft.setAnchorMode(Animation::AnchorMode::BottomCentered);
            animationJumpLeft.addFrame(ResourcesManager::getInstance()->getAtlas("enemy_jump_left"));

            Animation &animationJumpRight = animationJump.right;
            animationJumpRight.setInterval(0.1f);
            animationJumpRight.setLoop(false);
            animationJumpRight.setAnchorMode(Animation::AnchorMode::BottomCentered);
            animationJumpRight.addFrame(ResourcesManager::getInstance()->getAtlas("enemy_jump_right"));
        }
        {
            AnimationGroup &animationRun = animation_pool["run"];

            Animation &animationRunLeft = animationRun.left;
            animationRunLeft.setInterval(0.05f);
            animationRunLeft.setLoop(true);
            animationRunLeft.setAnchorMode(Animation::AnchorMode::BottomCentered);
            animationRunLeft.addFrame(ResourcesManager::getInstance()->getAtlas("enemy_run_left"));

            Animation &animationRunRight = animationRun.right;
            animationRunRight.setInterval(0.05f);
            animationRunRight.setLoop(true);
            animationRunRight.setAnchorMode(Animation::AnchorMode::BottomCentered);
            animationRunRight.addFrame(ResourcesManager::getInstance()->getAtlas("enemy_run_right"));
        }
        {
            AnimationGroup &animationSquat = animation_pool["squat"];

            Animation &animationSquatLeft = animationSquat.left;
            animationSquatLeft.setInterval(0.05f);
            animationSquatLeft.setLoop(false);
            animationSquatLeft.setAnchorMode(Animation::AnchorMode::BottomCentered);
            animationSquatLeft.addFrame(ResourcesManager::getInstance()->getAtlas("enemy_squat_left"));

            Animation &animationSquatRight = animationSquat.right;
            animationSquatRight.setInterval(0.05f);
            animationSquatRight.setLoop(false);
            animationSquatRight.setAnchorMode(Animation::AnchorMode::BottomCentered);
            animationSquatRight.addFrame(ResourcesManager::getInstance()->getAtlas("enemy_squat_right"));
        }
        {
            AnimationGroup &animationThrowBarb = animation_pool["throw_barb"];

            Animation &animationThrowBarbLeft = animationThrowBarb.left;
            animationThrowBarbLeft.setInterval(0.1f);
            animationThrowBarbLeft.setLoop(false);
            animationThrowBarbLeft.setAnchorMode(Animation::AnchorMode::BottomCentered);
            animationThrowBarbLeft.addFrame(ResourcesManager::getInstance()->getAtlas("enemy_throw_barb_left"));

            Animation &animationThrowBarbRight = animationThrowBarb.right;
            animationThrowBarbRight.setInterval(0.1f);
            animationThrowBarbRight.setLoop(false);
            animationThrowBarbRight.setAnchorMode(Animation::AnchorMode::BottomCentered);
            animationThrowBarbRight.addFrame(ResourcesManager::getInstance()->getAtlas("enemy_throw_barb_right"));
        }
        {
            AnimationGroup &animationThrowSilk = animation_pool["throw_silk"];

            Animation &animationThrowSilkLeft = animationThrowSilk.left;
            animationThrowSilkLeft.setInterval(0.1f);
            animationThrowSilkLeft.setLoop(true);
            animationThrowSilkLeft.setAnchorMode(Animation::AnchorMode::BottomCentered);
            animationThrowSilkLeft.addFrame(ResourcesManager::getInstance()->getAtlas("enemy_throw_silk_left"));

            Animation &animationThrowSilkRight = animationThrowSilk.right;
            animationThrowSilkRight.setInterval(0.1f);
            animationThrowSilkRight.setLoop(true);
            animationThrowSilkRight.setAnchorMode(Animation::AnchorMode::BottomCentered);
            animationThrowSilkRight.addFrame(ResourcesManager::getInstance()->getAtlas("enemy_throw_silk_right"));
        }
        {
            AnimationGroup &animationThrowSword = animation_pool["throw_sword"];

            Animation &animationThrowSwordLeft = animationThrowSword.left;
            animationThrowSwordLeft.setInterval(0.05f);
            animationThrowSwordLeft.setLoop(false);
            animationThrowSwordLeft.setAnchorMode(Animation::AnchorMode::BottomCentered);
            animationThrowSwordLeft.addFrame(ResourcesManager::getInstance()->getAtlas("enemy_throw_sword_left"));

            Animation &animationThrowSwordRight = animationThrowSword.right;
            animationThrowSwordRight.setInterval(0.05f);
            animationThrowSwordRight.setLoop(false);
            animationThrowSwordRight.setAnchorMode(Animation::AnchorMode::BottomCentered);
            animationThrowSwordRight.addFrame(ResourcesManager::getInstance()->getAtlas("enemy_throw_sword_right"));
        }
    }
    {
        animation_silk.setInterval(0.1f);
        animation_silk.setLoop(false);
        animation_silk.setAnchorMode(Animation::AnchorMode::Centered);
        animation_silk.addFrame(ResourcesManager::getInstance()->getAtlas("silk"));

        Animation &animationDashInAirLeft = animation_dash_in_air_vfx.left;
        animationDashInAirLeft.setInterval(0.1f);
        animationDashInAirLeft.setLoop(false);
        animationDashInAirLeft.setAnchorMode(Animation::AnchorMode::Centered);
        animationDashInAirLeft.addFrame(ResourcesManager::getInstance()->getAtlas("enemy_vfx_dash_in_air_left"));

        Animation &animationDashInAirRight = animation_dash_in_air_vfx.right;
        animationDashInAirRight.setInterval(0.1f);
        animationDashInAirRight.setLoop(false);
        animationDashInAirRight.setAnchorMode(Animation::AnchorMode::Centered);
        animationDashInAirRight.addFrame(ResourcesManager::getInstance()->getAtlas("enemy_vfx_dash_in_air_right"));

        Animation &animationDashOnFloorLeft = animation_dash_on_floor_vfx.left;
        animationDashOnFloorLeft.setInterval(0.1f);
        animationDashOnFloorLeft.setLoop(false);
        animationDashOnFloorLeft.setAnchorMode(Animation::AnchorMode::BottomCentered);
        animationDashOnFloorLeft.addFrame(ResourcesManager::getInstance()->getAtlas("enemy_vfx_dash_on_floor_left"));

        Animation &animationDashOnFloorRight = animation_dash_on_floor_vfx.right;
        animationDashOnFloorRight.setInterval(0.1f);
        animationDashOnFloorRight.setLoop(false);
        animationDashOnFloorRight.setAnchorMode(Animation::AnchorMode::BottomCentered);
        animationDashOnFloorRight.addFrame(ResourcesManager::getInstance()->getAtlas("enemy_vfx_dash_on_floor_right"));
    }
    {
        state_machine.registerState("aim", new EnemyAimState());
        state_machine.registerState("dash_in_air", new EnemyDashInAirState());
        state_machine.registerState("dash_on_floor", new EnemyDashOnFloorState());
        state_machine.registerState("dead", new EnemyDeadState());
        state_machine.registerState("fall", new EnemyFallState());
        state_machine.registerState("idle", new EnemyIdleState());
        state_machine.registerState("jump", new EnemyJumpState());
        state_machine.registerState("run", new EnemyRunState());
        state_machine.registerState("squat", new EnemySquatState());
        state_machine.registerState("throw_barb", new EnemyThrowBarbState());
        state_machine.registerState("throw_silk", new EnemyThrowSilkState());
        state_machine.registerState("throw_sword", new EnemyThrowSwordState());

        state_machine.setEntry("idle");
    }
}
Enemy::~Enemy()
{
    CollisionSystem::getInstance()->destroyBox(collision_box_silk);
}
void Enemy::update(float delta)
{
    if (velocity.x >= 0.0001f)
        is_facing_left = (velocity.x < 0);
    Character::update(delta);
    hit_box->setPosition(getLogicCenter());

    if (is_throwing_silk)
    {
        collision_box_silk->setPosition(getLogicCenter());
        animation_silk.setPosition(getLogicCenter());
        animation_silk.update(delta);
    }
    if (is_dashing_in_air || is_dashing_on_floor)
    {
        current_dash_animation->setPosition(is_dashing_in_air ? getLogicCenter() : position);
        current_dash_animation->update(delta);
    }
    for (Barb *barb : barbList)
        barb->update(delta);
    for (Sword *sword : swordList)
        sword->update(delta);
    barbList.erase(std::remove_if(barbList.begin(), barbList.end(), [](Barb *barb){
            bool canRemove=!barb->checkValid();
            if(canRemove)delete barb;
            return canRemove;
        }), barbList.end());
    swordList.erase(std::remove_if(swordList.begin(), swordList.end(), [](Sword *sword){
            bool canRemove=!sword->checkValid();
            if(canRemove)delete sword;
            return canRemove;
        }), swordList.end());
}
void Enemy::draw()
{
    for (Barb *barb : barbList)
        barb->draw();
    for (Sword *sword : swordList)
        sword->draw();
    Character::draw();
    if (is_throwing_silk)animation_silk.draw();
    if (is_dashing_in_air || is_dashing_on_floor)
        current_dash_animation->draw();
}
void Enemy::hurt()
{
    playAudio((L"enemy_hurt_" + std::to_wstring(rangeRandom(1, 3))), false);
}
void Enemy::throwBarbs()
{
    int numNewBarb = rangeRandom(3, 6);
    if (barbList.size() >= 10) numNewBarb = 1;
    int widthGrid = getwidth() / numNewBarb;

    for (int i = 0; i < numNewBarb; i++)
    {
        Barb *newBarb = new Barb();
        int randX = rangeRandom(-widthGrid * 2, widthGrid * (i + 1));
        int randY = rangeRandom(250, 500);
        newBarb->setPosition({(float)randX, (float)randY});
        barbList.push_back(newBarb);
    }
}
void Enemy::throwSword()
{
    Sword *newSword = new Sword(getLogicCenter(), is_facing_left);

    swordList.push_back(newSword);
}
void Enemy::dash()
{
    if (is_dashing_in_air)
        current_dash_animation = velocity.x < 0 ? 
        &animation_dash_in_air_vfx.left : &animation_dash_in_air_vfx.right;
    else
        current_dash_animation = velocity.x < 0 ?
         &animation_dash_on_floor_vfx.left : &animation_dash_on_floor_vfx.right;
    current_dash_animation->reset();
}
void Enemy::onThrowSilk()
{
    animation_silk.reset();
}
