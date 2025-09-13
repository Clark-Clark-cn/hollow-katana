#include "bulletTimeManager.h"
#include <graphics.h>

BulletTimeManager *BulletTimeManager::instance = nullptr;

BulletTimeManager *BulletTimeManager::getInstance()
{
    if (!instance)
        instance = new BulletTimeManager();
    return instance;
}
void BulletTimeManager::postProcess()
{
    DWORD *buffer = GetImageBuffer();
    int w = getwidth(), h = getheight();
    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            int idx = y * w + x;
            DWORD color = buffer[idx];
            BYTE r = (BYTE)(GetBValue(color) * lerp(1.0f, DST_COLOR_FACTOR, progress));
            BYTE g = (BYTE)(GetGValue(color) * lerp(1.0f, DST_COLOR_FACTOR, progress));
            BYTE b = (BYTE)(GetRValue(color) * lerp(1.0f, DST_COLOR_FACTOR, progress));
            buffer[idx] = BGR(RGB(b, g, r)) | (((DWORD)(BYTE)(255)) << 24);
        }
    }
}
void BulletTimeManager::setStatus(Status status)
{
    this->status = status;
}
float BulletTimeManager::update(float delta)
{
    float deltaProgress = SPEED_PROGRESS * delta;
    progress += deltaProgress * (status == Status::Entering ? 1 : -1);
    if (progress < 0)progress = 0;
    if (progress > 1.0f)progress = 1.0f;
    return delta * lerp(1.0f, DST_DELTA_FACTOR, progress);
}
BulletTimeManager::BulletTimeManager() = default;
BulletTimeManager::~BulletTimeManager() = default;
