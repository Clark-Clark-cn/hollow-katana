#include "Character.h"
#include "Collision.h"

Character::Character(){
    hit_box=CollisionSystem::getInstance()->createBox();
    hurt_box=CollisionSystem::getInstance()->createBox();

    timer_invulnerable_status.setWaitTime(1.0f);
    timer_invulnerable_status.setOneShot(true);
    timer_invulnerable_status.setCallback([&](){
        is_invulnerable=false;
    });
    timer_invulnerable_blink.setWaitTime(0.075f);
    timer_invulnerable_blink.setOneShot(false);
    timer_invulnerable_blink.setCallback([&](){
        is_blink_invisible=!is_blink_invisible;
    });
}

Character::~Character(){
    CollisionSystem::getInstance()->destroyBox(hit_box);
    CollisionSystem::getInstance()->destroyBox(hurt_box);
}

void Character::decreaseHp(){
    if(is_invulnerable) return;
    hp-=1;
    std::cout << "decrease hp current hp : " << hp << std::endl;
    if(hp>0)makeInvulnerable();
    hurt();
}

bool Character::input(const ExMessage& msg){return false;}

void Character::update(float delta){
    state_machine.update(delta);

    if(hp<=0) velocity.x=0;
    if(enable_gravity) velocity.y+=GRAVITY*delta;
    position+=velocity*delta;
    if(position.y>=FLOOR_Y){
        position.y=FLOOR_Y;
        velocity.y=0;
    }
    if(position.x<=0)position.x=0;
    if(position.x>=getwidth()) position.x=(float)getwidth();
    hurt_box->setPosition(getLogicCenter());
    timer_invulnerable_status.update(delta);
    if(is_invulnerable)
        timer_invulnerable_blink.update(delta);
    if(!current_animation)return;
    Animation& animation=(is_facing_left
        ? current_animation->left : current_animation->right);
        animation.update(delta);
        animation.setPosition(position);
}
void Character::draw()
{
    if(!current_animation || (is_invulnerable && is_blink_invisible)) return;
    (is_facing_left ? current_animation->left:current_animation->right).draw();
}
void Character::hurt(){}

void Character::switchState(const std::string& id)
{
    state_machine.switchTo(id);
}
void Character::setAnimation(const std::string& id)
{
    current_animation=&animation_pool[id];

    current_animation->left.reset();
    current_animation->right.reset();
}