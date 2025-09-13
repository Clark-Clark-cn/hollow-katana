#include "Enemy.h"
#include "Player.h"
#include "CharacterManager.h"
#include "BulletTimeManager.h"

CharacterManager *CharacterManager::instance = nullptr;

CharacterManager *CharacterManager::getInstance()
{
    if (!instance)
        instance = new CharacterManager();
    return instance;
}

bool CharacterManager::input(const ExMessage& msg)
{
    return player->input(msg);
}
void CharacterManager::update(float delta)
{
    enemy->update(delta);
    player->update(delta);
}
void CharacterManager::draw()
{
    enemy->draw();
    BulletTimeManager::getInstance()->postProcess();
    player->draw();
}
CharacterManager::CharacterManager()
{
    enemy = new Enemy();
    player = new Player();
}
CharacterManager::~CharacterManager()
{
    delete enemy;
    delete player;
}
