#include "enemy.h"
#include "characterManager.h"
#include "enemyStateNodes.h"

EnemyAimState::EnemyAimState()
{
    timer.setOneShot(true);
    timer.setWaitTime(0.5f);
    timer.setCallback([&](){
        Enemy*enemy=(Enemy*)CharacterManager::getInstance()->getEnemy();
        enemy->setEnableGravity(true);
        enemy->switchState("dash_in_air"); 
    });
}
void EnemyAimState::enter()
{
    CharacterManager::getInstance()->getEnemy()->setAnimation("aim");
    Enemy *enemy = (Enemy *)CharacterManager::getInstance()->getEnemy();
    enemy->setEnableGravity(false);
    enemy->setVelocity({0, 0});

    timer.restart();
}
void EnemyAimState::update(float delta)
{
    Enemy *enemy = (Enemy *)CharacterManager::getInstance()->getEnemy();
    timer.update(delta);
    if (enemy->getHp() <= 0)
        enemy->switchState("dead");
}

void EnemyDashInAirState::enter()
{
    playAudio(L"enemy_dash", false);
    CharacterManager::getInstance()->getEnemy()->setAnimation("dash_in_air");

    Enemy *enemy = (Enemy *)CharacterManager::getInstance()->getEnemy();
    const Character *player = CharacterManager::getInstance()->getPlayer();
    Vector2 posTarget = {player->getPosition().x, player->getFloorY()};
    enemy->setVelocity((posTarget - enemy->getPosition()).normalize() * SPEED_DASH);
    enemy->setDashingInAir(true);
    enemy->setEnableGravity(false);
    enemy->dash();

}
void EnemyDashInAirState::update(float delta)
{
    Enemy *enemy = (Enemy *)CharacterManager::getInstance()->getEnemy();
    if (enemy->getHp() <= 0)
        enemy->switchState("dead");
    else if (enemy->isOnFloor())
        enemy->switchState("idle");
}
void EnemyDashInAirState::exit()
{
    Enemy *enemy = (Enemy *)CharacterManager::getInstance()->getEnemy();
    enemy->setEnableGravity(true);
    enemy->setDashingInAir(false);
}

EnemyDashOnFloorState::EnemyDashOnFloorState()
{
    timer.setOneShot(true);
    timer.setWaitTime(0.5f);
    timer.setCallback([&](){
        Enemy*enemy=(Enemy*)CharacterManager::getInstance()->getEnemy();
        enemy->setDashingOnFloor(false); 
    });
}
void EnemyDashOnFloorState::enter()
{
    playAudio(L"enemy_dash", false);
    CharacterManager::getInstance()->getEnemy()->setAnimation("dash_on_floor");

    Enemy *enemy = (Enemy*)CharacterManager::getInstance()->getEnemy();

    enemy->setVelocity({enemy->isFacingLeft() ? -SPEED_DASH : SPEED_DASH, 0});
    enemy->setDashingOnFloor(true);
    enemy->dash();
    timer.restart();
}
void EnemyDashOnFloorState::update(float delta)
{
    Enemy *enemy = (Enemy *)CharacterManager::getInstance()->getEnemy();
    timer.update(delta);
    if (enemy->getHp() <= 0)
        enemy->switchState("dead");
    else if (!enemy->getDashingOnFloor())
        enemy->switchState("idle");
}
extern bool restart;
void EnemyDeadState::enter()
{
    MessageBox(GetHWnd(), L"Great! That's it!.", L"Succeed!", MB_OK|MB_SYSTEMMODAL|MB_ICONEXCLAMATION);
    restart = true;
}

void EnemyFallState::enter(){
    CharacterManager::getInstance()->getEnemy()->setAnimation("fall");
}
void EnemyFallState::update(float delta)
{
    Enemy *enemy = (Enemy *)CharacterManager::getInstance()->getEnemy();

    if (enemy->getHp() <= 0)
        enemy->switchState("dead");
    else if (enemy->isOnFloor())
        enemy->switchState("idle");
}
EnemyIdleState::EnemyIdleState()
{
    static const int max_hp=Config::getInstance()->get("enemy.max_hp");
    static const int healthy_jump_prob=Config::getInstance()->get("enemy.probility.healthy.idle.jump");
    static const int healthy_run_prob=Config::getInstance()->get("enemy.probility.healthy.idle.run");
    static const int healthy_squat_prob=Config::getInstance()->get("enemy.probility.healthy.idle.squat");
    static const int healthy_throw_silk_prob=100-(healthy_jump_prob+healthy_run_prob+healthy_squat_prob);

    static const int unhealthy_jump_prob=Config::getInstance()->get("enemy.probility.unhealthy.idle.jump");
    static const int unhealthy_squat_prob=Config::getInstance()->get("enemy.probility.unhealthy.idle.squat");
    static const int unhealthy_throw_silk_prob=Config::getInstance()->get("enemy.probility.unhealthy.idle.throw_silk");
    static const int unhealthy_throw_barb_prob=Config::getInstance()->get("enemy.probility.unhealthy.idle.throw_barb");

    timer.setOneShot(true);
    timer.setCallback([&](){
        Enemy*enemy=(Enemy*)CharacterManager::getInstance()->getEnemy();

        int randNum=rangeRandom(0,100);
        if(enemy->getHp()>max_hp/2)
        {
            if(randNum<=healthy_jump_prob)
            {
                if(!enemy->isOnFloor())
                enemy->switchState("fall");
                else
                enemy->switchState("jump");
            }
            else if(randNum<=healthy_run_prob+healthy_jump_prob)
            {
                if(!enemy->isOnFloor())
                    enemy->switchState("fall");
                else
                    enemy->switchState("run");
            }
            else if(randNum<=healthy_run_prob+healthy_jump_prob+healthy_squat_prob)
            enemy->switchState("squat");
            else if(randNum<=healthy_throw_silk_prob+healthy_run_prob+healthy_jump_prob)
             enemy->switchState("throw_silk");
            else enemy->switchState("throw_sword");
        }
        else
        {
            if(randNum<=unhealthy_jump_prob)
            {
                if(!enemy->isOnFloor())
                    enemy->switchState("fall");
                else
                    enemy->switchState("jump");
            }
            else if(randNum<=unhealthy_squat_prob+unhealthy_jump_prob)
                enemy->switchState("squat");
            else if(randNum<=unhealthy_throw_silk_prob+unhealthy_squat_prob+unhealthy_jump_prob)
                 enemy->switchState("throw_silk");
            else if(randNum<=unhealthy_throw_barb_prob+unhealthy_throw_silk_prob+unhealthy_squat_prob+unhealthy_jump_prob)
                 enemy->switchState("throw_barb");
            else
                 enemy->switchState("throw_sword");
        }
    });
}
void EnemyIdleState::enter()
{
    CharacterManager::getInstance()->getEnemy()->setAnimation("idle");
    Enemy *enemy = (Enemy *)CharacterManager::getInstance()->getEnemy();
    enemy->setVelocity({0, 0});
    float waitTime = 0;
    if (enemy->getHp() > 5)
        waitTime = rangeRandom(0, 2) * 0.25f; // 0.0s~0.5s
    else
        waitTime = rangeRandom(0, 1) * 0.25f; // 0.0s~0.25s
    timer.setWaitTime(waitTime);
    timer.restart();
}
void EnemyIdleState::update(float delta)
{
    Enemy *enemy = (Enemy *)CharacterManager::getInstance()->getEnemy();
    timer.update(delta);
    if (enemy->getHp() <= 0)
        enemy->switchState("dead");
    else if (enemy->getVelocity().y > 0)
        enemy->switchState("fall");
}
void EnemyIdleState::exit()
{
    Enemy *enemy = (Enemy *)CharacterManager::getInstance()->getEnemy();
    enemy->setFacingLeft(enemy->getPosition().x > CharacterManager::getInstance()->getPlayer()->getPosition().x);
}
void EnemyJumpState::enter()
{
    CharacterManager::getInstance()->getEnemy()->setAnimation("jump");

    Enemy *enemy = (Enemy *)CharacterManager::getInstance()->getEnemy();
    enemy->setVelocity({0, -SPEED_JUMP});
}
void EnemyJumpState::update(float delta)
{
    Enemy *enemy = (Enemy *)CharacterManager::getInstance()->getEnemy();
    static const int max_hp = Config::getInstance()->get("enemy.max_hp");
    static const int healthy_aim_prob=Config::getInstance()->get("enemy.probility.healthy.jump.aim");
    static const int healthy_fall_prob=Config::getInstance()->get("enemy.probility.healthy.jump.fall");
    static const int unhealthy_aim_prob=Config::getInstance()->get("enemy.probility.unhealthy.jump.aim");
    static const int unhealthy_fall_prob=Config::getInstance()->get("enemy.probility.unhealthy.jump.fall");
    if (enemy->getHp() <= 0)
        enemy->switchState("dead");
    else if (enemy->getVelocity().y > 0)
    {
        int randNum = rangeRandom(0, 100);
        if (enemy->getHp() > max_hp/2)
        {
            if (randNum <= healthy_aim_prob)
                enemy->switchState("aim");
            else if (randNum <= healthy_fall_prob+healthy_aim_prob)
                enemy->switchState("fall");
            else
                enemy->switchState("throw_silk");
        }
        else
        {
            if (randNum <= unhealthy_aim_prob)
                enemy->switchState("aim");
            else if (randNum <= unhealthy_fall_prob+unhealthy_aim_prob)
                enemy->switchState("fall");
            else
                enemy->switchState("throw_silk");
        }
    }
}
void EnemyRunState::enter()
{
    playAudio(L"enemy_run", true);
    CharacterManager::getInstance()->getEnemy()->setAnimation("run");
}
void EnemyRunState::update(float delta)
{
    static const int healthy_squat_prob=Config::getInstance()->get("enemy.probility.healthy.run.squat");
    static const int unhealthy_squat_prob=Config::getInstance()->get("enemy.probility.unhealthy.run.squat");
    static const int max_hp=Config::getInstance()->get("enemy.max_hp");
    auto enemy = CharacterManager::getInstance()->getEnemy();
    const Vector2 &posEnemy = enemy->getPosition();
    const Vector2 &posPlayer = CharacterManager::getInstance()->getPlayer()->getPosition();
    enemy->setVelocity({posEnemy.x < posPlayer.x ? SPEED_RUN : -SPEED_RUN, 0});

    if (enemy->getHp() <= 0)
        enemy->switchState("dead");
    else if (abs(posEnemy.x - posPlayer.x) <= MIN_DIS)
    {
        int randNum = rangeRandom(0, 100);
        if (enemy->getHp() > max_hp/2)
        {
            if (randNum <= healthy_squat_prob)
                enemy->switchState("squat");
            else
                enemy->switchState("throw_silk");
        }
        else
        {
            if (randNum <= unhealthy_squat_prob)
                enemy->switchState("squat");
            else
                enemy->switchState("throw_silk");
        }
        stopAudio(L"enemy_run");
    }
}
void EnemyRunState::exit()
{
    Enemy *enemy = (Enemy *)CharacterManager::getInstance()->getEnemy();
    enemy->setVelocity({0, 0});
}
EnemySquatState::EnemySquatState()
{
    timer.setOneShot(true);
    timer.setWaitTime(0.5f);
    timer.setCallback([&](){
        Enemy*enemy=(Enemy*)CharacterManager::getInstance()->getEnemy();
        enemy->switchState("dash_on_floor"); 
    });
}
void EnemySquatState::enter()
{
    static const Vector2 hit_box_size_squat=Config::getInstance()->get("enemy.hit_box_size_squat");
    static const Vector2 hit_box_offset_squat=Config::getInstance()->get("enemy.hit_box_offset_squat");
    CharacterManager::getInstance()->getEnemy()->setAnimation("squat");

    Enemy *enemy = (Enemy *)CharacterManager::getInstance()->getEnemy();
    enemy->setFacingLeft(enemy->getPosition().x > CharacterManager::getInstance()->getPlayer()->getPosition().x);
    enemy->getHitBox()->setSize(hit_box_size_squat);
    enemy->setBoxOffset(hit_box_offset_squat);
    timer.restart();
}
void EnemySquatState::update(float delta)
{
    Enemy *enemy = (Enemy *)CharacterManager::getInstance()->getEnemy();
    timer.update(delta);
    if (enemy->getHp() <= 0)
        enemy->switchState("dead");
}
void EnemySquatState::exit()
{
    static const Vector2 hit_box_size=Config::getInstance()->get("enemy.hit_box_size");
    Enemy *enemy = (Enemy *)CharacterManager::getInstance()->getEnemy();
    enemy->getHitBox()->setSize(hit_box_size);
    enemy->setBoxOffset({0,0});
}
EnemyThrowBarbState::EnemyThrowBarbState()
{
    timer.setOneShot(true);
    timer.setWaitTime(0.8f);
    timer.setCallback([&](){
        Enemy*enemy=(Enemy*)CharacterManager::getInstance()->getEnemy();
        enemy->throwBarbs();
        enemy->switchState("idle"); 
    });
}
void EnemyThrowBarbState::enter()
{
    playAudio(L"enemy_throw_barbs", false);
    CharacterManager::getInstance()->getEnemy()->setAnimation("throw_barb");
    timer.restart();
}
void EnemyThrowBarbState::update(float delta)
{
    Enemy *enemy = (Enemy *)CharacterManager::getInstance()->getEnemy();
    timer.update(delta);
    if (enemy->getHp() <= 0)
        enemy->switchState("dead");
}
EnemyThrowSilkState::EnemyThrowSilkState()
{
    timer.setOneShot(true);
    timer.setWaitTime(0.9f);
    timer.setCallback([&](){
        Enemy*enemy=(Enemy*)CharacterManager::getInstance()->getEnemy();
        enemy->setEnableGravity(true);
        enemy->setThrowingSilk(false);
        if(!enemy->isOnFloor()&&enemy->getHp()>5 && rangeRandom(0,100)<=25)
            enemy->switchState("aim");
        else if(!enemy->isOnFloor())
            enemy->switchState("fall");
        else
            enemy->switchState("idle");
    });
}
void EnemyThrowSilkState::enter()
{
    playAudio(L"enemy_throw_silk", false);
    CharacterManager::getInstance()->getEnemy()->setAnimation("throw_silk");

    Enemy *enemy = (Enemy*)CharacterManager::getInstance()->getEnemy();
    enemy->setEnableGravity(false);
    enemy->setThrowingSilk(true);
    enemy->setVelocity({0, 0});
    enemy->onThrowSilk();
    timer.restart();
}
void EnemyThrowSilkState::update(float delta)
{
    Enemy *enemy = (Enemy *)CharacterManager::getInstance()->getEnemy();
    timer.update(delta);
    if (enemy->getHp() <= 0)
        enemy->switchState("dead");
}
EnemyThrowSwordState::EnemyThrowSwordState()
{
    static const int max_hp=Config::getInstance()->get("enemy.max_hp");
    static const int healthy_squat_prob=Config::getInstance()->get("enemy.probility.healthy.throw_sword.squat");
    static const int healthy_jump_prob=Config::getInstance()->get("enemy.probility.healthy.throw_sword.jump");
    static const int unhealthy_jump_prob=Config::getInstance()->get("enemy.probility.unhealthy.throw_sword.jump");
    static const int unhealthy_throw_silk_prob=Config::getInstance()->get("enemy.probility.unhealthy.throw_sword.throw_silk");
    timerThrow.setWaitTime(0.65f);
    timerThrow.setOneShot(true);
    timerThrow.setCallback([&](){
        Enemy *enemy = (Enemy *)CharacterManager::getInstance()->getEnemy();
        enemy->throwSword();

        playAudio(L"enemy_throwSword", false);
    });
    timerSwitch.setWaitTime(1.0f);
    timerSwitch.setOneShot(true);
    timerSwitch.setCallback([&]()
    {
        Enemy*enemy=(Enemy*)CharacterManager::getInstance()->getEnemy();

        int randNum = rangeRandom(0, 100);
        if(enemy->getHp() > max_hp/2)
        {
            if(randNum<=healthy_squat_prob)
                enemy->switchState("squat");
            else if(randNum<=healthy_jump_prob+healthy_squat_prob)
                enemy->switchState("jump");
            else
                enemy->switchState("idle");
        }
        else
        {
            if(randNum<=unhealthy_jump_prob)
                enemy->switchState("jump");
            else if(randNum<=unhealthy_jump_prob+unhealthy_throw_silk_prob)
                enemy->switchState("throw_silk");
            else
                enemy->switchState("idle");
        }
    });
}
void EnemyThrowSwordState::enter()
{
    CharacterManager::getInstance()->getEnemy()->setAnimation("throw_sword");
    timerThrow.restart();
    timerSwitch.restart();
}
void EnemyThrowSwordState::update(float delta)
{
    Enemy *enemy = (Enemy *)CharacterManager::getInstance()->getEnemy();
    timerThrow.update(delta);
    timerSwitch.update(delta);
    if (enemy->getHp() <= 0)
        enemy->switchState("dead");
}