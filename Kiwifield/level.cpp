#include "level.h"


Level::Level(PixelGameEngine& g)
{
	//No image

	for (int i = layerNum - 1; i > -1; i--)
	{
		layers[i] = g.CreateLayer();

		spr[i] = new olc::Sprite(size.x, size.y);
		dec[i] = new olc::Decal(spr[i]);
	}

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

void Level::update(PixelGameEngine& g, Player& p, float fElapsed)
{
	p.update(fElapsed, collisionArray, g);

	g.Clear(olc::BLANK);

	g.SetDrawTarget(layers[0]);
	g.DrawDecal(vf2d(0, 0), dec[0]);
	g.EnableLayer(layers[0], true);
	g.SetDrawTarget(nullptr);
}

