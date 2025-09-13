#include "sword.h"
#include "resourcesManager.h"
#include "Collision.h"

Sword::Sword(const Vector2& position,bool moveLeft)
{
   
    animation.setInterval(0.1f);
    animation.setLoop(true);
    animation.setAnchorMode(Animation::AnchorMode::Centered);
    animation.addFrame(ResourcesManager::getInstance()->getAtlas(moveLeft?"sword_left":"sword_right"));
    
    collisionBox=CollisionSystem::getInstance()->createBox();
    collisionBox->setLayerSrc(CollisionLayer::None);
    collisionBox->setLayerDst(CollisionLayer::Player);
    collisionBox->setSize({195,10});
    this->position=position;
    this->velocity={moveLeft?-SPEED_MOVE:SPEED_MOVE,0};
}
Sword::~Sword()
{
    CollisionSystem::getInstance()->destroyBox(collisionBox);
}
void Sword::update(float delta)
{
    position+=velocity*delta;
    animation.setPosition(position);
    collisionBox->setPosition(position);
    animation.update(delta); 
    if (position.x<=-200||position.x>=getwidth()+200)
        isValid=false;
}
void Sword::draw()
{
   animation.draw();
}