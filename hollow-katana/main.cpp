#include "base.h"
#include "Explorer.h"

#include <chrono>
#include <thread>
#include <graphics.h>
#include <mutex>

bool initFinished = false;

static void drawBg(){
    static IMAGE* img_bg=Explorer::getInstance()->getImage("background");
    if(img_bg){
        static Rect rect_dst{
            (getwidth()-img_bg->getwidth())/2,
            (getheight()-img_bg->getheight())/2,
            img_bg->getwidth(),
            img_bg->getheight()
        };
        drawImage(img_bg,&rect_dst);
    }
}

int main(int argc, char* argv[]){
    using namespace std::chrono;
    HWND hwnd = initgraph(1280,720,EW_SHOWCONSOLE);

    SetWindowText(hwnd, L"Hollow Katana");
    ExMessage msg;
    bool isQuit = false;
    std::mutex mtx;
    std::thread([&]{
        Explorer::getInstance()->load();
        initFinished = true;
        while (!isQuit)
        {
            while(peekmessage(&msg)){

            }
            
            //upgrade

            std::this_thread::sleep_for(std::chrono::milliseconds(16));
        }
    }).detach();
    const nanoseconds frameDuration(1000000000/144);
    steady_clock::time_point lastFrameTime = steady_clock::now();

    BeginBatchDraw();
    while(!initFinished);
    while (!isQuit)
    {
        
        auto frameStart = high_resolution_clock::now();

        drawBg();

        FlushBatchDraw();
        lastFrameTime = frameStart;
        nanoseconds sleepTime = frameDuration - duration_cast<nanoseconds>(high_resolution_clock::now() - frameStart);
        if (sleepTime > nanoseconds::zero())
        {
            std::this_thread::sleep_for(sleepTime);
        }
        EndBatchDraw();
    }
    return 0;
}