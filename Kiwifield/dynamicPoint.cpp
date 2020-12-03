#include "dynamicPoint.h"

DynamicPoint::DynamicPoint()
{
	
}

DynamicPoint::~DynamicPoint()
{
	
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

void DynamicPoint::MoveUp(Stage& stage)
{
	// Up check
	directions[up] = false;
	for (int y = (int)pos.y; y > floor(newPos.y) - 1; y--)
	{
		if (stage.GetCollision(vi2d((int)newPos.x, y)))
		{
			directions[up] = true;
			if (vel.y < 0)
				pos.y = y + 1;
		}
	}
	if (!directions[up] && vel.y < 0)
		pos.y += timedVelocity.y;
}
void DynamicPoint::MoveDown(Stage& stage)
{
	// Down check
	directions[down] = false;
	for (int y = (int)pos.y; y < ceil(newPos.y) + 1; y++)
	{
		if (stage.GetCollision(vi2d((int)newPos.x, y)))
		{
			directions[down] = true;
			if (vel.y > 0)
				pos.y = y - 1;
		}
	}
	if (!directions[down] && vel.y > 0)
		pos.y += timedVelocity.y;

}
void DynamicPoint::MoveLeft(Stage& stage)
{
	// Left check
	directions[left] = false;
	for (int x = (int)pos.x; x > floor(newPos.x) - 1; x--)
	{
		if (stage.GetCollision(vi2d(x, (int)newPos.y)) || stage.GetCollision(vi2d(x, (int)pos.y)))
		{
			directions[left] = true;
			if (vel.x < 0)
				pos.x = x + 1;
		}
	}
	if (!directions[left] && vel.x < 0)
		pos.x += timedVelocity.x;
}
void DynamicPoint::MoveRight(Stage& stage)
{
	// Right check
	directions[right] = false;
	for (int x = (int)pos.x; x < ceil(newPos.x) + 1; x++)
	{

		if (stage.GetCollision(vi2d(x, (int)newPos.y)) || stage.GetCollision(vi2d(x, (int)pos.y)))
		{
			directions[right] = true;
			if (vel.x > 0)
				pos.x = x - 1;
		}
	}
	if (!directions[right] && vel.x > 0)
		pos.x += timedVelocity.x;
}

void DynamicPoint::Move(float time, Stage& stage)
{
	timedVelocity = vel * time;
	newPos = timedVelocity + pos;

	MoveUp(stage);
	MoveDown(stage);
	MoveLeft(stage);
	MoveRight(stage);
}