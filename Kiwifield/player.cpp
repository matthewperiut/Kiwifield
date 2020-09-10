#include "player.h"
#define NEW
#define FALLFIX

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
	
	g.DrawRect(pos.x-4,pos.y-8,8,8,Pixel(100,100,100));
	//g.Draw(pos, olc::GREEN);

	bool collisionDirections[8];
	setDirections(collisionArray, collisionDirections, pos);

	static bool gravity = true;

	constexpr int maximumVel = 150;
	constexpr int speed = 50;
	static vf2d velocity = { 0, 0 };

	//Firstly the player can move sideways
	if (g.GetKey(Key::A).bHeld)
		//Left
		velocity.x = -speed;
	else if (g.GetKey(Key::D).bHeld)
		//Right
		velocity.x = speed;
	else
		//Still in horizontal
		velocity.x = 0;

	//Vertical movement
	if (collisionDirections[down] && velocity.y >= 0)
		gravity = false;
	else
		gravity = true;

	if (!gravity)
		velocity.y < maximumVel;

	if (gravity)
	{
		if (velocity.y < maximumVel)
			velocity.y += time * 400;
	}
	else
		velocity.y = 0;
	
	static float elapsedSkip = 0;
	if (collisionDirections[right] && !collisionDirections[topright] && velocity.x > 0 && elapsedSkip > 0.05)
	{
		pos.y -= 1;
		gravity = false;
		elapsedSkip = 0;
	}
	else if (collisionDirections[left] && !collisionDirections[topleft] && velocity.x < 0 && elapsedSkip > 0.05)
	{
		pos.y -= 1;
		gravity = false;
		elapsedSkip = 0;
	}
	else
	{
		elapsedSkip += time;
	}

	if (!gravity && g.GetKey(Key::SPACE).bPressed)
	{
		gravity = true;
		velocity.y = -maximumVel;
	}

	vf2d timedVelocity = velocity * time;

	vf2d newpos = pos + timedVelocity;
	g.SetPixelMode(Pixel::Mode::ALPHA);
	//Horizontal
	if (velocity.x > 0)
	{
		bool canmove = true;
		for (int x = (int)pos.x; x < ceil(newpos.x) + 1; x++)
		{
			
			//g.Draw(vi2d(x, (int)pos.y), Pixel(0, 0, 255, 125));
			if (collisionArray[x][(int)pos.y])
			{
				canmove = false;
				pos.x = x - 1;
			}
		}
		if (canmove)
			pos.x += timedVelocity.x;
	}
	if (velocity.x < 0)
	{
		bool canmove = true;
		for (int x = (int)pos.x; x > floor(newpos.x) - 1; x--)
		{
			//g.Draw(vi2d(x, (int)pos.y), Pixel(0, 0, 255, 125));
			if (collisionArray[x][(int)pos.y])
			{
				canmove = false;
				pos.x = x+1;
			}
		}
		if (canmove)
			pos.x += timedVelocity.x;
	}

	//Vertical
	if (velocity.y > 0)
	{
		bool canmove = true;
		for (int y = (int)pos.y; y < ceil(newpos.y) + 1; y++)
		{
			//g.Draw(vi2d((int)pos.x, y), Pixel(0, 0, 255, 125));
			if (collisionArray[(int)pos.x][y])
			{
				canmove = false;
				pos.y = y - 1;
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
			//g.Draw(vi2d((int)pos.x, y), Pixel(0, 0, 255, 125));
			if (collisionArray[(int)pos.x][y])
			{
				canmove = false;
				pos.y = y + 1;
				velocity.y = 0;
			}
		}
		if (canmove)
			pos.y += timedVelocity.y;
	}
	g.SetPixelMode(Pixel::Mode::NORMAL);
}