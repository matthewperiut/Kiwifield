#include "player.h"

Player::Player(vf2d p, PixelGameEngine& g)
	: Character(p, g, string("./assets/player.png"))
{
	
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
	draw();
	logic(time, stage);
}
