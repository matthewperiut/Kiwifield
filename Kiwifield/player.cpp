#include "player.h"
#define NEW

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
	g.Draw(pos, olc::GREEN);
#endif
	bool collisionDirections[8];
	setDirections(collisionArray, collisionDirections, pos);

	static const int maximumVel = 150;
	static bool gravity = true;
	static vf2d velocity = { 0,0 };
	if (collisionDirections[down] && velocity.y >= 0)
	{
		gravity = false;
	}
	else
	{
		gravity = true;
	}
	
	
	if (gravity)
	{
		if (velocity.y < maximumVel)
		{
			velocity.y += time*400;
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
	if (g.GetKey(Key::A).bHeld)
	{
#ifdef NEW
		velocity.x = -50;

		if (!collisionDirections[topleft] && collisionDirections[left] && !collisionDirections[up] && !gravity)
			velocity.y = -30;
#endif
#ifdef OLD
		if (!collisionDirections[left])
			velocity.x = -50;
		else if (collisionDirections[topleft])
			velocity.x = 0;
		else if (!collisionDirections[up] && !gravity)
			velocity = { -50, -25 };
#endif
	}
	else if (g.GetKey(Key::D).bHeld)
	{
#ifdef NEW
		velocity.x = 50;

		if (!collisionDirections[topright] && collisionDirections[right] && !collisionDirections[up] && !gravity)
			velocity.y = -30;
#endif
#ifdef OLD
		if (!collisionDirections[right])
			velocity.x = 50;
		else if (collisionDirections[topright])
			velocity.x = 0;
		else if (!collisionDirections[up] && !gravity)
			velocity = { 50, -25 };
#endif
	}
	else
	{
		velocity.x = 0;
	}

	
	vf2d timedVelocity = velocity * time;
#ifdef OLD
	pos += timedVelocity;
#endif

#ifdef NEW
	// Here I will do collision detection regardless of framerate :D
	vf2d newpos = pos + timedVelocity;
	
	//Horizontal
	if (velocity.x > 0)
	{
		bool canmove = true;
		for (int x = (int)pos.x; x < ceil(newpos.x); x++)
		{
			if (collisionArray[x][(int)pos.y])
			{
				canmove = false;
			}
		}
		if (canmove)
			pos.x += timedVelocity.x;
	}
	if (velocity.x < 0)
	{
		bool canmove = true;
		for (int x = (int)pos.x; x > floor(newpos.x)-1; x--)
		{
			if (collisionArray[x][(int)pos.y])
			{
				canmove = false;
			}
		}
		if (canmove)
			pos.x += timedVelocity.x;
	}

	//Vertical
	if (velocity.y > 0)
	{
		bool canmove = true;
		for (int y = (int)pos.y; y < ceil(newpos.y); y++)
		{
			if (collisionArray[(int)pos.x][y])
			{
				canmove = false;
			}
		}
		if (canmove)
			pos.y += timedVelocity.y;
	}
	if (velocity.y < 0)
	{
		bool canmove = true;
		for (int y = (int)pos.y; y > floor(newpos.y) - 1; y--)
		{
			if (collisionArray[(int)pos.x][y])
			{
				//canmove = false;
			}
		}
		if (canmove)
			pos.y += timedVelocity.y;
	}

	
#endif
#ifdef OLD
	// Temporary Solution to all my problems
	static vi2d expectedPos = { 0, 0 };
	for (int i = pos.y; i < wHeight; i++)
	{
		if (collisionArray[(int)pos.x][i])
		{
			expectedPos.y = i - 1;
			expectedPos.x = pos.x;
			break;
		}
	}
	static bool twice = false;
	if (expectedPos.y - pos.y < 1 && velocity.y > 0)
	{
		if (twice)
		{
			//No ground below
		}
		pos.y = expectedPos.y;
		
		twice = true;
	}
	else
	{
		twice = false;
	}
#endif
}