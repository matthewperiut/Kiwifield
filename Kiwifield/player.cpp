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

void Player::KeyboardInput(float time, Stage& stage)
{
	const static int speed = 50;
	if (g->GetKey(A).bHeld)
	{
		vel.x = -speed;
	}
	else if (g->GetKey(D).bHeld)
	{
		vel.x = speed;
	}
	else
	{
		vel.x = 0;
	}

	static const double maxJumpTime = 0.2;
	static double elapsedJumpTime = 0;
	static const double power = 20;

	// Gravity must come first
	

	/**/
	// Press space
	static int downCount = 0;
	if (Down())
	{
		downCount = 3;
	}
	else
	{
		downCount--;
	}
	if (downCount < 0)
	{
		vel.y += time * speed * 7;
	}
	else
	{
		if (vel.y > 0)
			vel.y = 0;
	}
	if (g->GetKey(SPACE).bPressed && !g->GetKey(S).bHeld)
	{
		if (downCount > 0 || canJump)
		{
			jump = true;
			vel.y = -125;
		}
	}
	if (g->GetKey(SPACE).bReleased && jump)
	{
		jump = false;
		vel.y /= 2;
	}

	if(g->GetKey(SPACE).bPressed && g->GetKey(S).bHeld && canJump)
	{
		if(!stage.GetCollision(vi2d(pos.x, pos.y + 2)) && pos.y + 2 < stage.GetHeight())
		{
			pos.y += 2;
		}
	}
	
	/*
	if(jump && elapsedJumpTime < maxJumpTime)
	{
		elapsedJumpTime += time;
		// = -(1+elapsedJumpTime) * power;
	}
	else
	{
		jump = false;
		elapsedJumpTime = 0;
	}
	std::cout << vel.y << endl;
	*/
	Logic(time, stage);
}

void Player::MoveUp(Stage& stage)
{
	// Up check
	directions[up] = false;
	for (int y = (int)pos.y; y > floor(newPos.y) - 1; y--)
	{
		if (stage.GetCollision(vi2d((int)newPos.x, y)))
		{
			directions[up] = false;
		}
	}
	if (!directions[up] && vel.y < 0)
		pos.y += timedVelocity.y;
}

void Player::Logic(float time, Stage& stage)
{
	g->EnableLayer(dynamics, true);
	g->SetDrawTarget(dynamics);

	if(drawSprite)
		if (scale.x > 0)
		{
			g->DrawDecal(vi2d(pos.x - size.x/2 + g->cam.GetX(), pos.y - size.y + 1 + g->cam.GetY()), decal, scale);
		}
		else
		{
			g->DrawDecal(vi2d(pos.x + size.x/2 + g->cam.GetX(), pos.y - size.y + 1 + g->cam.GetY()), decal, scale);
		}
	else
		g->DrawRect(pos.x - (size.x / 2) + g->cam.GetX(), pos.y - size.y + g->cam.GetY(), size.x, size.y);

	g->EnableLayer(dynamics, true); 
	g->SetDrawTarget(nullptr);
	
	constexpr int maximumVel = 150;
	//Vertical movement
	//if (Down())
		

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

	canJump = false;
	bool up = false;
	bool down = false;
	bool left = false;
	bool right = false;
	bool topright = false;
	bool topleft = false;
	for(int i = 1; i < 2; i++)
	{
		if (stage.GetCollision(vi2d(pos.x, pos.y - i)))
			up = true;
		if (stage.GetCollision(vi2d(pos.x, pos.y + i)))
			down = true;
		if (stage.GetCollision(vi2d(pos.x - i, pos.y)))
			left = true;
		if (stage.GetCollision(vi2d(pos.x + i, pos.y)))
			right = true;
		if (stage.GetCollision(vi2d(pos.x + i, pos.y - i)))
			topright = true;
		if (stage.GetCollision(vi2d(pos.x - i, pos.y - i)))
			topleft = true;
	}

	
	if(down && right)
	{
		if(!topright)
		{
			if(g->GetKey(D).bHeld)
				pos.y -= 1;
			
			canJump = true;
		}
	}
	if (down && left)
	{
		if (!topleft)
		{
			if(g->GetKey(A).bHeld)
				pos.y -= 1;
			canJump = true;
		}
	}
	for(int i = 1; i < 4; i++)
	{
		if (stage.GetCollision(vi2d(pos.x, pos.y + i)))
			canJump = true;
	}
	if(left && up && g->GetKey(A).bHeld)
	{
		vel.x = 0;
		pos.y -= 1;
		pos.x -= 1;
	}
	if (right && up && g->GetKey(D).bHeld)
	{
		vel.x = 0;
		pos.y -= 1;
		pos.x += 1;
	}
	Move(time, stage);
}