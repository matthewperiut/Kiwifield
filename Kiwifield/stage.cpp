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