#define OLC_PGE_APPLICATION
#include <string>
#include <iostream>
#include "olcPixelGameEngine.h"
#include "player.h"
#include "stage.h"
#include "files.h"

class Game : public olc::PixelGameEngine
{
public:
	Game()
	{
		sAppName = "Kiwifield";
	}
public:
	Player* player;
	Stage* stage;

	bool OnUserCreate() override
	{
		stage = new Stage(vi2d(260, 144), *this);
		stage->images.push_back(Image("./image.png", vi2d(0, 100)));
		player = new Player(vi2d(20, 20));
		return true;
	}

	bool OnUserDestroy() override
	{
		delete stage;
		delete player;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		
		
		int expectedCamXLeft = player->pos.x - (ScreenWidth() / 2);
		int expectedCamXRight = expectedCamXLeft + ScreenWidth();
		if (expectedCamXLeft >= 0 && expectedCamXRight < stage->getWidth()+1)
		{
			cam.x = expectedCamXLeft;
		}
		int expectedCamYTop = player->pos.y - (ScreenHeight() / 2);
		int expectedCamYBottom = expectedCamYTop + ScreenHeight();
		if (expectedCamYTop >= 0 && expectedCamYBottom < stage->getHeight()+1)
		{
			cam.y = expectedCamYTop;
		}

		if (GetKey(Key::LEFT).bPressed)
		{
			cam.x -= 1;
		}
		if (GetKey(Key::RIGHT).bPressed)
		{
			cam.x += 1;
		}
		if (GetKey(Key::UP).bPressed)
		{
			cam.y -= 1;
		}
		if (GetKey(Key::DOWN).bPressed)
		{
			cam.y += 1;
		}

		
		stage->drawImages(*this);

		Clear(olc::BLANK);
		stage->drawCollider(*this);
		player->update(fElapsedTime, *stage, *this);

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