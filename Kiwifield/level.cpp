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
}

void Level::update(PixelGameEngine& g)
{
	g.Clear(olc::BLANK);

	g.SetDrawTarget(layers[0]);
	g.DrawDecal(vf2d(0, 0), dec[0]);
	g.EnableLayer(layers[0], true);
	g.SetDrawTarget(nullptr);
}