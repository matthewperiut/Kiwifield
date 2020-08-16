#include "olcPixelGameEngine.h"
#include "collisionHelper.h"

using namespace olc;

void viewer()
{
	static bool editCollision = false;
	if (GetKey(Key::TAB).bPressed)
	{
		editCollision = !editCollision;
	}

	if (editCollision)
	{
		if (GetMouse(0).bHeld)
		{
			vi2d m = GetMousePos();
			for (int x = 0; x < 2; x++)
			{
				for (int y = 0; y < 2; y++)
				{
					collisionArray[m.x + x - 1][m.y + y - 1] = true;
				}
			}
		}
		if (GetMouse(1).bHeld)
		{
			vi2d m = GetMousePos();
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
					Draw(vi2d(x, y), olc::RED);
				}
			}
		}
	}
}