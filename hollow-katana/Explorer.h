#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <graphics.h>
#include "Atlas.h"

class Explorer{
public:
    struct ImageInfo{
        std::string ID;
        std::wstring path;
    };
    struct AtlasInfo{
        std::string ID;
        std::wstring path;
        int frameCount=0;
    };
private:
    std::unordered_map<std::string, IMAGE*> images;
    std::unordered_map<std::string, Atlas*> atlases;
    static std::shared_ptr<Explorer> instance;

    void flipImage(IMAGE* src,IMAGE* dest,int num=1);
    void flipImage(const std::string& id,const std::string& dstID,int num=1);
    void flipAtlas(const std::string& id,const std::string& dstID);

public:
    Explorer() = default;
    ~Explorer() = default;
    static Explorer* getInstance(){
        if (!instance) {
            instance = std::make_shared<Explorer>();
        }
        return instance.get();
    }
    static inline bool checkImageValid(IMAGE* img){return GetImageBuffer(img);}

    void load();
    Atlas* getAtlas(const std::string& id)const;
    IMAGE* getImage(const std::string& id)const;
};
