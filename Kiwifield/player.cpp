#include "player.h"

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
	g.Draw(pos, olc::RED);
#endif

	static bool editCollision = false;
	if (g.GetKey(Key::TAB).bPressed)
	{
		editCollision = !editCollision;
	}

	if (editCollision)
	{
		if (g.GetMouse(0).bHeld)
		{
			vi2d m = g.GetMousePos();
			for (int x = 0; x < 2; x++)
			{
				for (int y = 0; y < 2; y++)
				{
					collisionArray[m.x + x - 1][m.y + y - 1] = true;
				}
			}
		}
		if (g.GetMouse(1).bHeld)
		{
			vi2d m = g.GetMousePos();
			for (int x = 0; x < 4; x++)
			{
				for (int y = 0; y < 4; y++)
				{
					collisionArray[m.x + x - 1][m.y + y - 1] = false;
				}
			}
		}

		for (int x = 0; x < wWidth; x++)
		{
			for (int y = 0; y < wHeight; y++)
			{
				if (collisionArray[x][y])
				{
					g.Draw(vi2d(x, y), olc::RED);
				}
			}
		}
	}
}