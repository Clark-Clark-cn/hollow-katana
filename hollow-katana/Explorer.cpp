#include "Explorer.h"
#include <iostream>
#include "base.h"

std::shared_ptr<Explorer> Explorer::instance = nullptr;

static const std::vector<Explorer::ImageInfo> imageList = {
    {"background", L"res/background.png"},
    {"ui_heart", L"res/ui_heart.png"},

    {"player_attack_right", L"res/player/attack.png"},
    {"player_dead_right", L"res/player/dead.png"},
    {"player_fall_right", L"res/player/fall.png"},
    {"player_run_right", L"res/player/run.png"},
    {"player_roll_right", L"res/player/roll.png"},

    {"player_vfx_attack_down", L"res/player/vfx_attack_down.png"},
    {"player_vfx_attack_up", L"res/player/vfx_attack_up.png"},
    {"player_vfx_attack_left", L"res/player/vfx_attack_left.png"},
    {"player_vfx_attack_right", L"res/player/vfx_attack_right.png"},
    {"player_vfx_jump", L"res/player/vfx_jump.png"},
    {"player_vfx_land", L"res/player/vfx_land.png"},
};
static const std::vector<Explorer::AtlasInfo> atlasList = {
    {"barb_break", L"res/enemy/barb_break/%d.png", 3},
    {"barb_loose", L"res/enemy/barb_loose/%d.png", 5},
    {"silk", L"res/enemy/silk/%d.png", 9},
    {"sword_left", L"res/enemy/sword/%d.png", 3},

    {"enemy_aim_left", L"res/enemy/aim/%d.png", 9},
    {"enemy_dash_in_air_left", L"res/enemy/dash_in_air/%d.png", 2},
    {"enemy_dash_on_floor_left", L"res/enemy/dash_on_floor/%d.png", 2},
    {"enemy_fall_left", L"res/enemy/fall/%d.png", 4},
    {"enemy_idle_left", L"res/enemy/idle/%d.png", 6},
    {"enemy_jump_left", L"res/enemy/jump/%d.png", 8},
    {"enemy_run_left", L"res/enemy/run/%d.png", 8},
    {"enemy_squat_left", L"res/enemy/squat/%d.png", 10},
    {"enemy_throw_barb_left", L"res/enemy/throw_barb/%d.png", 8},
    {"enemy_throw_silk_left", L"res/enemy/throw_silk/%d.png", 17},
    {"enemy_throw_sword_left", L"res/enemy/throw_sword/%d.png", 16},

    {"enemy_vfx_dash_in_air_left", L"res/enemy/vfx_dash_in_air/%d.png", 5},
    {"enemy_vfx_dash_on_floor_left", L"res/enemy/vfx_dash_on_floor/%d.png", 6},
};

void Explorer::flipImage(IMAGE *src, IMAGE *dest, int num)
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

void Explorer::flipImage(const std::string& id, const std::string& dstID, int num)
{
    IMAGE* src = getImage(id);
    IMAGE* dest = new IMAGE();
    if (src && dest)
    {
        flipImage(src, dest, num);
    }
    images[dstID] = dest;
}

void Explorer::flipAtlas(const std::string& id, const std::string& dstID)
{
    Atlas* src = getAtlas(id);
    Atlas* dest = new Atlas();
    for(int i=0; i<src->size(); i++)
    {
        IMAGE* srcImage = src->getImage(i);
        IMAGE destImage;
        flipImage(srcImage, &destImage);
        dest->addImage(destImage);
    }
    atlases[dstID] = dest;
}

void Explorer::load()
{
    for (const auto &info : imageList)
    {
        IMAGE *img = new IMAGE();
        loadimage(img, info.path.c_str());
        if (!checkImageValid(img))
        {
            std::wcerr << L"Failed to load image: " << info.path << std::endl;
            MessageBoxW(nullptr, (L"Failed to load image: " + info.path).c_str(), L"Error", MB_OK | MB_ICONERROR);
            delete img;
            continue;
        }
        images[info.ID] = img;
    }

    for (const auto &info : atlasList)
    {
        Atlas *atlas = new Atlas();
        atlas->loadimage(info.path, info.frameCount);
        for (auto i = 0; i < atlas->size(); ++i)
        {
            if (!checkImageValid(atlas->getImage(i)))
            {
                std::wcerr << L"Failed to load atlas: " << info.path << std::endl;
                MessageBoxW(nullptr, (L"Failed to load atlas: " + info.path).c_str(), L"Error", MB_OK | MB_ICONERROR);
                delete atlas;
                atlas = nullptr;
                break;
            }
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

    loadAudio(L"res/audio/bgm.mp3", L"bgm");
    loadAudio(L"res/audio/barb_break.mp3", L"barb_break");
    loadAudio(L"res/audio/bullet_time.mp3", L"bullet_time");

    loadAudio(L"res/audio/enemy_dash.mp3", L"enemy_dash");
    loadAudio(L"res/audio/enemy_run.mp3", L"enemy_run");
    loadAudio(L"res/audio/enemy_hurt_1.mp3", L"enemy_hurt_1");
    loadAudio(L"res/audio/enemy_hurt_2.mp3", L"enemy_hurt_2");
    loadAudio(L"res/audio/enemy_hurt_3.mp3", L"enemy_hurt_3");
    loadAudio(L"res/audio/enemy_throw_barbs.mp3", L"enemy_throw_barbs");
    loadAudio(L"res/audio/enemy_throw_silk.mp3", L"enemy_throw_silk");
    loadAudio(L"res/audio/enemy_throw_sword.mp3", L"enemy_throw_sword");

    loadAudio(L"res/audio/player_attack_1.mp3", L"player_attack_1");
    loadAudio(L"res/audio/player_attack_2.mp3", L"player_attack_2");
    loadAudio(L"res/audio/player_attack_3.mp3", L"player_attack_3");
    loadAudio(L"res/audio/player_dead.mp3", L"player_dead");
    loadAudio(L"res/audio/player_hurt.mp3", L"player_hurt");
    loadAudio(L"res/audio/player_jump.mp3", L"player_jump");
    loadAudio(L"res/audio/player_land.mp3", L"player_land");
    loadAudio(L"res/audio/player_roll.mp3", L"player_roll");
    loadAudio(L"res/audio/player_run.mp3", L"player_run");
}

Atlas *Explorer::getAtlas(const std::string &id) const
{
    const auto &it = atlases.find(id);
    return it != atlases.end() ? it->second : nullptr;
}

IMAGE *Explorer::getImage(const std::string &id) const
{
    const auto &it = images.find(id);
    return it != images.end() ? it->second : nullptr;
}