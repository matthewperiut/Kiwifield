#include "player.h"

Player::Player(vf2d p, PixelGameEngine& g)
{
	pos = p;
	this->g = &g;
	sprite = new olc::Sprite("./assets/localplayer/still.png");
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

void Player::Animate(float time)
{
	static bool init = false;
	static vector<unique_ptr<Sprite>> walk;
	static vector<unique_ptr<Sprite>> idle;
	static vector<unique_ptr<Sprite>> jump;
	if(!init)
	{
		string localplayer ="./assets/localplayer/";
		for(int i = 0; i < 8; i++)
		{
			if (!fs::exists(localplayer + "walk" + to_string(i) + ".png"))
				break;
			else
				walk.push_back(std::make_unique<olc::Sprite>(localplayer + "walk" + to_string(i) + ".png"));
		}
		for (int i = 0; i < 8; i++)
		{
			if (!fs::exists(localplayer + "idle" + to_string(i) + ".png"))
				break;
			else
				idle.push_back(std::make_unique<olc::Sprite>(localplayer + "idle" + to_string(i) + ".png"));
		}
		for (int i = 0; i < 8; i++)
		{
			if (!fs::exists(localplayer + "jump" + to_string(i) + ".png"))
				break;
			else
				jump.push_back(std::make_unique<olc::Sprite>(localplayer + "jump" + to_string(i) + ".png"));
		}
	}

	static float counter = 0;
	static bool walking = false;
	static bool idling = false;
	static bool jumping = false;
	if(vel.x != 0)
	{
		walking = true;
		idling = false;
		jumping = false;
	}
	if(vel.y != 0)
	{
		walking = false;
		idling = false;
		jumping = true;
	}
	if(vel.x == 0 && vel.y == 0)
	{
		walking = false;
		jumping = false;
		idling = true;
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
	if (g->GetKey(SPACE).bReleased && jump && vel.y < 0)
	{
		jump = false;
		vel.y /= 2;
	}

	if(g->GetKey(SPACE).bPressed && g->GetKey(S).bHeld && canJump)
	{
		bool canFall = false;
		for(int i = pos.y+2; i < stage.GetHeight(); i++)
		{
			if (stage.GetCollision(vi2d(pos.x, i)))
			{
				canFall = true;
				break;
			}
		}
		if (!stage.GetCollision(vi2d(pos.x, pos.y + 2)) && canFall)
		{
			pos.y += 2;
		}
	}
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
		if (g->GetKey(A).bHeld)
			vel.y = 100;
		if(!topright)
		{
			if(g->GetKey(D).bHeld)
				pos.y -= 1;
			canJump = true;
		}
	}
	if (down && left)
	{
		if (g->GetKey(D).bHeld)
			vel.y = 100;
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
	if(!stage.GetCollision(vi2d(pos.x, pos.y - 2)))
	{
		if (left && up && g->GetKey(A).bHeld)
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
	}
	
	Move(time, stage);

	//Addition movement in player
	int ct = 0;
	if(vel.y < 0)
		for (int y = pos.y - size.y; y > floor(newPos.y - size.y - 2); y--)
		{
			if(stage.GetCollision(vi2d(pos.x, y)))
				ct++;
		}
	if(ct > 1)
	{
		vel.y = 0;
	}
	
	
}