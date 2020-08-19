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
	olc::PixelGameEngine* self;

	Player x = Player(vi2d(20, 110));
	Level* y;
	bool OnUserCreate() override
	{
		y = new Level(*self, "new", true);
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{

		Clear(olc::BLANK);

		y->update(*self, x, fElapsedTime);

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