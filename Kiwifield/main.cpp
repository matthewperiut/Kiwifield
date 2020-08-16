#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include "player.h"
#include "level.h"
#include <string>

using namespace olc;

class Game : public olc::PixelGameEngine
{
public:
	Game()
	{
		sAppName = "Kiwifield";
	}
public:
	olc::PixelGameEngine* self;

	Player x = Player(vi2d(20, 120));
	Level* y;
	bool OnUserCreate() override
	{
		y = new Level(*self,"./levels/examplelevel/scene.pge",true);
		y->levelid = 1;
		y->loadCollisions("./levels/examplelevel/scene.txt");
		
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		if (GetFPS() < 45)
		{
			// Below 45 fps player collision still doesn't work, so I will be working on a solution
			static float total = 0;
			total += fElapsedTime;
			if(total == 5)
				return false;
			std::cout << total << std::endl;
		}

		Clear(olc::BLANK);

		y->update(*self, x, fElapsedTime);
#ifdef _DEBUG
		y->CollisionEditor(*self);
#endif
		return true;
	}
};


int main()
{
	Game game;
	game.self = &game;
	if (game.Construct(wWidth, wHeight, 5, 5, false, true))
		game.Start();

	return 0;
}