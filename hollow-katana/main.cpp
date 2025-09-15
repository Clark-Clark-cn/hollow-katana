#include "base.h"
#include "ResourcesManager.h"
#include "CharacterManager.h"
#include "Collision.h"
#include "bulletTimeManager.h"

#include <chrono>
#include <thread>
#include <graphics.h>
#include <mutex>

bool restart = false;
bool isPaused = false;
bool isDebug = false;
bool isShowingTips = true;

static void drawBg()
{
    static IMAGE *img_bg = ResourcesManager::getInstance()->getImage("background");
    if (img_bg)
    {
        static Rect rect_dst{
            (getwidth() - img_bg->getwidth()) / 2,
            (getheight() - img_bg->getheight()) / 2,
            img_bg->getwidth(),
            img_bg->getheight()};
        drawImage(img_bg, &rect_dst);
    }
}

int main(int argc, char *argv[])
{
    using namespace std::chrono;
    HWND hwnd = initgraph(1280, 720, NULL);

    SetWindowText(hwnd, L"Hollow Katana");

    bool isQuit = false;
    ExMessage msg;
    std::mutex lock;
    steady_clock::time_point lastTick = steady_clock::now();
    bool inited = false;
    std::thread([&]()
    {
        try
        {
            ResourcesManager::getInstance()->load();
        }
        catch (const LPCTSTR id)
        {
            TCHAR err_msg[512];
            _stprintf_s(err_msg, 512, _T("%s load error"), id);
            MessageBox(hwnd, err_msg, L"res load error", MB_ICONERROR | MB_OK | MB_SYSTEMMODAL);
            return  -1;
        }
        playAudio(L"bgm", true);
        inited = true;
        while (isShowingTips)std::this_thread::sleep_for(milliseconds(1));
    	while (!isQuit){
            steady_clock::time_point frameStart = steady_clock::now();
            duration<float> delta = duration<float>(frameStart - lastTick);
		    {
                std::lock_guard<std::mutex> guard(lock);

			    float scaledDelta = BulletTimeManager::getInstance()->update(delta.count()*!isPaused);
				CharacterManager::getInstance()->update(scaledDelta);
				CollisionSystem::getInstance()->processCollisions();
                if (restart)
                {
                    BulletTimeManager::restart();
                    CharacterManager::restart();
                    CollisionSystem::restart();
					restart = false;
                }
		    }
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
            lastTick = frameStart;
        }
        return 0;
        }).detach();
    std::thread([&]() {
        while (!inited)std::this_thread::sleep_for(milliseconds(1));
        while (isShowingTips)std::this_thread::sleep_for(milliseconds(1));
        while (!isQuit) {
            while (peekmessage(&msg))
            {
                while (restart)Sleep(1);
                if (msg.message==WM_KEYUP)
                {
                    if (msg.vkcode == VK_ESCAPE)
                        isPaused = !isPaused;
                    else if (msg.vkcode == VK_F1)
                        isDebug = !isDebug;
					else if (msg.vkcode == VK_F2)
						isQuit = true;
                }
                if (isPaused)continue;
                std::lock_guard<std::mutex> guard(lock);
                CharacterManager::getInstance()->input(msg);
				std::cout << "message: " << msg.message << std::endl;
            }
            while (restart)Sleep(1);
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        }).detach();
    constexpr nanoseconds frameDuration(1000000000 / 144);
    BeginBatchDraw();
    MessageBox(NULL, LR"(
Control the character with WASD or Arrow Keys:  
- A/D or left/right: Move left/right  
- W or Space: Jump  
- S: Roll/Dodge  
- Left Mouse Button: Attack  
- Hold Right Mouse Button: Enter bullet time  
- ESC: Pause  
- F2: Exit
	)", L"Tips", MB_ICONINFORMATION|MB_SYSTEMMODAL);
	isShowingTips = false;
    while (!inited)Sleep(1);
    while (!isQuit)
    {
        steady_clock::time_point frameStart = steady_clock::now();
        setbkcolor(RGB(0, 0, 0));
        cleardevice();
	    {
            while (restart)Sleep(1);
            std::lock_guard<std::mutex> guard(lock);
            drawBg();
        	CharacterManager::getInstance()->draw();
        	if (isDebug)CollisionSystem::getInstance()->debugDraw();
}
        while (restart)Sleep(1);

        FlushBatchDraw();
        nanoseconds sleepDuration = frameDuration - (steady_clock::now() - frameStart);
        if (sleepDuration > nanoseconds(0))
            std::this_thread::sleep_for(sleepDuration);
    }
    EndBatchDraw();
    return 0;
}