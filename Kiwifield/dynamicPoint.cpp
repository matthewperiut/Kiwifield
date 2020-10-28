#include "dynamicPoint.h"

DynamicPoint::DynamicPoint(Stage& s)
{
	stage = &s;
}

void DynamicPoint::Update(float time)
{
	vf2d timedVelocity = vel * time;
	vf2d newpos = pos + timedVelocity;
	//Horizontal
	if (vel.x > 0)
	{
		bool canmove = true;
		for (int x = (int)pos.x; x < ceil(newpos.x) + 1; x++)
		{

			//g->Draw(vi2d(x, (int)pos.y), Pixel(0, 0, 255, 125));
			if (stage->getCollision(vi2d(x, (int)pos.y)))
			{
				canmove = false;
				pos.x = x - 1;
			}
		}
		if (canmove)
			pos.x += timedVelocity.x;
	}
	if (vel.x < 0)
	{
		bool canmove = true;
		for (int x = (int)pos.x; x > floor(newpos.x) - 1; x--)
		{
			//g->Draw(vi2d(x, (int)pos.y), Pixel(0, 0, 255, 125));
			if (stage->getCollision(vi2d(x, (int)pos.y)))
			{
				canmove = false;
				pos.x = x + 1;
			}
		}
		if (canmove)
			pos.x += timedVelocity.x;
	}

	//Vertical
	if (vel.y > 0)
	{
		bool canmove = true;
		for (int y = (int)pos.y; y < ceil(newpos.y) + 1; y++)
		{
			//g->Draw(vi2d((int)pos.x, y), Pixel(0, 0, 255, 125));
			if (stage->getCollision(vi2d((int)pos.x, y)))
			{
				canmove = false;
				pos.y = y - 1;
			}
		}
		if (canmove)
			pos.y += timedVelocity.y;
	}
	if (vel.y < 0)
	{
		bool canmove = true;
		for (int y = (int)pos.y; y > floor(newpos.y) - 1; y--)
		{
			//g->Draw(vi2d((int)pos.x, y), Pixel(0, 0, 255, 125));
			if (stage->getCollision(vi2d((int)pos.x, y)))
			{
				pos.y -= 2;
			}
		}
		if (canmove)
			pos.y += timedVelocity.y;
	}
}