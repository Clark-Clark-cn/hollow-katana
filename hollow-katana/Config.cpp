#include "Config.h"
#include <Windows.h>
#include <fstream>
#include <iostream>

Config* Config::instance = nullptr;
Config* Config::default_instance = nullptr;

std::wstring to_wstring(const std::string& str) {
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
    std::wstring wstr(size_needed-1, 0);
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), &wstr[0], size_needed);
    return wstr;
}

std::string to_string(const std::wstring& wstr) {
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
    std::string str(size_needed-1, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), wstr.length(), &str[0], size_needed, nullptr, nullptr);
    return str;
}

const nlohmann::json ImageInfo::toJson()const{
    nlohmann::json j;
    j["id"]=ID;
    j["path"]=to_string(path);
    return j;
}

const ImageInfo& ImageInfo::fromJson(const nlohmann::json& j) {
    if (j.contains("id") && j["id"].is_string()) ID = j["id"].get<std::string>();
    if (j.contains("path") && j["path"].is_string()) path = to_wstring(j["path"].get<std::string>());
    return *this;
}

const nlohmann::json AtlasInfo::toJson()const{
    nlohmann::json j;
    j["id"]=ID;
    j["path"]=to_string(path);
    j["number"]=frameCount;
    return j;
}

const AtlasInfo& AtlasInfo::fromJson(const nlohmann::json& j) {
    if (j.contains("id") && j["id"].is_string()) ID = j["id"].get<std::string>();
    if (j.contains("path") && j["path"].is_string()) path = to_wstring(j["path"].get<std::string>());
    if (j.contains("number") && j["number"].is_number()) frameCount = j["number"].get<int>();
    return *this;
}

const nlohmann::json AudioInfo::toJson()const{
    nlohmann::json j;
    j["id"]=to_string(ID);
    j["path"]=to_string(path);
    return j;
}

const AudioInfo& AudioInfo::fromJson(const nlohmann::json& j) {
    if (j.contains("id") && j["id"].is_string()) ID = to_wstring(j["id"].get<std::string>());
    if (j.contains("path") && j["path"].is_string()) path = to_wstring(j["path"].get<std::string>());
    return *this;
}

const nlohmann::json ConfigItem::toJson()const{
    nlohmann::json j;
    switch(type){
        case Type::INT:j=int_val;break;
        case Type::UINT:j=uint_val;break;
        case Type::FLOAT:j=float_val;break;
        case Type::VECTOR2:
            j["x"]=vec2_val.x;
            j["y"]=vec2_val.y;
            break;
        case Type::STRING:j=string_val;break;
        case Type::IMAGE:
            for(const auto& img:image_val){
                j.push_back(img.toJson());
            }
            break;
        case Type::ATLAS:
            for(const auto& atlas:atlas_val){
                j.push_back(atlas.toJson());
            }
            break;
        case Type::AUDIO:
            for(const auto& audio:audio_val){
                j.push_back(audio.toJson());
            }
            break;
        case Type::JSON:j=json_val;break;
    }
    return j;
}

void Config::load(){
    if(this==default_instance)return;
    std::ifstream input("config.json");
    if (input.is_open()) {
        nlohmann::json j;
        try
        {
            input >> j;
            for (auto& [key, value] : j.items()) {
                if (value.is_null()) continue;
                else if (value.is_number_integer())items[key] = ConfigItem(value.get<int>());
                else if (value.is_number_float())items[key] = ConfigItem(value.get<float>());
                else if (value.is_number_unsigned())items[key] = ConfigItem(value.get<unsigned int>());
                else if (value.is_string())items[key] = ConfigItem(value.get<std::string>());
                else if (value.is_array()) {
                        if (key == "res.imgs")
                            for (const auto& img : value) items[key] += ImageInfo(img);
                        else if (key == "res.atlas")
                            for (const auto& atlas : value) items[key] += AtlasInfo(atlas);
                        else if (key == "res.audio")
                            for (const auto& audio : value) items[key] += AudioInfo(audio);
                    if (value.contains("x") && value.contains("y")) {
                        if (value["x"].is_number() && value["y"].is_number())
                            items[key] = ConfigItem(Vector2(value["x"].get<float>(), value["y"].get<float>()));
                    }
                    else items[key] = ConfigItem(value);
                }
            }
        }
        catch (...)
        {
			MessageBox(nullptr, L"Config file format error \n Recover to default config", L"Error", MB_OK | MB_ICONERROR);
        }
    }
    else{        
        MessageBox(nullptr,L"Cannot open config file",L"Error",MB_OK|MB_ICONERROR);
    }
    setDefaultConfig();
    saveToFile();
}

const ConfigItem& Config::get(const std::string& key)const{
    if(!items.contains(key)){
        if(this==default_instance){
            MessageBoxA(NULL,("Config key not found: "+key).c_str(),"Error",MB_OK|MB_ICONERROR);
            return ConfigItem(0);
        }
        return getDefaultInstance()->get(key);
    }
    return items.at(key);
}

void Config::setifno(const std::string& key,const ConfigItem& item){
    getDefaultInstance()->set(key,item);
    if(!items.contains(key)){
        items[key]=item;
    }
}

void Config::saveToFile(){
    std::ofstream output("config.json");
    if(!output.is_open()){
        MessageBox(NULL,L"Cannot open config file for writing",L"Error",MB_OK|MB_ICONERROR);
        return;
    }
    
    nlohmann::json j;
    for (const auto& [key, value] : items) {
        j[key] = value.toJson();
    }
    output << j.dump(4);
}

void Config::setDefaultConfig() {
    using i = ConfigItem;
    if(this==default_instance)return;

    ConfigItem imgs;
    imgs+=ImageInfo{"background",L"res/background.png"};
    imgs+=ImageInfo{"ui_heart",L"res/ui_heart.png"};
    imgs+=ImageInfo{"player_attack_right",L"res/player/attack.png"};
    imgs+=ImageInfo{"player_dead_right",L"res/player/dead.png"};
    imgs+=ImageInfo{"player_fall_right",L"res/player/fall.png"};
    imgs+=ImageInfo{"player_run_right",L"res/player/run.png"};
    imgs+=ImageInfo{"player_roll_right",L"res/player/roll.png"};
    imgs+=ImageInfo{"player_jump_right",L"res/player/jump.png"};
    imgs+=ImageInfo{"player_idle_right",L"res/player/idle.png"};

    imgs+=ImageInfo{"player_vfx_attack_down",L"res/player/vfx_attack_down.png"};
    imgs+=ImageInfo{"player_vfx_attack_up",L"res/player/vfx_attack_up.png"};
    imgs+=ImageInfo{"player_vfx_attack_left",L"res/player/vfx_attack_left.png"};
    imgs+=ImageInfo{"player_vfx_attack_right",L"res/player/vfx_attack_right.png"};
    imgs+=ImageInfo{"player_vfx_jump",L"res/player/vfx_jump.png"};
    imgs+=ImageInfo{"player_vfx_land",L"res/player/vfx_land.png"};

    items["res.imgs"]=imgs;

    ConfigItem atlas;
    atlas+=AtlasInfo{"barb_break", L"res/enemy/barb_break/%d.png", 3};
    atlas+=AtlasInfo{"barb_loose", L"res/enemy/barb_loose/%d.png", 5};
    atlas+=AtlasInfo{"silk", L"res/enemy/silk/%d.png", 9};
    atlas+=AtlasInfo{"sword_left", L"res/enemy/sword/%d.png", 3};

    atlas+=AtlasInfo{"enemy_aim_left", L"res/enemy/aim/%d.png", 9};
    atlas+=AtlasInfo{"enemy_dash_in_air_left", L"res/enemy/dash_in_air/%d.png", 2};
    atlas+=AtlasInfo{"enemy_dash_on_floor_left", L"res/enemy/dash_on_floor/%d.png", 2};
    atlas+=AtlasInfo{"enemy_fall_left", L"res/enemy/fall/%d.png", 4};
    atlas+=AtlasInfo{"enemy_idle_left", L"res/enemy/idle/%d.png", 6};
    atlas+=AtlasInfo{"enemy_jump_left", L"res/enemy/jump/%d.png", 8};
    atlas+=AtlasInfo{"enemy_run_left", L"res/enemy/run/%d.png", 8};
    atlas+=AtlasInfo{"enemy_squat_left", L"res/enemy/squat/%d.png", 10};
    atlas+=AtlasInfo{"enemy_throw_barb_left", L"res/enemy/throw_barb/%d.png", 8};
    atlas+=AtlasInfo{"enemy_throw_silk_left", L"res/enemy/throw_silk/%d.png", 17};
    atlas+=AtlasInfo{"enemy_throw_sword_left", L"res/enemy/throw_sword/%d.png", 16};

    atlas+=AtlasInfo{"enemy_vfx_dash_in_air_left", L"res/enemy/vfx_dash_in_air/%d.png", 5};
    atlas+=AtlasInfo{"enemy_vfx_dash_on_floor_left", L"res/enemy/vfx_dash_on_floor/%d.png", 6};

    items["res.atlas"] = atlas;

    ConfigItem audio;
    audio+=AudioInfo{L"bgm", L"res/audio/bgm.mp3"};
    audio+=AudioInfo{L"barb_break", L"res/audio/barb_break.mp3"};
    audio+=AudioInfo{L"bullet_time", L"res/audio/bullet_time.mp3"};

    audio+=AudioInfo{L"enemy_dash", L"res/audio/enemy_dash.mp3"};
    audio+=AudioInfo{L"enemy_run", L"res/audio/enemy_run.mp3"};
    audio+=AudioInfo{L"enemy_hurt_1", L"res/audio/enemy_hurt_1.mp3"};
    audio+=AudioInfo{L"enemy_hurt_2", L"res/audio/enemy_hurt_2.mp3"};
    audio+=AudioInfo{L"enemy_hurt_3", L"res/audio/enemy_hurt_3.mp3"};
    audio+=AudioInfo{L"enemy_throw_barbs", L"res/audio/enemy_throw_barbs.mp3"};
    audio+=AudioInfo{L"enemy_throw_silk", L"res/audio/enemy_throw_silk.mp3"};
    audio+=AudioInfo{L"enemy_throw_sword", L"res/audio/enemy_throw_sword.mp3"};

    audio+=AudioInfo{L"player_attack_1", L"res/audio/player_attack_1.mp3"};
    audio+=AudioInfo{L"player_attack_2", L"res/audio/player_attack_2.mp3"};
    audio+=AudioInfo{L"player_attack_3", L"res/audio/player_attack_3.mp3"};
    audio+=AudioInfo{L"player_dead", L"res/audio/player_dead.mp3"};
    audio+=AudioInfo{L"player_hurt", L"res/audio/player_hurt.mp3"};
    audio+=AudioInfo{L"player_jump", L"res/audio/player_jump.mp3"};
    audio+=AudioInfo{L"player_land", L"res/audio/player_land.mp3"};
    audio+=AudioInfo{L"player_roll", L"res/audio/player_roll.mp3"};
    audio+=AudioInfo{L"player_run", L"res/audio/player_run.mp3"};

    items["res.audio"] = audio;

    items["player.cd_roll"] = i(0.75f);
    items["player.cd_attack"] = i(0.5f);
    items["player.speed_jump"] = i(1000.0f);
    items["player.speed_run"] = i(300.0f);
    items["player.speed_roll"] = i(800.0f);
    items["player.time_go_back"] = i(0.3f);
    items["player.key.left1"] = i('A');
    items["player.key.left2"] = i(VK_LEFT);
    items["player.key.right1"] = i('D');
    items["player.key.right2"] = i(VK_RIGHT);
    items["player.key.jump1"] = i('W');
    items["player.key.jump2"] = i(VK_UP);
    items["player.key.jump3"] = i(VK_SPACE);
    items["player.key.roll1"] = i('S');
    items["player.key.roll2"] = i(VK_DOWN);
    items["player.default_position"]=i(Vector2(250,200));
    items["player.hit_box_size"]=i(Vector2(150,150));
    items["player.hurt_box_size"]=i(Vector2(40,80));
    items["player.max_hp"]=i(10);
    items["player.max_rec_progress"]=i(3);
    items["player.invulnerable_time"]=i(1.0f);
    items["enemy.max_hp"]=i(10);
    items["enemy.hit_box_size"]=i(Vector2(100,150));
    items["enemy.hit_box_size_squat"]=i(Vector2(100,80));
    items["enemy.hit_box_offset_squat"]=i(Vector2(0,30));
    items["enemy.hurt_box_size"]=i(Vector2(70,80));
    items["enemy.default_position"]=i(Vector2(1050,200));
    items["enemy.silk_size"]=i(Vector2(225,225));
    items["enemy.invulnerable_time"]=i(1.0f);
    items["enemy.probility.healthy.idle.jump"]=i(25);
    items["enemy.probility.healthy.idle.run"]=i(25);
    items["enemy.probility.healthy.idle.squat"]=i(30);
    items["enemy.probility.healthy.idle.throw_silk(auto calculate)"]=i(0);
    items["enemy.probility.healthy.jump.aim"]=i(50);
    items["enemy.probility.healthy.jump.fall"]=i(30);
    items["enemy.probility.healthy.jump.throw_silk(auto calculate)"]=i(0);
    items["enemy.probility.healthy.run.squat"]=i(75);
    items["enemy.probility.healthy.run.throw_silk(auto calculate)"]=i(0);
    items["enemy.probility.healthy.throw_sword.squat"]=i(50);
    items["enemy.probility.healthy.throw_sword.jump"]=i(30);
    items["enemy.probility.healthy.throw_sword.idle(auto calculate)"]=i(0);
    items["enemy.probility.unhealthy.idle.jump"]=i(25);
    items["enemy.probility.unhealthy.idle.squat"]=i(10);
    items["enemy.probility.unhealthy.idle.throw_silk"]=i(10);
    items["enemy.probility.unhealthy.idle.throw_barb"]=i(20);
    items["enemy.probility.unhealthy.idle.throw_sword(auto calculate)"]=i(0);
    items["enemy.probility.unhealthy.jump.aim"]=i(50);
    items["enemy.probility.unhealthy.jump.fall"]=i(30);
    items["enemy.probility.unhealthy.jump.throw_silk(auto calculate)"]=i(0);
    items["enemy.probility.unhealthy.run.squat"]=i(25);
    items["enemy.probility.unhealthy.run.throw_silk(auto calculate)"]=i(0);
    items["enemy.probility.unhealthy.throw_sword.jump"]=i(50);
    items["enemy.probility.unhealthy.throw_sword.throw_silk"]=i(30);
    items["enemy.probility.unhealthy.throw_sword.idle(auto calculate)"]=i(0);
    items["character.gravity"]=i(980*2.0f);
    items["character.invulnerable_blink_interval"]=i(0.1f);
}
