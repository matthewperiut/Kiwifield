#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include "player.h"
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

	bool OnUserUpdate(float fElapsedTime) override
	{
		Clear(olc::BLANK);
		` 
		return true;
	}
};


int main()
{
	Game game;
	game.self = &game;
	if (game.Construct(wWidth, wHeight, 7, 7, false, true))
		game.Start();

	return 0;
}