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

	
	bool collisionDirections[8];
	setDirections(collisionArray, collisionDirections, pos);

	static const int maximumVel = 100;
	static bool gravity = true;
	static vf2d velocity = { 0,0 };
	if (collisionDirections[bottomleft] || collisionDirections[bottomright] || collisionDirections[down])
	{
		gravity = false;
	}
	else
	{
		gravity = true;
	}
	if (g.GetKey(Key::A).bHeld)
	{
		velocity.x = -50;
	}
	else if (g.GetKey(Key::D).bHeld)
	{
		velocity.x = 50;
	}
	else
	{
		velocity.x = 0;
	}
	
	if (gravity)
	{
		if (velocity.y < maximumVel)
		{
			velocity.y += time*100;
		}
	}
	else
	{
		velocity.y = 0;
	}

	if (g.GetKey(Key::SPACE).bPressed && gravity == false)
	{
		gravity = true;
		velocity.y = -maximumVel;
	}

	vf2d timedVelocity = velocity * time;
	pos += timedVelocity;

	// Temporary Solution to all my problems
	static int expectedYPos = 0;
	for (int i = pos.y; i < wHeight; i++)
	{
		if (collisionArray[(int)pos.x][i])
		{
			expectedYPos = i-1;
			break;
		}
		if (i == wHeight - 1)
		{
			pos.y = expectedYPos;
			velocity.y = 0;
			std::cout << "fixed falling through world" << std::endl;
		}
	}
}