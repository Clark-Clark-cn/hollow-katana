#pragma once

#include "Vector2.h"
#include <functional>
#include <vector>

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
    Vector2 position; //center
    Vector2 size;
    bool enabled = true;
    std::function<void()> on_collision;
    CollisionLayer src = CollisionLayer::None;
    CollisionLayer dest = CollisionLayer::None;
    CollisionBox()=default;
    ~CollisionBox()=default;
public:
    void setEnabled(bool enabled){
        this->enabled=enabled;
    }
    void setLayerSrc(CollisionLayer layer){
        this->src=layer;
    }
    void setLayerDst(CollisionLayer layer){
        this->dest=layer;
    }
    void setOnCollision(std::function<void()> callback){
        this->on_collision=callback;
    }
    void setPosition(const Vector2& position){
        this->position=position;
    }
    void setSize(const Vector2& size){
        this->size=size;
    }
    const Vector2& getPosition()const{
        return position;
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
    static CollisionSystem* getInstance();
	static void restart()
	{
		if (instance)
			delete instance;
		instance = nullptr;
	}
    void processCollisions() const;
    void debugDraw();
    CollisionBox* createBox();
    void destroyBox(CollisionBox* box);
};