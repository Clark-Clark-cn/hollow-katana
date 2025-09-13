#include "Collision.h"
#include <graphics.h>
#undef max
#undef min

CollisionSystem *CollisionSystem::instance = nullptr;

CollisionSystem *CollisionSystem::getInstance()
{
    if (!instance)
    {
        instance = new CollisionSystem();
    }
    return instance;
}

CollisionBox *CollisionSystem::createBox()
{
    CollisionBox *box = new CollisionBox();
    boxes.push_back(box);
    return box;
}

void CollisionSystem::destroyBox(CollisionBox *box)
{
    boxes.erase(std::remove(boxes.begin(), boxes.end(), box), boxes.end());
    delete box;
}

void CollisionSystem::processCollisions() const
{
    for (const auto &src : boxes)
    {
        if (!src->enabled||src->dest==CollisionLayer::None)
            continue;
        for (const auto &dst : boxes)
        {
            if (!dst->enabled||src==dst||src->dest != dst->src)
                continue;

            bool is_collision_x = (std::max(src->position.x + src->size.x / 2, dst->position.x + dst->size.x / 2) - 
                std::min(src->position.x - src->size.x / 2, dst->position.x - dst->size.x / 2)) <= src->size.x + dst->size.x;
            bool is_collision_y = (std::max(src->position.y + src->size.y / 2, dst->position.y + dst->size.y / 2) -
                std::min(src->position.y - src->size.y / 2, dst->position.y - dst->size.y / 2)) <= src->size.y + dst->size.y;
            if (is_collision_x && is_collision_y)
            {
                if (dst->on_collision)
                    dst->on_collision();
            }
        }
    }
}

void CollisionSystem::debugDraw()
{
    for (const auto &box : boxes)
    {
        setlinecolor(box->enabled ? RED : BLUE);
        rectangle(box->position.x - box->size.x / 2,
                  box->position.y - box->size.y / 2,
                  box->position.x + box->size.x / 2,
                  box->position.y + box->size.y / 2);
    }
}
 