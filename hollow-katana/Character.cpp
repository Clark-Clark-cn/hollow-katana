#include "Character.h"
#include "Collision.h"

Character::Character(){
    hitBox=CollisionSystem::getInstance()->createBox();
    hurtBox=CollisionSystem::getInstance()->createBox();

    timerInvulnerableBlink.setWaitTime(1.0f);
    timerInvulnerableBlink.setOneShot(true);
    timerInvulnerableBlink.setCallback([&](){
        isInvulnerable=false;
    });
    timerInvulnerableBlink.setWaitTime(0.075f);
    timerInvulnerableStatus.setOneShot(false);
    timerInvulnerableBlink.setCallback([&](){
        isBlinkVisible=!isBlinkVisible;
    });
}

Character::~Character(){
    CollisionSystem::getInstance()->destoryBox(hitBox);
    CollisionSystem::getInstance()->destoryBox(hurtBox);
}