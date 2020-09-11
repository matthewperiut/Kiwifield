#include "stage.h"

void inline Stage::createCollisionVector()
{
	for (int y = 0; y < stageSize.y; y++)
	{
		collision.push_back({});
		for (int x = 0; x < stageSize.x; x++)
		{
			collision[y].push_back(0);
		}
	}
	for (int x = 0; x < getWidth(); x++)
	{
		setCollision(vi2d(x, getHeight() - 1), true);
		setCollision(vi2d(x, 0), true);
	}
	for (int y = 0; y < getHeight(); y++)
	{
		setCollision(vi2d(getWidth() - 1, y), true);
		setCollision(vi2d(0, y), true);
	}
}
Stage::Stage(vi2d size, PixelGameEngine& g)
{
	stageSize = vi2d(size.y,size.x);
	createCollisionVector();
}
Stage::Stage(string file, PixelGameEngine& g)
{
}
bool Stage::inbound(vi2d pos)
{
	if (pos.x >= 0 && pos.x < getWidth())
		return true;
	else
		return false;
}
void Stage::setCollision(vi2d pos, bool boolean)
{
	if (pos.x >= 0 && pos.x < getWidth())
		if (pos.y >= 0 && pos.y < getHeight())
			collision[pos.x][pos.y] = boolean;
}
bool Stage::getCollision(vi2d pos)
{
	if(pos.x >= 0 && pos.x < getWidth())
		if (pos.y >= 0 && pos.y < getHeight())
			return collision[pos.x][pos.y];

	return false;
}
int Stage::getWidth()
{
	return collision.size();
}
int Stage::getHeight()
{
	return collision[0].size();
}

void Stage::cameraFollow(vi2d player, PixelGameEngine& g)
{
	static bool init = false;
	static bool tooSmall[2] = { false, false };

	if (!init)
	{
		if (getWidth() < g.ScreenWidth())
		{
			tooSmall[0] = true;
		}

		if (getHeight() < g.ScreenHeight())
		{
			tooSmall[1] = true;
		}

		init = true;
	}

	if (!tooSmall[0])
	{
		int expectedCamXLeft = player.x - (g.ScreenWidth() / 2);
		int expectedCamXRight = expectedCamXLeft + g.ScreenWidth();
		if (expectedCamXLeft >= 0 && expectedCamXRight < getWidth() + 1)
		{
			g.cam.x = expectedCamXLeft;
		}
		else
		{
			if (expectedCamXLeft < 0)
				g.cam.x = 0;
			if (expectedCamXRight > getWidth())
				g.cam.x = getWidth() - g.ScreenWidth();
		}
	}
	else
	{
		int midstage = getWidth() / 2;
		int differenceToCamera = g.ScreenWidth() / 2;

		g.cam.x = midstage - differenceToCamera;
	}

	if (!tooSmall[1])
	{
		int expectedCamYTop = player.y - (g.ScreenHeight() / 2);
		int expectedCamYBottom = expectedCamYTop + g.ScreenHeight();
		if (expectedCamYTop >= 0 && expectedCamYBottom < getHeight() + 1)
		{
			g.cam.y = expectedCamYTop;
		}
		else
		{
			if (expectedCamYTop < 0)
				g.cam.y = 0;
			if (expectedCamYBottom > getHeight())
				g.cam.y = getHeight() - g.ScreenHeight();
		}
	}
	else
	{
		int midstage = getHeight() / 2;
		int differenceToCamera = g.ScreenHeight() / 2;

		g.cam.y = midstage - differenceToCamera;
	}
}

void Stage::drawImages(PixelGameEngine& g)
{
	for (int i = 0; i < images.size(); i++)
	{
		g.DrawDecal(vi2d(images[i].position.x + g.cam.getX(), images[i].position.y + g.cam.getY()), images[i].decal);
	}
}

void Stage::drawCollider(PixelGameEngine& g)
{
	for (int y = 0; y < collision.size(); y++)
	{
		for (int x = 0; x < collision[y].size(); x++)
		{
			g.Draw(vi2d(y + g.cam.getX(), x + g.cam.getY()), Pixel(255 * collision[y][x], 0, 0));
		}
	}
}