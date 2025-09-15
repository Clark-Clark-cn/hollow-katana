#pragma once
#include "Player.h"

class StatusBar
{
public:
    StatusBar(Player* player): player(player) {
        img_ui_heart = ResourcesManager::getInstance()->getImage("ui_heart");
    }
    void draw(){
        if (!player)return;
        int hp = player->getHp();
        Rect dst = { BAR_X+10, BAR_Y, img_ui_heart->getwidth(), img_ui_heart->getheight() };
        for (int i = 0; i < hp; i++)
        {
            drawImage(img_ui_heart, &dst);
            dst.x += 40;
        }
        setfillcolor(RGB(35, 18, 10));
        solidroundrect(BAR_X, BAR_Y, BAR_X + 7, BAR_Y + img_ui_heart->getheight(), 8, 8);;
        float hpBarH = img_ui_heart->getheight() * (float)max(0, player->rec_progress) / player->max_rec_progress;
        setfillcolor(RGB(197,61,67));
        solidroundrect(BAR_X, BAR_Y, BAR_X+3, BAR_Y + hpBarH, 8, 8);
    }
private:
    Player* player;
    IMAGE* img_ui_heart;
    const int BAR_X = 20;
    const int BAR_Y = 20;
};