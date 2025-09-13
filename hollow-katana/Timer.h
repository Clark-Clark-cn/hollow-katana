#pragma once

#include <functional>

class Timer
{
public:
    Timer()=default;
    ~Timer()=default;
    void restart(){
        pass_time=0;
        shotted=false;
    }
    void setWaitTime(float time){
        wait_time=time;
    }
    void setOneShot(bool one_shot){
        this->one_shot=one_shot;
    }
    void setCallback(std::function<void()> callback){
        this->callback=callback;
    }
    void pause(){
        is_paused=true;
    }
    void resume(){
        is_paused=false;
    }
    void update(float delta){
        if(is_paused) return;
        pass_time+=delta;
        if(pass_time>=wait_time)
        {
            if((!one_shot||(one_shot&&!shotted))&&callback)
                callback();
            shotted=true;
            pass_time-=wait_time;
        }
    }
private:
    float pass_time=0;
    float wait_time=0;
    bool one_shot=false;
    bool shotted=false;
    bool is_paused=false;
    std::function<void()> callback;
};