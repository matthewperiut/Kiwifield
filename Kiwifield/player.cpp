#include "player.h"

Player::Player(vf2d p)
{
	pos = p;
}

enum { up, down, left, right, topleft, topright, bottomleft, bottomright };

void setDirections(bool(&collisionArray)[wWidth][wHeight], bool(&cd)[8], vi2d pos)
{
	cd[up] =			collisionArray[pos.x][pos.y-1];
	cd[down] =			collisionArray[pos.x][pos.y+1];
	cd[left] =			collisionArray[pos.x-1][pos.y];
	cd[right] =			collisionArray[pos.x+1][pos.y];

	cd[topleft] =		collisionArray[pos.x - 1][pos.y - 1];
	cd[topright] =		collisionArray[pos.x + 1][pos.y - 1];
	cd[bottomleft] =	collisionArray[pos.x - 1][pos.y + 1];
	cd[bottomright] =	collisionArray[pos.x + 1][pos.y + 1];
}

void Player::update(float time, bool(&collisionArray)[wWidth][wHeight], PixelGameEngine& g)
{
	
	g.DrawCircle(pos + vf2d(0, -8), 8);

#ifdef _DEBUG
	g.Draw(pos, olc::RED);
#endif

	const static int multiplier = 100;
	float movement = time * multiplier;

	bool collisionDirections[8] = {};
	setDirections(collisionArray, collisionDirections, pos);

	if (g.GetKey(olc::Key::RIGHT).bHeld)
	{
		if (!collisionDirections[right])
			pos.x += movement;
		else if (!collisionDirections[topright])
			pos.y -= movement;
	}
	else if(g.GetKey(olc::Key::LEFT).bHeld)
	{
		if (!collisionDirections[left])
			pos.x -= movement;
		else if (!collisionDirections[topleft])
			pos.y -= movement;
	}

	static float verticalVelocity = 0;
	static float verticalVelMax = 2;
	if (!collisionDirections[down])
	{
		if(verticalVelocity < verticalVelMax)
			verticalVelocity += time*5;	
	}
	if (g.GetKey(olc::Key::UP).bPressed && collisionDirections[down])
	{
		collisionDirections[down] = false;
		pos.y -= 1;
		verticalVelocity = -2;
	}
	if (collisionDirections[down])
	{
		verticalVelocity = 0;
	}
	if (!collisionDirections[down])
	{
		pos.y += movement * verticalVelocity;
	}
	if (collisionDirections[up] && collisionDirections[down])
	{
		pos.y -= movement;
	}
}