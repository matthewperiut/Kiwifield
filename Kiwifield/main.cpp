#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

using namespace olc;

class Example : public olc::PixelGameEngine
{
public:
	Example()
	{
		sAppName = "Example";
	}
private:
	std::unique_ptr<olc::Sprite> sprTile;

public:
	bool OnUserCreate() override
	{
		// Called once at the start, so create things here
		sprTile = std::make_unique<olc::Sprite>("./scene.png");

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		// called once per frame
		//World
		DrawSprite(olc::vi2d(0, 0), sprTile.get());

		//Player
		DrawRect(vi2d(0, 0), vi2d(16,16));

		return true;
	}
};


int main()
{
	Example demo;
	if (demo.Construct(256, 144, 4, 4))
		demo.Start();

	return 0;
}