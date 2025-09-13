#pragma once

class BulletTimeManager
{
public:
   enum class Status
   {
      Entering,
      Exiting

   };

static BulletTimeManager *getInstance();
	static void restart()
	{
		if (instance)delete instance;
		instance = nullptr;
	}
	void postProcess();
	void setStatus(Status status);
	float update(float delta);

private:
   static BulletTimeManager *instance;

   float progress = 0;
   Status status = Status::Exiting;
   const float SPEED_PROGRESS = 1.0f;
   const float DST_DELTA_FACTOR = 0.35f;
   const float DST_COLOR_FACTOR = 0.35f;

	BulletTimeManager();
   ~BulletTimeManager();

   static float lerp(float start, float end, float t)
   {
      return (1 - t) * start + t * end;
   }
};