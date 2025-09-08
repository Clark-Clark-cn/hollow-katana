#pragma once

#include "Vector2.h"
#include <functional>

enum class CollisionLayer
{
    None,
    Player,
    Enemy
};

class CollisionSystem;

class CollisionBox
{
    friend class CollisionSystem;
    Vector2 pos; //center position
    Vector2 size; //width and height
    bool enabled = true;
    std::function<void(CollisionBox*)> on_collision;
    CollisionLayer layer_src = CollisionLayer::None;
    CollisionLayer layer_dst = CollisionLayer::None;
    CollisionBox()=default;
    ~CollisionBox()=default;
public:
    void setEnabled(bool enabled){
        this->enabled=enabled;
    }
    void setLayerSrc(CollisionLayer layer){
        this->layer_src=layer;
    }
    void setLayerDst(CollisionLayer layer){
        this->layer_dst=layer;
    }
    void setOnCollision(std::function<void(CollisionBox*)> callback){
        this->on_collision=callback;
    }
    void setPosition(const Vector2& pos){
        this->pos=pos;
    }
    void setSize(const Vector2& size){
        this->size=size;
    }
    const Vector2& getPosition()const{
        return pos;
    }
    const Vector2& getSize()const{
        return size;
    }
};

class CollisionSystem
{
    std::vector<CollisionBox*> boxes;
    static CollisionSystem* instance;
    CollisionSystem()=default;
    ~CollisionSystem()=default;
public:
    static CollisionSystem* getInstance(){
        if(!instance){
            instance=new CollisionSystem();
        }
        return instance;
    }
    void processCollisions();
    void debugDraw();
    CollisionBox* createBox();
    void destoryBox(CollisionBox* box);
};