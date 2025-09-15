#pragma once
#include <nlohmann/json.hpp>
#include <vector>
#include "Vector2.h"
#include <variant>

struct ImageInfo
{
    ImageInfo()=default;
    ImageInfo(const nlohmann::json& j){fromJson(j);}
    ImageInfo(const std::string& id,const std::wstring& p):ID(id),path(p){}
    std::string ID;
    std::wstring path;
    const nlohmann::json toJson()const;
    const ImageInfo& fromJson(const nlohmann::json& j);
};
struct AtlasInfo
{
    AtlasInfo()=default;
    AtlasInfo(const nlohmann::json& j){fromJson(j);}
    AtlasInfo(const std::string& id,const std::wstring& p,int n):ID(id),path(p),frameCount(n){}
    std::string ID;
    std::wstring path;
    int frameCount = 0;
    const nlohmann::json toJson()const;
    const AtlasInfo& fromJson(const nlohmann::json& j);
};
struct AudioInfo
{
    AudioInfo()=default;
    AudioInfo(const nlohmann::json& j){fromJson(j);}
    AudioInfo(const std::wstring& id,const std::wstring& p):ID(id),path(p){}
    std::wstring path;
    std::wstring ID;
    const nlohmann::json toJson()const;
    const AudioInfo& fromJson(const nlohmann::json& j);
};

class ConfigItem{
    enum class Type {
        INT,
        UINT,
        FLOAT,
        VECTOR2,
        STRING,
        IMAGE,
        ATLAS,
        AUDIO,
        JSON
    };
    Type type = Type::INT;
    int int_val=0;
    unsigned int uint_val=0;
    float float_val=0;
    Vector2 vec2_val={0,0};
    std::string string_val="";
    std::vector<ImageInfo> image_val;
    std::vector<AtlasInfo> atlas_val;
    std::vector<AudioInfo> audio_val;
    nlohmann::json json_val=nullptr;
public:
    ConfigItem()=default;
    ~ConfigItem()=default;
    ConfigItem(int val):int_val(val){type=Type::INT;}
    ConfigItem(unsigned int val):uint_val(val){type=Type::UINT;}
    ConfigItem(float val):float_val(val){type=Type::FLOAT;}
    ConfigItem(const Vector2& val):vec2_val(val){type=Type::VECTOR2;}
    ConfigItem(const std::string& val):string_val(val){type=Type::STRING;}
    ConfigItem(const ImageInfo& val){image_val.push_back(val);type=Type::IMAGE;}
    ConfigItem(const AtlasInfo& val){atlas_val.push_back(val);type=Type::ATLAS;}
    ConfigItem(const AudioInfo& val){audio_val.push_back(val);type=Type::AUDIO;}
    ConfigItem(const nlohmann::json& val):json_val(val){type=Type::JSON;}

    int asInt()const{
        if(int_val)return int_val;
        if(uint_val)return static_cast<int>(uint_val);
        if(float_val)return static_cast<int>(float_val);
        return 0;
    }
    unsigned int asUInt()const{
        if(uint_val)return uint_val;
        if(int_val)return static_cast<unsigned int>(int_val);
        if(float_val)return static_cast<unsigned int>(float_val);
        return 0;
    }
    float asFloat()const{
        if(float_val)return float_val;
        if(int_val)return static_cast<float>(int_val);
        if(uint_val)return static_cast<float>(uint_val);
        return 0.0f;
    }
    const Vector2& asVector2()const{return vec2_val;}
    const std::string& asString()const{return string_val;}
    const std::vector<ImageInfo>& asImageInfo()const{return image_val;}
    const std::vector<AtlasInfo>& asAtlasInfo()const{return atlas_val;}
    const std::vector<AudioInfo>& asAudioInfo()const{return audio_val;}
    const nlohmann::json& asJson()const{return json_val;}
    const nlohmann::json toJson()const;

    operator int()const{return asInt();}
    operator unsigned int()const{return asUInt();}
    operator float()const{return asFloat();}
    operator Vector2()const{return asVector2();}
    operator std::string()const{return asString();}
    operator nlohmann::json()const{return asJson();}

    ConfigItem& operator+=(const ImageInfo& val){type=Type::IMAGE;image_val.push_back(val); return *this;}
    ConfigItem& operator+=(const AtlasInfo& val){type=Type::ATLAS;atlas_val.push_back(val); return *this;}
    ConfigItem& operator+=(const AudioInfo& val){type=Type::AUDIO;audio_val.push_back(val); return *this;}
};

class Config{
    static Config* instance;
    static Config* default_instance;
    std::unordered_map<std::string,ConfigItem> items;
private:
    Config(){load();}
    ~Config()=default;
    static Config* getDefaultInstance()
    {
        if (!default_instance)
            default_instance = new Config();
        return default_instance;
    }
    void load();
    void setDefaultConfig();
public:
    static Config* getInstance()
    {
        if (!instance)
            instance = new Config();
        return instance;
    }
    const ConfigItem& get(const std::string& key)const;
    void set(const std::string& key,const ConfigItem& item){items[key]=item;}
    void setifno(const std::string& key,const ConfigItem& item);

    void saveToFile();
};