#pragma once
#include "character.h"

class CharacterManager
{
public:
	static CharacterManager* getInstance();
	static void restart(){
		if(instance){
			delete instance;
			instance=nullptr;
		}
	}
	Character* getEnemy()
	{
	    return enemy;
	}
	Character* getPlayer()
	{
	    return player;
	}

	bool input(const ExMessage& msg);
	void update(float delta);
	void draw();
	private:
	static CharacterManager* instance;
	Character* enemy=nullptr;
	Character* player=nullptr;
	CharacterManager();
    ~CharacterManager();
};

