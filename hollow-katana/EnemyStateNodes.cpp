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
    timer.setOneShot(true);
    timer.setCallback([&](){
        Enemy*enemy=(Enemy*)CharacterManager::getInstance()->getEnemy();

        int randNum=rangeRandom(0,100);
        if(enemy->getHp()>5)
        {
            if(randNum<=25)
            {
                if(!enemy->isOnFloor())
                enemy->switchState("fall");
                else
                enemy->switchState("jump");      //  25%
            }
            else if(randNum<=50)
            {
                if(!enemy->isOnFloor())
                    enemy->switchState("fall");
                else
                    enemy->switchState("run");      //  25%
            }
            else if(randNum<=80)
            enemy->switchState("squat");      //  30%
            else if(randNum<=90)
             enemy->switchState("throw_silk");  //10%
            else enemy->switchState("throw_sword"); //10%
        }
        else
        {
            if(randNum<=25)
            {
                if(!enemy->isOnFloor())
                    enemy->switchState("fall");
                else
                    enemy->switchState("jump");      //  25%
            }
            else if(randNum<=60)
                 enemy->switchState("throw_sword");      //  35%
            else if(randNum<=70)
                 enemy->switchState("throw_silk");  //  10%
            else if(randNum<=90)
                 enemy->switchState("throw_barb");  //  20%
            else enemy->switchState("squat");  //  10%
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
    if (enemy->getHp() <= 0)
        enemy->switchState("dead");
    else if (enemy->getVelocity().y > 0)
    {
        int randNum = rangeRandom(0, 100);
        if (enemy->getHp() > 5)
        {
            if (randNum <= 50)
                enemy->switchState("aim"); //  50%
            else if (randNum <= 80)
                enemy->switchState("fall"); //  30%
            else
                enemy->switchState("throw_silk"); //  20%
        }
        else
        {
            if (randNum <= 50)
                enemy->switchState("throw_silk"); //  50%
            else if (randNum <= 80)
                enemy->switchState("fall"); //  30%
            else
                enemy->switchState("aim"); //  20%
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
    auto enemy = CharacterManager::getInstance()->getEnemy();
    const Vector2 &posEnemy = enemy->getPosition();
    const Vector2 &posPlayer = CharacterManager::getInstance()->getPlayer()->getPosition();
    enemy->setVelocity({posEnemy.x < posPlayer.x ? SPEED_RUN : -SPEED_RUN, 0});

    if (enemy->getHp() <= 0)
        enemy->switchState("dead");
    else if (abs(posEnemy.x - posPlayer.x) <= MIN_DIS)
    {
        int randNum = rangeRandom(0, 100);
        if (enemy->getHp() > 5)
        {
            if (randNum <= 75)
                enemy->switchState("squat"); //  75%
            else
                enemy->switchState("throw_silk"); //  25%
        }
        else
        {
            if (randNum <= 75)
                enemy->switchState("throw_silk"); //  75%

            else
                enemy->switchState("squat"); //  25%
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
    CharacterManager::getInstance()->getEnemy()->setAnimation("squat");

    Enemy *enemy = (Enemy *)CharacterManager::getInstance()->getEnemy();
    enemy->setFacingLeft(enemy->getPosition().x > CharacterManager::getInstance()->getPlayer()->getPosition().x);
    timer.restart();
}
void EnemySquatState::update(float delta)
{
    Enemy *enemy = (Enemy *)CharacterManager::getInstance()->getEnemy();
    timer.update(delta);
    if (enemy->getHp() <= 0)
        enemy->switchState("dead");
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
        if(enemy->getHp() > 5)
        {
            if(randNum<=50)
                enemy->switchState("squat");        //  50%
            else if(randNum<=80)
                enemy->switchState("jump");        //  30%
            else
                enemy->switchState("idle");            //  20%
        }
        else
        {
            if(randNum<=50)
                enemy->switchState("jump");       //  50%
            else if(randNum<=80)
                enemy->switchState("throw_silk");       //  30%
            else
                enemy->switchState("idle");       //  20%
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