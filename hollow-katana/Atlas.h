#pragma once

#include <string>
#include <graphics.h>
#include <vector>

class Atlas
{
    std::vector<IMAGE> img_list;
public:
    Atlas()=default;
    ~Atlas()=default;

    void loadimage(const std::wstring& filePath, int num){
        img_list.clear();
        img_list.resize(num);

        wchar_t path[256];
        for (int i = 0; i < num; ++i) {
            _stprintf_s(path,filePath.c_str(),i+1);
            ::loadimage(&img_list[i], path);
        }
    }
    void clear(){
        img_list.clear();
    }
    int size() const {
        return img_list.size();
    }
    IMAGE* getImage(int index) {
        if (index < 0 || index >= img_list.size()) return &img_list.back();
        return &img_list[index];
    }
    void addImage(const IMAGE& img) {
        img_list.push_back(img);
    }
};