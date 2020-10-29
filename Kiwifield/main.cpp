#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include "world.h"
#include "const.h"

class Game : public olc::PixelGameEngine
{
public:
	Game()
	{
		sAppName = "Kiwifield";
	}
public:
	World world;

	bool OnUserCreate() override
	{
		world.g = this;
		world.Construct(); 
		// Layer 0 is default starting
		CreateLayer(); // Layer 1
		CreateLayer(); // Layer 2
		return true;
	}

	bool OnUserDestroy() override
	{
		return true;
	}

	bool OnUserUpdate(const float fElapsedTime) override
	{
		static bool showFps = false;
		if (GetKey(Key::F3).bPressed)
		{
			showFps = !showFps;
		}
		if (showFps)
		{
			string fps = to_string(GetFPS());
			DrawStringDecal(vf2d(ScreenWidth()-fps.size()*8, 0), fps, GREEN);
		}

		world.Update(fElapsedTime);
		return true;
	}
};


int main()
{
	Game game;
	if (game.Construct(wWidth, wHeight, 5, 5, false, true))
		game.Start();

	return 0;
}
