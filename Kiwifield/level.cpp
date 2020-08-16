#include "level.h"
#include "collisionfiles.h"

void Level::Create(PixelGameEngine& g)
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
	for (int i = layerNum - 1; i > -1; i--)
	{
		layers[i] = g.CreateLayer();

		spr[i] = new olc::Sprite(size.x, size.y);
		dec[i] = new olc::Decal(spr[i]);
	}
}

Level::Level(PixelGameEngine& g)
{
	//No image
	Create(g);
}

Level::Level(PixelGameEngine& g, std::string imgfilepath, bool pge = false)
{
	Create(g);

	if (imgfilepath != "null")
	{
		if(!pge)
			spr[2]->LoadFromFile(imgfilepath);
		else
			spr[2]->LoadFromPGESprFile(imgfilepath);
		dec[2]->Update();
	}
}

void Level::update(PixelGameEngine& g, Player& p, float fElapsed)
{
	g.Clear(olc::BLANK);

	p.update(fElapsed, collisionArray, g);

	if (g.GetKey(Key::CTRL).bHeld)
	{
		if (g.GetKey(Key::S).bPressed)
		{
			CollisionFiles::save(collisionArray,"./new.txt");
		}
	}

	g.SetDrawTarget(layers[2]);
	g.DrawDecal(vf2d(0, 0), dec[2]);
	g.EnableLayer(layers[2], true);
	g.SetDrawTarget(nullptr);
}

void Level::loadCollisions(string filepath)
{
	CollisionFiles::load(collisionArray, filepath);
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
			if (m.x > -1 && m.x < wWidth && m.y > -1 && m.y < wHeight)
			{
				for (int x = 0; x < 2; x++)
				{
					collisionArray[m.x][m.y] = true;
				}
			}
		}
		if (g.GetMouse(1).bHeld)
		{
			vi2d m = g.GetMousePos();
			if (m.x > -1 && m.x < wWidth && m.y > -1 && m.y < wHeight)
			{
				for (int x = 0; x < 4; x++)
				{
					for (int y = 0; y < 4; y++)
					{
						collisionArray[m.x + x - 1][m.y + y - 1] = false;
					}
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