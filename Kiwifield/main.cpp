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

	bool OnUserCreate() override
	{
		

		return true;
	}

	Player x = Player(vi2d(0, 0));
	Level* y = new Level(*self);

	bool OnUserUpdate(float fElapsedTime) override
	{
		Clear(olc::BLANK);

		y->update(*self, x, fElapsedTime);
		y->CollisionEditor(*self);

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