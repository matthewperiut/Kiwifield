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

	if (g->GetKey(Key::SHIFT).bHeld)
	{
		/* Future slope work
		
		vi2d localplayer = pos + vi2d(g->cam.getX(), g->cam.getY() - size.y / 2);
		vi2d localmouse = g->GetMousePos();
		g->DrawLine(localplayer, localmouse);
		if ((localplayer.x - localmouse.x) != 0)
		{
			double slope = (double)(localplayer.y - localmouse.y) / (double)(localplayer.x - localmouse.x) * -1;
			if (slope > 0)
				cout << "pos" << '\n';
			else
				cout << "neg" << '\n';
			
		}
			
		else
			cout << "infinity" << '\n';
			*/
	}

	//Firstly the player can move sideways
	if (g->GetKey(Key::A).bHeld)
		vel.x = -speed;
		
	else if (g->GetKey(Key::D).bHeld)
		vel.x = speed;
	else
		//Still in horizontal
		vel.x = 0;
	
	static bool canJump = true;
	static float power = 300;
	static float airTime = 0;
	static float maxAir = 0.5;
	if (Down())
	{
		jump = true;
	}
	if ((g->GetKey(Key::SPACE).bHeld && (!gravity || jump) && airTime < maxAir))
	{
		jump = true;
		airTime += time;
		if (vel.y > -100)
			vel.y = (-(1+maxAir) * power + (1+airTime) * power);
	}
	if (g->GetKey(Key::SPACE).bReleased)
	{
		airTime = 0;
		jump = false;
		if(vel.y < -80)
			vel.y = -80;
	}
	if (Up())
	{
		vel.y = 0;
		airTime = maxAir;
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
	if (Down() && vel.y >= 0)
		gravity = false;
	else
		gravity = true;

	if (vel.x < 0)
	{
		//left
		scale.x = -1;
		
	}
	else if (vel.x > 0)
	{
		//right
		scale.x = 1;
	}
	if (gravity)
	{
		if (vel.y < maximumVel)
			vel.y += time * 400;
	}
	else
		vel.y = 0;

	if (!Up())
	{
		if (!stage.getCollision(vi2d(pos.x, pos.y + 2)))
		{
			if (stage.getCollision(vi2d(pos.x, pos.y + 1)) && stage.getCollision(vi2d(pos.x - 1, pos.y + 2)) && vel.x < 0)
			{
				vel.y = 100;
				gravity = false;
			}
			if (stage.getCollision(vi2d(pos.x, pos.y + 1)) && stage.getCollision(vi2d(pos.x + 1, pos.y + 2)) && vel.x > 0)
			{
				vel.y = 100;
				gravity = false;
			}
		}
		if (stage.getCollision(vi2d(pos.x + 1, pos.y)) && !stage.getCollision(vi2d(pos.x + 1, pos.y - 1)) && vel.x > 0)
		{
			pos.y -= 1;
			gravity = false;
		}
		if (stage.getCollision(vi2d(pos.x - 1, pos.y)) && !stage.getCollision(vi2d(pos.x - 1, pos.y - 1)) && vel.x < 0)
		{
			pos.y -= 1;
			gravity = false;
		}
	}

	Move(time, stage);
}
