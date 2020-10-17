#include "image.h"

Image::Image()
{
	sprite.push_back(new olc::Sprite());
	decal.push_back(new olc::Decal(sprite.at(sprite.size() - 1)));
}

Image::Image(string file, vi2d pos)
{
	filepath = file;
	sprite.push_back(new olc::Sprite(file));
	decal.push_back(new olc::Decal(sprite.at(sprite.size() - 1)));
	position = pos;
}

Image::Image(vi2d size, vi2d pos)
{
	sprite.push_back(new olc::Sprite(size.x, size.y));
	decal.push_back(new olc::Decal(sprite.at(sprite.size() - 1)));
	position = pos;
}

Image::Image(string file)
{
	filepath = file;
	sprite.push_back(new olc::Sprite(file));
	decal.push_back(new olc::Decal(sprite.at(sprite.size() - 1)));
}

Image::Image(vi2d size)
{
	sprite.push_back(new olc::Sprite(size.x, size.y));
	decal.push_back(new olc::Decal(sprite.at(sprite.size() - 1)));
}

void Image::animate(float elapsed)
{
	if (secAnimate == 0)
		return;
	static float total = 0;
	total += elapsed;
	if (secAnimate < total)
	{
		frame++;
		if (frame > decal.size() - 1)
			frame = 0;
		total = 0;
	}
}

Decal* Image::getDecal()
{
	return decal.at(frame);
}
Sprite* Image::getSprite()
{
	return sprite.at(frame);
}
void Image::setSprite(int location, string file)
{
	if (location > -1 && location < sprite.size())
	{
		sprite.at(location) = new olc::Sprite(file);
		decal.at(location) = new olc::Decal(sprite.at(location));
		update();
	}
}

void Image::update()
{
	for(int i = 0; i < decal.size(); i++)
		decal.at(i)->Update();
}