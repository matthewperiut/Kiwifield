#include "player.h"

Player::Player(vf2d p, PixelGameEngine& g)
{
	pos = p;
	this->g = &g;
	sprite = new olc::Sprite("./player.png");
	decal = new olc::Decal(sprite);
}

void Player::keyboardInput(float time, Stage& stage)
{
	g->DrawRect(pos.x - 4 + g->cam.getX(), pos.y - 8 + g->cam.getY(), 8, 8);

	static bool gravity = true;

	constexpr int maximumVel = 150;
	constexpr int speed = 50;

	//Firstly the player can move sideways
	if (g->GetKey(Key::A).bHeld)
		//l
		velocity.x = -speed;
	else if (g->GetKey(Key::D).bHeld)
		//r
		velocity.x = speed;
	else
		//Still in horizontal
		velocity.x = 0;

	//Vertical movement
	if (stage.getCollision(vi2d(pos.x, pos.y + 1)) && velocity.y >= 0)
		gravity = false;
	else
        gravity = true;

	if (gravity)
	{
		if (velocity.y < maximumVel)
			velocity.y += time * 400;
	}
	else
		velocity.y = 0;

	static float elapsedSkip = 0;
	if (velocity.y <= 0)// && elapsedSkip > 0.05)
	{
		if (stage.getCollision(vi2d(pos.x + 1, pos.y)) && !stage.getCollision(vi2d(pos.x + 1, pos.y - 1)) && velocity.x > 0)
		{
			pos.y -= 1;
			gravity = false;
			elapsedSkip = 0;
		}
		if (stage.getCollision(vi2d(pos.x - 1, pos.y)) && !stage.getCollision(vi2d(pos.x - 1, pos.y - 1)) && velocity.x < 0)
		{
			pos.y -= 1;
			gravity = false;
			elapsedSkip = 0;
		}
	}
	
	
	else
	{
		elapsedSkip += time;
	}

	if (!gravity && g->GetKey(Key::SPACE).bPressed)
	{
		gravity = true;
		velocity.y = -maximumVel;
	}
	move(time, stage);
}

void Player::move(float time, Stage& stage)
{
	vf2d timedVelocity = velocity * time;

	vf2d newpos = pos + timedVelocity;
	//Horizontal
	if (velocity.x > 0)
	{
		bool canmove = true;
		for (int x = (int)pos.x; x < ceil(newpos.x) + 1; x++)
		{

			//g->Draw(vi2d(x, (int)pos.y), Pixel(0, 0, 255, 125));
			if (stage.getCollision(vi2d(x, (int)pos.y)))
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
			//g->Draw(vi2d(x, (int)pos.y), Pixel(0, 0, 255, 125));
			if (stage.getCollision(vi2d(x, (int)pos.y)))
			{
				canmove = false;
				pos.x = x + 1;
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
			//g->Draw(vi2d((int)pos.x, y), Pixel(0, 0, 255, 125));
			if (stage.getCollision(vi2d((int)pos.x, y)))
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
			//g->Draw(vi2d((int)pos.x, y), Pixel(0, 0, 255, 125));
			if (stage.getCollision(vi2d((int)pos.x, y)))
			{
				canmove = false;
				pos.y = y + 1;
				velocity.y = 0;
			}
		}
		if (canmove)
			pos.y += timedVelocity.y;
	}

}
