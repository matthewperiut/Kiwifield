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

	bool collisionArray[wWidth][wHeight] = {};

	bool OnUserCreate() override
	{
		for (int x = 0; x < wWidth; x++)
		{
			for (int y = 0; y < wHeight; y++)
			{
				collisionArray[x][y] = 0;

				if (y == 120)
				{
					collisionArray[x][y] = 1;
				}
			}
		}

		return true;
	}

	Player x = Player(vi2d(0, 0));

	bool OnUserUpdate(float fElapsedTime) override
	{
		Clear(olc::BLANK);

		x.update(fElapsedTime, collisionArray, *self);
		
		static bool editCollision = false;
		if (GetKey(Key::TAB).bPressed)
		{
			editCollision = !editCollision;
		}

		if (editCollision)
		{
			if (GetMouse(0).bHeld)
			{
				vi2d m = GetMousePos();
				for (int x = 0; x < 2; x++)
				{
					for (int y = 0; y < 2; y++)
					{
						collisionArray[m.x + x - 1][m.y + y - 1] = true;
					}
				}
			}
			if (GetMouse(1).bHeld)
			{
				vi2d m = GetMousePos();
				for (int x = 0; x < 4; x++)
				{
					for (int y = 0; y < 4; y++)
					{
						collisionArray[m.x+x-1][m.y+y-1] = false;
					}
				}
			}

			for (int x = 0; x < wWidth; x++)
			{
				for (int y = 0; y < wHeight; y++)
				{
					if (collisionArray[x][y])
					{
						Draw(vi2d(x, y), olc::RED);
					}
				}
			}
		}
		

		return true;
	}
};


int main()
{
	Game game;
	game.self = &game;
	if (game.Construct(wWidth, wHeight, 7, 7, false, false))
		game.Start();

	return 0;
}