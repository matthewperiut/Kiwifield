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
	Sprite img = Sprite::Sprite(32, 32);

	olc::Sprite* bg = nullptr;
	olc::Decal* dec = nullptr;

	const static int layerNum = 3;
	int layers[layerNum];

	bool OnUserCreate() override
	{
		// Called once at the start, so create things here
		bg = new olc::Sprite("./scene.png");
		dec = new olc::Decal(bg);

		for (int i = 0; i < 32; i++)
		{
			for (int j = 0; j < 32; j++)
			{
				img.SetPixel(vi2d(i, j), olc::WHITE);
			}
		}

		img.SetPixel(vi2d(30, 30), olc::RED);

		for (int i = layerNum-1; i > -1; i--)
		{
			layers[i] = CreateLayer();
		}

		return true;
	}

	double elapsed = 0;
	Player x = Player(vi2d(0, 0));

	bool OnUserUpdate(float fElapsedTime) override
	{
		
		Clear(olc::BLANK);
		
		if (GetMouse(0).bHeld)
		{
			bg->SetPixel(GetMousePos(), olc::BLACK);
			dec->Update();
		}

		SetDrawTarget(layers[0]);
		DrawDecal(vf2d(0, 0), dec);
		EnableLayer(layers[0], true);
		SetDrawTarget(nullptr);
		
		
		if(x.pos.x < 256)
			elapsed += fElapsedTime;
		DrawString(vi2d(10, 10), std::to_string(elapsed));
		//DrawDecal(vi2d(0, 0), dec);

		x.update(fElapsedTime, *self);

		return true;
	}
};


int main()
{
	Game game;
	game.self = &game;
	if (game.Construct(256, 144, 4, 4, false, false))
		game.Start();

	return 0;
}