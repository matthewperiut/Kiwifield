#include "player.h"

Player::Player(vf2d p, PixelGameEngine& g)
{
	pos = p;
	this->g = &g;
	sprite = new olc::Sprite("./assets/player.png");
	decal = new olc::Decal(sprite);

	if (sprite->width == size.x && sprite->height == size.y)
	{
		drawSprite = true;
	}
	else
	{
		drawSprite = false;
	}
}

Player::~Player()
{
	delete sprite;
	delete decal;
}

void Player::keyboardInput(float time, Stage& stage)
{
	constexpr int speed = 50;

	//Firstly the player can move sideways
	if (g->GetKey(Key::A).bHeld)
		velocity.x = -speed;
		
	else if (g->GetKey(Key::D).bHeld)
		velocity.x = speed;
	else
		//Still in horizontal
		velocity.x = 0;

	if (g->GetKey(Key::SPACE).bPressed)
	{
		if (g->GetKey(Key::S).bHeld && !gravity)
		{
			if(pos.y < stage.getHeight()-4)
				pos.y += 2;
		}
		else if (stage.getCollision(vi2d(pos.x, pos.y + 1)) && stage.getCollision(vi2d(pos.x - 1, pos.y + 2)) && velocity.x < 0)
		{
			jump = true;
			pos.y -= 1;
		}
		else if (stage.getCollision(vi2d(pos.x, pos.y + 1)) && stage.getCollision(vi2d(pos.x + 1, pos.y + 2)) && velocity.x > 0)
		{
			jump = true;
			pos.y -= 1;
		}
		else if (stage.getCollision(vi2d(pos.x + 1, pos.y)) && !stage.getCollision(vi2d(pos.x + 1, pos.y - 1)) && velocity.x > 0)
		{
			jump = true;
			pos.y -= 1;
		}
		else if (stage.getCollision(vi2d(pos.x - 1, pos.y)) && !stage.getCollision(vi2d(pos.x - 1, pos.y - 1)) && velocity.x < 0)
		{
			jump = true;
			pos.y -= 1;
		}
		else if (!gravity)
		{
			jump = true;
			pos.y -= 1;
		}
		
	}

	logic(time, stage);
}

void Player::logic(float time, Stage& stage)
{
	if(drawSprite)
		if (scale.x > 0)
		{
			g->DrawDecal(vi2d(pos.x - size.x/2 + g->cam.getX(), pos.y - size.y + 1 + g->cam.getY()), decal, scale);
		}
		else
		{
			g->DrawDecal(vi2d(pos.x + size.x/2 + g->cam.getX(), pos.y - size.y + 1 + g->cam.getY()), decal, scale);
		}
	else
		g->DrawRect(pos.x - (size.x / 2) + g->cam.getX(), pos.y - size.y + g->cam.getY(), size.x, size.y);

	constexpr int maximumVel = 150;

	
	//Vertical movement
	if (stage.getCollision(vi2d(pos.x, pos.y + 1)) && velocity.y >= 0)
		gravity = false;
	else
		gravity = true;

	if (velocity.x < 0)
	{
		//left
		scale.x = -1;
		
	}
	else if (velocity.x > 0)
	{
		//right
		scale.x = 1;
	}
	if (gravity)
	{
		if (velocity.y < maximumVel)
			velocity.y += time * 400;
	}
	else
		velocity.y = 0;

	if (!stage.getCollision(vi2d(pos.x, pos.y - 1)))
	{
		if (!stage.getCollision(vi2d(pos.x, pos.y + 2)))
		{
			if (stage.getCollision(vi2d(pos.x, pos.y + 1)) && stage.getCollision(vi2d(pos.x - 1, pos.y + 2)) && velocity.x < 0)
			{
				velocity.y = 100;
				gravity = false;
			}
			if (stage.getCollision(vi2d(pos.x, pos.y + 1)) && stage.getCollision(vi2d(pos.x + 1, pos.y + 2)) && velocity.x > 0)
			{
				velocity.y = 100;
				gravity = false;
			}
		}
		if (stage.getCollision(vi2d(pos.x + 1, pos.y)) && !stage.getCollision(vi2d(pos.x + 1, pos.y - 1)) && velocity.x > 0)
		{
			pos.y -= 1;
			gravity = false;
		}
		if (stage.getCollision(vi2d(pos.x - 1, pos.y)) && !stage.getCollision(vi2d(pos.x - 1, pos.y - 1)) && velocity.x < 0)
		{
			pos.y -= 1;
			gravity = false;
		}
	}
	
	

	if (jump)
	{
		gravity = true;
		velocity.y = -maximumVel;
		jump = false;
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
				pos.y -= 2;
			}
		}
		if (canmove)
			pos.y += timedVelocity.y;
	}

}
