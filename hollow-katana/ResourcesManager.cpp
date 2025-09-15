#include "ResourcesManager.h"
#include <iostream>
#include "base.h"
#include "Config.h"

ResourcesManager *ResourcesManager::instance = nullptr;

ResourcesManager *ResourcesManager::getInstance()
{
    if (!instance)
    {
        instance = new ResourcesManager();
    }
    return instance;
}

void ResourcesManager::flipImage(IMAGE *src, IMAGE *dest, int num)
{
    int w = src->getwidth();
    int h = src->getheight();
    int w_frame = w / num;
    Resize(dest, w, h);
    DWORD *src_buffer = GetImageBuffer(src);
    DWORD *dest_buffer = GetImageBuffer(dest);
    for (int i = 0; i < num; ++i)
    {
        int x_left = i * w_frame;
        int x_right = x_left + w_frame;
        for (int j = 0; j < h; ++j)
        {
            for (int x = x_left; x < x_right; ++x)
            {
                int src_index = j * w + x;
                int dest_index = j * w + x_right - (x - x_left);
                dest_buffer[dest_index] = src_buffer[src_index];
            }
        }
    }
}

void ResourcesManager::flipImage(const std::string &id, const std::string &dstID, int num)
{
    IMAGE *src = getImage(id);
    IMAGE *dest = new IMAGE();
    if (src && dest)
    {
        flipImage(src, dest, num);
    }
    images[dstID] = dest;
}

void ResourcesManager::flipAtlas(const std::string &id, const std::string &dstID)
{
    Atlas *src = getAtlas(id);
    Atlas *dest = new Atlas();
    for (int i = 0; i < src->size(); i++)
    {
        IMAGE *srcImage = src->getImage(i);
        IMAGE destImage;
        flipImage(srcImage, &destImage);
        dest->addImage(destImage);
    }
    atlases[dstID] = dest;
}

void ResourcesManager::load()
{
    for (const auto &info : Config::getInstance()->get("res.imgs").asImageInfo())
    {
        IMAGE *img = new IMAGE();
        loadimage(img, info.path.c_str());
        if (!checkImageValid(img))
            throw info.path;
        images[info.ID] = img;
    }

    for (const auto &info : Config::getInstance()->get("res.atlas").asAtlasInfo())
    {
        Atlas *atlas = new Atlas();
        atlas->loadimage(info.path, info.frameCount);
        for (auto i = 0; i < atlas->size(); ++i)
        {
            if (!checkImageValid(atlas->getImage(i)))
                throw info.path;
        }
        atlases[info.ID] = atlas;
    }
    flipImage("player_attack_right", "player_attack_left", 5);
    flipImage("player_dead_right", "player_dead_left", 6);
    flipImage("player_fall_right", "player_fall_left", 5);
    flipImage("player_idle_right", "player_idle_left", 5);
    flipImage("player_jump_right", "player_jump_left", 5);
    flipImage("player_run_right", "player_run_left", 10);
    flipImage("player_roll_right", "player_roll_left", 7);

    flipAtlas("sword_left", "sword_right");
    flipAtlas("enemy_aim_left", "enemy_aim_right");
    flipAtlas("enemy_dash_in_air_left", "enemy_dash_in_air_right");
    flipAtlas("enemy_dash_on_floor_left", "enemy_dash_on_floor_right");
    flipAtlas("enemy_fall_left", "enemy_fall_right");
    flipAtlas("enemy_idle_left", "enemy_idle_right");
    flipAtlas("enemy_jump_left", "enemy_jump_right");
    flipAtlas("enemy_run_left", "enemy_run_right");
    flipAtlas("enemy_squat_left", "enemy_squat_right");
    flipAtlas("enemy_throw_barb_left", "enemy_throw_barb_right");
    flipAtlas("enemy_throw_silk_left", "enemy_throw_silk_right");
    flipAtlas("enemy_throw_sword_left", "enemy_throw_sword_right");
    flipAtlas("enemy_vfx_dash_in_air_left", "enemy_vfx_dash_in_air_right");
    flipAtlas("enemy_vfx_dash_on_floor_left", "enemy_vfx_dash_on_floor_right");

    for(const auto& audio : Config::getInstance()->get("res.audio").asAudioInfo())
    {
        loadAudio(audio.path.c_str(), audio.ID.c_str());
    }
}

Atlas *ResourcesManager::getAtlas(const std::string &id) const
{
    const auto &it = atlases.find(id);
    return it != atlases.end() ? it->second : nullptr;
}

IMAGE *ResourcesManager::getImage(const std::string &id) const
{
    const auto &it = images.find(id);
    return it != images.end() ? it->second : nullptr;
}