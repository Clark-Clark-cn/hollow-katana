#include "barb.h"
#include "Collision.h"
#include "ResourcesManager.h"
#include "characterManager.h"

Barb::Barb()
{
    diffPeriod = rangeRandom(0, 6);

    animationLoose.setInterval(0.15f);
    animationLoose.setLoop(true);
    animationLoose.setAnchorMode(Animation::AnchorMode::Centered);
    animationLoose.addFrame(ResourcesManager::getInstance()->getAtlas("barb_loose"));

    animationBreak.setInterval(0.1f);
    animationBreak.setLoop(false);
    animationBreak.setAnchorMode(Animation::AnchorMode::Centered);
    animationBreak.addFrame(ResourcesManager::getInstance()->getAtlas("barb_break"));
    animationBreak.setOnComplete([&](){ isValid = false; });

    collisionBox = CollisionSystem::getInstance()->createBox();
    collisionBox->setLayerSrc(CollisionLayer::Enemy);
    collisionBox->setLayerDst(CollisionLayer::Player);
    collisionBox->setSize({20, 20});
    collisionBox->setOnCollision([&](){ onBreak(); });

    timerIdle.setWaitTime((float)rangeRandom(3, 10));
    timerIdle.setOneShot(true);
    timerIdle.setCallback([&](){
        if (stage == Stage::Idle)
        {
            stage = Stage::Aim;
            basePosition = currentPosition;
        }
    });
    timerAim.setWaitTime(0.75f);
    timerAim.setOneShot(true);
    timerAim.setCallback([&](){
        if(stage==Stage::Aim){
            stage= Stage::Dash;
            const Vector2& playerPos=CharacterManager::getInstance()->getPlayer()->getPosition();
            velocity = (playerPos - currentPosition).normalize() * SPEED_DASH;
        }
    });
}
Barb::~Barb()
{
    CollisionSystem::getInstance()->destroyBox(collisionBox);
}

void Barb::update(float delta)
{
    if (stage == Stage::Idle)
        timerIdle.update(delta);
    else if (stage == Stage::Aim)
        timerAim.update(delta);
    totalDeltaTime += delta;
    switch (stage)
    {
    case Barb::Stage::Idle:
        currentPosition.y = basePosition.y + sin(totalDeltaTime * 2 + diffPeriod) * 30;
        break;
    case Barb::Stage::Aim:
        currentPosition.x = basePosition.x + rangeRandom(-10, 10);
        break;
    case Barb::Stage::Dash:
        currentPosition += velocity * delta;
        if (currentPosition.y >= CharacterManager::getInstance()->getPlayer()->getFloorY())
            onBreak();
        if (currentPosition.y <= 0)
            isValid = false;
        break;
    }
    collisionBox->setPosition(currentPosition);
    currentAnimation = (stage == Stage::Break ? &animationBreak : &animationLoose);
    currentAnimation->setPosition(currentPosition);
    currentAnimation->update(delta);
}
void Barb::draw()
{
    currentAnimation->draw();
}
void Barb::onBreak()
{
    if (stage == Stage::Break)
        return; 

    stage = Stage::Break;
    collisionBox->setEnabled(false);
    playAudio(L"barb_break", false);
}
