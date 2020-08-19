#include <direct.h>
#include <string>
#include "level.h"
#include "files.h"

void Level::Create(PixelGameEngine& g)
{
	for (int x = 0; x < wWidth; x++)
	{
		for (int y = 0; y < wHeight; y++)
		{
			collisionArray[x][y] = 0;

			if (y == 122)
			{
				collisionArray[x][y] = 1;
			}
		}
	}
	layer = g.CreateLayer();

	spr = new olc::Sprite(size.x, size.y);

	for (int x = 0; x < wWidth; x++)
	{
		for (int y = 0; x < wHeight; x++)
		{
			spr->SetPixel(x, y, olc::BLACK);
		}
	}

	dec = new olc::Decal(spr);
}

Level::Level(PixelGameEngine& g)
{
	//No image
	Create(g);
}

Level::Level(PixelGameEngine& g, std::string id, bool pge = false)
{
	levelid = id;
	Create(g);
	
	std::string temp = "./levels/" + levelid + "/";
	if (!Files::exists(temp))
		_mkdir(temp.c_str());

	if (!pge)
	{
		if (!Files::exists(temp + id + ".png"))
		{
			spr->SaveToPGESprFile("./levels/" + levelid + "/" + levelid + ".pge");
		}
		else
		{
			spr->LoadFromPGESprFile("./levels/" + levelid + "/" + levelid + ".png");
		}
	}
	else
	{
		if (!Files::exists("./levels/" + id + "/" + id + ".pge"))
		{
			spr->SaveToPGESprFile("./levels/" + levelid + "/" + levelid + ".pge");
		}
		else
		{
			spr->LoadFromPGESprFile("./levels/" + levelid + "/" + levelid + ".pge");
		}
	}
	
	if (!Files::load(collisionArray, "./levels/" + id + "/" + id + ".txt"))
	{
		Files::save(collisionArray, ("./levels/" + levelid + "/" + levelid + ".txt"));
	}

	dec->Update();
}

void Level::update(PixelGameEngine& g, Player& p, float fElapsed)
{
	g.Clear(olc::BLANK);

	p.update(fElapsed, collisionArray, g);

	if (g.GetKey(Key::CTRL).bHeld)
	{
		if (g.GetKey(Key::S).bPressed)
		{
			Files::save(collisionArray, ("./levels/" + levelid + "/" + levelid + ".txt"));
			spr->SaveToPGESprFile("./levels/" + levelid + "/" + levelid + ".pge");
		}
	}

	g.SetDrawTarget(layer);
	g.DrawDecal(vf2d(0, 0), dec);
	g.EnableLayer(layer, true);
	g.SetDrawTarget(nullptr);

	enum { play, editimg, editcollide };
	static int mode = 0;
	if (g.GetKey(Key::TAB).bPressed)
	{
		mode++;
		if (mode > 2)
		{
			mode = 0;
		}
	}
	switch (mode)
	{
	case play:
		break;
	case editimg:
		ImageEditor(g);
		break;
	case editcollide:
		CollisionEditor(g);
		break;
	}
}

void Level::ImageEditor(PixelGameEngine& g)
{
	static bool hide = false;
	if (g.GetKey(Key::H).bPressed)
	{
		hide = !hide;
	}
	vi2d m = g.GetMousePos();
	static int radius = 0;
	static int colorpicked[3] = { 125, 125, 125 };

	if (!hide)
	{
		// Draw Color Picker
		for (int y = 0; y < 6; y += 2)
		{
			for (int x = 0; x < 256; x++)
			{
				int red = 0;
				int green = 0;
				int blue = 0;

				if (y == 0)
					red = x;
				else if (y == 2)
					green = x;
				else if (y == 4)
					blue = x;

				g.Draw(x, y, olc::Pixel(red, green, blue));
				g.Draw(x, y + 1, olc::Pixel(red, green, blue));
			}
			g.Draw(colorpicked[y / 2], y, olc::WHITE);
			g.Draw(colorpicked[y / 2], y + 1, olc::WHITE);
		}

		if (g.GetMouse(0).bHeld && (m.y == 0 || m.y == 1))
			colorpicked[0] = m.x;
		if (g.GetMouse(0).bHeld && (m.y == 2 || m.y == 3))
			colorpicked[1] = m.x;
		if (g.GetMouse(0).bHeld && (m.y == 4 || m.y == 5))
			colorpicked[2] = m.x;

		g.FillRect(vi2d(0, 6), vi2d(10, 10), olc::Pixel(colorpicked[0], colorpicked[1], colorpicked[2]));
	}

	int drawlimit = 0;
	if (!hide)
	{
		drawlimit = 5;
	}

	if (m.y > drawlimit)
	{
		if (g.GetMouse(0).bHeld)
		{
			FillCircleInSpr(m.x, m.y, radius, olc::Pixel(colorpicked[0], colorpicked[1], colorpicked[2]), *spr);
			dec->Update();
		}
		if (g.GetMouse(1).bPressed)
		{
			Pixel colorpicker = spr->GetPixel(m);
			colorpicked[0] = colorpicker.r;
			colorpicked[1] = colorpicker.g;
			colorpicked[2] = colorpicker.b;
		}
	}
	
	if (g.GetMouseWheel() > 0)
	{
		if(radius < 30)
			radius++;
	}
	if (g.GetMouseWheel() < 0)
	{
		if(radius > 0)
			radius--;
	}

	g.DrawCircle(m, radius, olc::Pixel(abs(colorpicked[0]-10), abs(colorpicked[1]-10), abs(colorpicked[2]-10)));
}

void Level::CollisionEditor(PixelGameEngine& g)
{
	static bool linemode = 0;
	if (g.GetKey(Key::SHIFT).bPressed)
	{
		linemode = !linemode;
	}
	if (!linemode)
	{
		if (g.GetMouse(0).bHeld)
		{
			vi2d m = g.GetMousePos();
			if (m.x > -1 && m.x < wWidth && m.y > -1 && m.y < wHeight)
			{
				collisionArray[m.x][m.y] = true;
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
	}
	else
	{
		static vi2d start = { 0, 0 };
		static vi2d end = { 0, 0 };

		if (g.GetMouse(0).bHeld)
		{
			if (g.GetMouse(0).bPressed)
				start = g.GetMousePos();
			else
				g.DrawLine(start, g.GetMousePos(), olc::RED);
		}
		else if (g.GetMouse(0).bReleased)
		{
			end = g.GetMousePos();
			DrawLineIn2DBoolean(start.x, start.y, end.x, end.y, collisionArray);
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