#define OLC_PGE_APPLICATION
#include <string>
#include "olcPixelGameEngine.h"
#include "player.h"
#include "level.h"

class Game : public olc::PixelGameEngine
{
public:
	Game()
	{
		sAppName = "Kiwifield";
	}
public:
	Player player = Player(vi2d(20, 0));
	Level* level;
	bool OnUserCreate() override
	{
		level = new Level(*this, "examplelevel", true);

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{

		Clear(olc::BLANK);
		level->update(*this, player, fElapsedTime);

		return true;
	}
};


int main()
{
	Game game;
	if (game.Construct(wWidth, wHeight, 5, 5, false, false))
		game.Start();

	return 0;
}