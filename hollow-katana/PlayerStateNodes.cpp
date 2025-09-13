#include "player.h"
#include "characterManager.h"
#include "playerStateNodes.h"

#include "bulletTimeManager.h"

PlayerAttackState::PlayerAttackState()
{
    timer.setWaitTime(0.3f);
    timer.setOneShot(true);
    timer.setCallback([&](){
       Player* player = (Player*)CharacterManager::getInstance()->getPlayer();
       player->setAttacking(false);
        });
}
void PlayerAttackState::enter()
{
    CharacterManager::getInstance()->getPlayer()->setAnimation("attack");

    Player *player = (Player *)CharacterManager::getInstance()->getPlayer();
    player->getHitBox()->setEnabled(true);
    player->setAttacking(true);
    updateHitBoxPosition();
    player->attack();
    timer.restart();
    playAudio(L"player_attack_" + std::to_wstring(rangeRandom(1, 3)), false);
}
void PlayerAttackState::update(float delta)
{
    timer.update(delta);
    updateHitBoxPosition();

    Player *player = (Player*)CharacterManager::getInstance()->getPlayer();

    if (player->getHp() <= 0)
        player->switchState("dead");
    else if (!player->getAttacking())
    {
        if (player->getVelocity().y > 0)
            player->switchState("fall");
        else if (player->getMoveAxis() == 0)
            player->switchState("idle");
        else if (player->isOnFloor() && player->getMoveAxis() != 0)
            player->switchState("run");
    }
}
void PlayerAttackState::exit()
{
    Player *player = (Player *)CharacterManager::getInstance()->getPlayer();
    player->getHitBox()->setEnabled(false);
	player->setAttacking(false);
}
void PlayerAttackState::updateHitBoxPosition()
{
    Player *player = (Player *)CharacterManager::getInstance()->getPlayer();
    Vector2 posCenter = player->getLogicCenter();
    CollisionBox *hitBox = player->getHitBox();
    const Vector2 &sizeHitBox = hitBox->getSize();
    Vector2 posHitBox;
    switch (player->getAttackDir())
    {
    case Player::AttackDir::Up:
        posHitBox = {posCenter.x, posCenter.y - sizeHitBox.y / 2};
        break;
    case Player::AttackDir::Down:
        posHitBox = {posCenter.x, posCenter.y + sizeHitBox.y / 2};
        break;
    case Player::AttackDir::Left:
        posHitBox = {posCenter.x - sizeHitBox.x / 2, posCenter.y};
        break;
    case Player::AttackDir::Right:
        posHitBox = {posCenter.x + sizeHitBox.x / 2, posCenter.y};
        break;
    }
    hitBox->setPosition(posHitBox);
}
extern bool restart;
PlayerDeadState::PlayerDeadState()
{
    timer.setWaitTime(2.0f);
    timer.setOneShot(true);
    timer.setCallback([&]()
    {
        MessageBox(GetHWnd(),L"NO......\n That is Wrong.",L"Failed!",MB_OK|MB_SYSTEMMODAL|MB_ICONWARNING);
		restart = true;
    });
}
void PlayerDeadState::enter()
{
    CharacterManager::getInstance()->getPlayer()->setAnimation("dead");
    playAudio(L"player_dead", false);
}
void PlayerDeadState::update(float delta)
{
    timer.update(delta);
}

void PlayerFallState::enter()
{
    CharacterManager::getInstance()->getPlayer()->setAnimation("fall");
}
void PlayerFallState::update(float delta)
{
    Player *player = (Player *)CharacterManager::getInstance()->getPlayer();
    if (player->getHp() <= 0)
        player->switchState("dead");
    else if(player->isOnFloor()){
        player->switchState("idle");
        player->land();
        playAudio(L"player_land", false);
    }
    else if(player->canAttack())
        player->switchState("attack");
}

void PlayerIdleState::enter()
{
    CharacterManager::getInstance()->getPlayer()->setAnimation("idle");
}
void PlayerIdleState::update(float delta)
{
    Player *player = (Player *)CharacterManager::getInstance()->getPlayer();
    if (player->getHp() <= 0)
        player->switchState("dead");
    else if (player->getVelocity().y > 0)
        player->switchState("fall");
    else if (player->canJump())
        player->switchState("jump");
    else if (player->canAttack())
        player->switchState("attack");
    else if (player->canRoll())
        player->switchState("roll");
    else if (player->getMoveAxis() != 0 && player->isOnFloor())
        player->switchState("run");
}

void PlayerJumpState::enter()
{
    CharacterManager::getInstance()->getPlayer()->setAnimation("jump");
    Player *player = (Player *)CharacterManager::getInstance()->getPlayer();
    player->jump();

    playAudio(L"player_jump", false);
}

void PlayerJumpState::update(float delta)
{
    Player *player = (Player *)CharacterManager::getInstance()->getPlayer();
    if (player->getHp() <= 0)
        player->switchState("dead");
    else if (player->getVelocity().y > 0)
        player->switchState("fall");
    else if (player->canAttack())
        player->switchState("attack");
}
PlayerRollState::PlayerRollState()
{
    timer.setWaitTime(0.35f);
    timer.setOneShot(true);
    timer.setCallback([&](){
       Player* player = (Player*)CharacterManager::getInstance()->getPlayer();
       player->setRolling(false);
    });
}
void PlayerRollState::enter()
{
    CharacterManager::getInstance()->getPlayer()->setAnimation("roll");

    Player *player = (Player *)CharacterManager::getInstance()->getPlayer();
    player->getHurtBox()->setEnabled(false);
    player->setRolling(true);
    player->roll();
    timer.restart();

    playAudio(L"player_roll", false);
}
void PlayerRollState::update(float delta)
{
    timer.update(delta);

    Player *player = (Player *)CharacterManager::getInstance()->getPlayer();

    if (!player->getRolling())
    {
        if (player->getMoveAxis() != 0)
            player->switchState("run");
        else if (player->canJump())
            player->switchState("jump");
        else
            player->switchState("idle");
    
    }
}
void PlayerRollState::exit()
{
    CharacterManager::getInstance()->getPlayer()->getHurtBox()->setEnabled(true);
}

void PlayerRunState::enter()
{
    CharacterManager::getInstance()->getPlayer()->setAnimation("run");
    playAudio(L"player_run", true);
}
void PlayerRunState::update(float delta)
{
    Player *player = (Player *)CharacterManager::getInstance()->getPlayer();
    if (player->getHp() <= 0)
        player->switchState("dead");
    else if (player->getMoveAxis() == 0)
        player->switchState("idle");
    else if (player->canJump())
        player->switchState("jump");
    else if (player->canAttack())
        player->switchState("attack");
    else if (player->canRoll())
        player->switchState("roll");
}
void PlayerRunState::exit()
{
    stopAudio(L"player_run");
}
