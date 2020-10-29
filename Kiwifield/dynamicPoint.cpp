#include "dynamicPoint.h"

DynamicPoint::DynamicPoint(Stage& s)
{
	stage = &s;
}

bool DynamicPoint::Up()
{
	return directions[up];
}
bool DynamicPoint::Down()
{
	return directions[down];
}
bool DynamicPoint::Left()
{
	return directions[left];
}
bool DynamicPoint::Right()
{
	return directions[right];
}

void DynamicPoint::Update(float time)
{
	vf2d timedVelocity = vel * time;
	vf2d newpos = pos + timedVelocity;

	// Right check
	directions[right] = false;
	for (int x = (int)pos.x; x < ceil(newpos.x) + 1; x++)
	{

		//g->Draw(vi2d(x, (int)pos.y), Pixel(0, 0, 255, 125));
		if (stage->getCollision(vi2d(x, (int)pos.y)))
		{
			directions[right] = true;
			if (vel.x > 0)
				pos.x = x - 1;
		}
	}
	if (!directions[right] && vel.x > 0)
		pos.x += timedVelocity.x;
	
	// Left check
	directions[left] = false;
	for (int x = (int)pos.x; x > floor(newpos.x) - 1; x--)
	{
		//g->Draw(vi2d(x, (int)pos.y), Pixel(0, 0, 255, 125));
		if (stage->getCollision(vi2d(x, (int)pos.y)))
		{
			directions[left] = true;
			if(vel.x < 0)
				pos.x = x + 1;
		}
	}
	if (!directions[left] && vel.x < 0)
		pos.x += timedVelocity.x;

	// Down check
	directions[down] = false;
	for (int y = (int)pos.y; y < ceil(newpos.y) + 1; y++)
	{
		//g->Draw(vi2d((int)pos.x, y), Pixel(0, 0, 255, 125));
		if (stage->getCollision(vi2d((int)pos.x, y)))
		{
			directions[down] = true;
			if(vel.y > 0)
				pos.y = y - 1;
		}
	}
	if (!directions[down] && vel.y > 0)
		pos.y += timedVelocity.y;

	// Up check
	directions[up] = false;
	for (int y = (int)pos.y; y > floor(newpos.y) - 1; y--)
	{
		//g->Draw(vi2d((int)pos.x, y), Pixel(0, 0, 255, 125));
		if (stage->getCollision(vi2d((int)pos.x, y)))
		{
			directions[up] = true;
			if (vel.y < 0)
				pos.y = y + 1;
		}
	}
	if (!directions[up] && vel.y < 0)
		pos.y += timedVelocity.y;
}