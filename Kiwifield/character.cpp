#include "character.h"

Character::Character(vf2d pos, PixelGameEngine& g, string img) :
	pos{ pos }, g{ &g }, sprite{ new olc::Sprite(img) }, decal{ new olc::Decal(sprite) }
{
	feet = new Sprite(sprite->width, 1);
	feetdec = new Decal(feet);
	if (sprite->width == size.x && sprite->height == size.y)
	{
		drawSprite = true;

		for (int i = 0; i < size.x; i++)
		{
			feet->SetPixel(vi2d(i, 0), olc::BLANK);
			g.SetPixelMode(Pixel::ALPHA);
			feet->SetPixel(vi2d(i, 0), sprite->GetPixel(vi2d(i, sprite->height - 1)));
			sprite->SetPixel(vi2d(i, sprite->height - 1), olc::BLANK);
			
			g.SetPixelMode(Pixel::NORMAL);
		}
	}
	else
	{
		drawSprite = false;
	}
	feetdec->Update();
	decal->Update();
}

Character::~Character()
{
	delete sprite;
	delete decal;
	delete feet;
}

void Character::draw()
{
	if (drawSprite)
	{
		bool left = false;
		if (scale.x > 0)
		{
			g->DrawDecal(vi2d(pos.x - size.x / 2 + g->cam.getX(), pos.y - size.y + 1 + g->cam.getY()), decal, scale);
			g->DrawDecal(vi2d(pos.x - size.x / 2 + g->cam.getX() - flipper, pos.y), feetdec);
			left = false;
		}
		else
		{
			g->DrawDecal(vi2d(pos.x + size.x / 2 + g->cam.getX(), pos.y - size.y + 1 + g->cam.getY()), decal, scale);
			g->DrawDecal(vi2d(pos.x - size.x / 2 + g->cam.getX() - !flipper, pos.y), feetdec);
			left = true;
		}
		
	}
	else
		g->DrawRect(pos.x - (size.x / 2) + g->cam.getX(), pos.y - size.y + g->cam.getY(), size.x, size.y);
}

void Character::logic(float time, Stage& stage) 
{
	
	constexpr int maximumVel = 150;

	static float elapsed;
	elapsed += time;
	if (elapsed > 0.1)
	{
		flipper = !flipper;
		elapsed = 0;
	}
	if (velocity.x == 0 || gravity)
	{
		flipper = false;
	}

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

void Character::move(float time, Stage& stage)
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