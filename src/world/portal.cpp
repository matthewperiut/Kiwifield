#include "portal.h"

#include "../const.h"

Portal::Portal(vi2d pos)
{
	this->pos = pos;
}

void Portal::Destination(std::string des, vi2d pos)
{
	destination = des;
	desPos = pos;
}
void Portal::DebugDraw(PixelGameEngine& g)
{
	g.EnableLayer(debug, true);
	g.SetDrawTarget(debug);
	
	g.DrawRect(vi2d(pos.x - (size.x / 2) + g.cam.GetX(), pos.y - size.y + g.cam.GetY()), size, GREY);

	g.EnableLayer(debug, true);
	g.SetDrawTarget(nullptr);
}

bool Portal::Update(float time, vi2d p, PixelGameEngine& g)
{
	g.EnableLayer(stage, true);
	g.SetDrawTarget(stage);
	
	elapsed += time;

	if (pos.x < 4 || pos.y < 4)
		return false;
	
	int topy = pos.y - size.y;
	int boty = pos.y;
	int leftx = pos.x - (size.x / 2);
	int rightx = leftx + size.x;

	const static int speed = 20;
	for (int x = leftx; x < rightx; x++)
	{
		float timeNormalized = ((1 + sin(elapsed)) / 2);
		int color = 128*timeNormalized;
		Pixel maxColor = Pixel(80, 60, 240);

		int newy = boty;
		newy -= size.y/8 * (cos((elapsed + x)* speed)+1);

		color *= (sin(x)+1)/2;
		color /= 2;

		g.DrawLine(x + g.cam.GetX(), boty + g.cam.GetY(), x + g.cam.GetX(), newy + g.cam.GetY(), Pixel( 80 + (20*sin((elapsed + x) * speed) ), 80 + (20 * sin((elapsed + x) * speed)), 200));
		for (int y = boty; y > newy; y--)
		{
			if (y == 0)
				y = 1;
			int truy = y-newy;
			float opacity = 255 / (y*y);
			g.SetPixelMode(Pixel::ALPHA);
			g.Draw(vi2d(x + g.cam.GetX(), y + g.cam.GetY()), Pixel(80, 60, 240, opacity));
			g.SetPixelMode(Pixel::NORMAL);
		}
	}
	
	if (g.GetKey(W).bPressed)
	{
		if (p.x > pos.x - (size.x / 2) && p.x < pos.x + (size.x / 2))
			if (p.y < pos.y + size.y && p.y > pos.y - 1)
			{
				return true;
				// I need to implement 'world' for this to work
				// I did it!!
			}
	}
	g.EnableLayer(stage, true);
	g.SetDrawTarget(nullptr);
	return false;
}