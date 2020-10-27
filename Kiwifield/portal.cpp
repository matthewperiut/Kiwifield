#include "portal.h"
Portal::Portal(vi2d pos)
{
	this->pos = pos;
}

void Portal::Destination(std::string des, vi2d pos)
{
	destination = des;
	desPos = pos;
}
bool Portal::Update(float time, vi2d p, PixelGameEngine& g)
{
	
	static float elapsed = 0;
	elapsed += time;

	int topy = pos.y - size.y;
	int boty = pos.y - g.cam.y;
	int leftx = pos.x - (size.x / 2);
	int rightx = leftx + size.x;

	//std::cout << leftx << ',' << boty << "-" << rightx << "," << topy << '\n';

	const static int speed = 20;
	for (int x = leftx; x < rightx; x++)
	{
		float timeNormalized = ((1 + sin(elapsed)) / 2);
		int color = 128*timeNormalized;
		Pixel maxColor = Pixel(80, 60, 240);

		int newy = boty;
		newy -= size.y/8 * (sin((elapsed + x)* speed)+1);

		color *= (sin(x)+1)/2;
		color /= 2;


		g.DrawLine(x + g.cam.getX(), boty + g.cam.getY(), x + g.cam.getX(), newy + g.cam.getY(), Pixel( 80 + (20*sin((elapsed + x) * speed) ), 80 + (20 * sin((elapsed + x) * speed)), 200));
		for (int y = boty; y > newy; y--)
		{
			int truy = y-newy;
			float opacity = 255 / (y*y);
			g.SetPixelMode(Pixel::ALPHA);
			g.Draw(vi2d(x + g.cam.getX(), y + g.cam.getY()), Pixel(80, 60, 240, opacity));
			g.SetPixelMode(Pixel::NORMAL);
		}
	}
	

	//animation
	//g.DrawRect(vi2d(pos.x - (size.x / 2), pos.y - size.y), size, GREY);
	//g.Draw(pos, RED);
	//check to teleport
	if (g.GetKey(W).bPressed)
	{
		if (p.x > pos.x - (size.x / 2) && p.x < pos.x + (size.x / 2))
			if (p.y < pos.y + size.y && p.y > pos.y - 1)
			{
				std::cout << "Teleport";
				return true;
				// I need to implement 'world' for this to work
			}
	}
	return false;
}