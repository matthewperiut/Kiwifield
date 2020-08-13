#include "player.h"

Player::Player(vf2d p)
{
	pos = p;
}

void Player::update(float time, PixelGameEngine& g)
{
#ifdef _DEBUG
	g.Draw(pos, olc::RED);
#endif

	const static int multiplier = 100;
	float movement = time * multiplier;

	if (g.GetKey(olc::Key::RIGHT).bHeld)
	{
		pos.x += movement;
	}
	else if(g.GetKey(olc::Key::LEFT).bHeld)
	{
		pos.x -= movement;
	}
	if (g.GetKey(olc::Key::UP).bHeld)
	{
		pos.y -= movement;
	}
	else if (g.GetKey(olc::Key::DOWN).bHeld)
	{
		pos.y += movement;
	}
}