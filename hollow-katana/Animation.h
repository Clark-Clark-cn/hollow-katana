#pragma once

#include "base.h"
#include "Timer.h"
#include "Vector2.h"
#include "Atlas.h"
#include <vector>
#include <functional>

class Animation
{
public:
    enum class AnchorMode
    {
        Centered,
        BottomCentered
    };
    Animation(){
        timer.setOneShot(false);
        timer.setCallback([this]() {
            frame_index++;
            if (frame_index >= frames.size()) {
                if (is_loop) {
                    frame_index = 0;
                }
                else {
                    frame_index = frames.size() - 1;
                    if (on_complete) on_complete();
                }
            }
            });
    }
    ~Animation()=default;

    void update(float delta){
        timer.update(delta);
    }

    void draw(){
        const Frame& frame=frames[frame_index];
        Rect rect_dst{(int)position.x-frame.rect.w/2,
            type==AnchorMode::Centered?(int)position.y-frame.rect.h/2:(int)position.y-frame.rect.h,
            frame.rect.w,frame.rect.h};
        drawImage(frame.img, &rect_dst,&frame.rect);
    }

    void reset(){
        timer.restart();
        frame_index=0;
    }
    void setAnchorMode(AnchorMode type){
        this->type=type;
    }
    void setPosition(const Vector2& position){
        this->position=position;
    }
    void setLoop(bool is_loop){
        this->is_loop=is_loop;
    }
    void setInterval(float interval){
        timer.setWaitTime(interval);
    }
    void setOnComplete(std::function<void()> on_complete){
        this->on_complete=on_complete;
    }
    void addFrame(IMAGE* img, int num){
        int w=img->getwidth();
        int h=img->getheight();
        int width_frame=w/num;
        for(int i=0;i<num;++i)
        {
            Rect rect_src{i*width_frame,0,width_frame,h};
            frames.emplace_back(img,rect_src);
        }
    }
    void addFrame(Atlas* atlas){
        for(int i=0;i<atlas->size();++i)
        {
            IMAGE* img=atlas->getImage(i);
            Rect rect_src{0,0,img->getwidth(),img->getheight()};
            frames.emplace_back(img,rect_src);
        }
    }
private:
    struct Frame
    {
        Rect rect;
        IMAGE* img=nullptr;

        Frame()=default;
        Frame(IMAGE* img, const Rect& rect):img(img),rect(rect) {}
        ~Frame()=default;
    };

    Timer timer;
    Vector2 position;
    bool is_loop=true;
    size_t frame_index=0;
    std::vector<Frame> frames;
    std::function<void()> on_complete=nullptr;
    AnchorMode type=AnchorMode::Centered;
};