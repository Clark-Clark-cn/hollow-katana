#include "Collision.h"
#undef max
#undef min

CollisionSystem* CollisionSystem::instance = nullptr;

CollisionBox* CollisionSystem::createBox()
{
    CollisionBox* box = new CollisionBox();
    boxes.push_back(box);
    return box;
}

void CollisionSystem::destoryBox(CollisionBox* box)
{
    boxes.erase(std::remove(boxes.begin(), boxes.end(), box), boxes.end());
    delete box;
}

void CollisionSystem::processCollisions(){
    for(const auto& src:boxes)
    {
        if(!src->enabled) continue;
        if(src->layer_src==CollisionLayer::None) continue;
        for(const auto& dst:boxes)
        {
            if(src==dst) continue;
            if(!dst->enabled) continue;
            if(src->layer_dst!=dst->layer_src) continue;

            bool is_collision_x = (std::max(src->pos.x+src->size.x/2,dst->pos.x+dst->size.x/2) - std::min(src->pos.x-src->size.x/2,dst->pos.x-dst->size.x/2)) <= src->size.x+dst->size.x;
            bool is_collision_y = (std::max(src->pos.y+src->size.y/2,dst->pos.y+dst->size.y/2) - std::min(src->pos.y-src->size.y/2,dst->pos.y-dst->size.y/2)) <= src->size.y+dst->size.y;
            if(is_collision_x && is_collision_y)
            {
                if(dst->on_collision) dst->on_collision(src);
            }
        }
    }
}