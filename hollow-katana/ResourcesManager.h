#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <graphics.h>
#include "Atlas.h"

class ResourcesManager{
    std::unordered_map<std::string, IMAGE*> images;
    std::unordered_map<std::string, Atlas*> atlases;
    static ResourcesManager* instance;

    static void flipImage(IMAGE* src,IMAGE* dest,int num=1);
    void flipImage(const std::string& id,const std::string& dstID,int num=1);
    void flipAtlas(const std::string& id,const std::string& dstID);

    ResourcesManager() = default;
    ~ResourcesManager() = default;
public:
    static ResourcesManager* getInstance();
    static inline bool checkImageValid(IMAGE* img){return GetImageBuffer(img);}

    void load();
    Atlas* getAtlas(const std::string& id)const;
    IMAGE* getImage(const std::string& id)const;
};
