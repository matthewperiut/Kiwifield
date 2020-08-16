#include "level.h"


Level::Level(PixelGameEngine& g)
{
	//No image

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
}

void Level::CollisionEditor(PixelGameEngine& g)
{
	static bool editCollision = false;
	if (g.GetKey(Key::TAB).bPressed)
	{
		editCollision = !editCollision;
	}

	if (editCollision)
	{
		if (g.GetMouse(0).bHeld)
		{
			vi2d m = g.GetMousePos();
			for (int x = 0; x < 2; x++)
			{
				collisionArray[m.x][m.y] = true;
			}
		}
		if (g.GetMouse(1).bHeld)
		{
			vi2d m = g.GetMousePos();
			for (int x = 0; x < 4; x++)
			{
				for (int y = 0; y < 4; y++)
				{
					collisionArray[m.x + x - 1][m.y + y - 1] = false;
				}
			}
		}

		for (int x = 0; x < wWidth; x++)
		{
			for (int y = 0; y < wHeight; y++)
			{
				if (collisionArray[x][y])
				{
					g.Draw(vi2d(x, y), olc::RED);
				}
			}
		}
	}
}

void Level::update(PixelGameEngine& g, Player& p, float fElapsed)
{
	static bool init = false;
	if (!init)
	{
		for (int i = layerNum - 1; i > -1; i--)
		{
			layers[i] = g.CreateLayer();

			spr[i] = new olc::Sprite(size.x, size.y);
			dec[i] = new olc::Decal(spr[i]);
		}
		init = true;
	}

	g.Clear(olc::BLANK);

	p.update(fElapsed, collisionArray, g);

	g.SetDrawTarget(layers[0]);
	g.DrawDecal(vf2d(0, 0), dec[0]);
	g.EnableLayer(layers[0], true);
	g.SetDrawTarget(nullptr);
}

