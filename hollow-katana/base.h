#pragma once

#include <Windows.h>
#include <graphics.h>
#include <iostream>

#pragma comment(lib,"Msimg32.lib")
#pragma comment(lib,"Winmm.lib")

constexpr int WINDOW_WIDTH=1280;
constexpr int WINDOW_HEIGHT=720;

struct Rect
{
    int x, y, w, h;
};

inline void drawImage(IMAGE* img,const Rect* rect_dst, const Rect* rect_src=nullptr)
{
    static BLENDFUNCTION bf = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
    AlphaBlend(GetImageHDC(GetWorkingImage()),rect_dst->x,rect_dst->y,rect_dst->w,rect_dst->h,
        GetImageHDC(img),rect_src?rect_src->x:0,rect_src?rect_src->y:0,
        rect_src?rect_src->w:img->getwidth(),rect_src?rect_src->h:img->getheight(),bf);
}

inline void loadAudio(const std::wstring& path, const std::wstring ID){
    std::wstring cmd = L"open \"" + path + L"\" alias " + ID;
    int r=mciSendStringW(cmd.c_str(), NULL, 0, NULL);
	if (r != 0)std::wcerr << L"load audio error: " << path << std::endl;
}

inline void playAudio(const std::wstring ID, bool loop = false){
    std::wstring cmd = L"play " + ID + L" from 0" +(loop ? L" repeat" : L"");
    mciSendStringW(cmd.c_str(), NULL, 0, NULL);
}

inline void stopAudio(const std::wstring ID){
    std::wstring cmd = L"stop " + ID;
    mciSendStringW(cmd.c_str(), NULL, 0, NULL);
}

inline int rangeRandom(int minNum,int maxNum)
{
    return minNum + rand() % (maxNum - minNum + 1);
}