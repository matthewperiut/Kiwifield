#include "image.h"

Image::Image()
{
	sprite = new olc::Sprite();
	decal = new olc::Decal(sprite);
}

Image::Image(string file, vi2d pos)
{
	filepath = file;
	sprite = new olc::Sprite(file);
	decal = new olc::Decal(sprite);
	position = pos;
}

Image::Image(vi2d size, vi2d pos)
{
	sprite = new olc::Sprite(size.x, size.y);
	decal = new olc::Decal(sprite);
	position = pos;
}

Image::Image(string file)
{
	filepath = file;

	sprite = new olc::Sprite(file);
	decal = new olc::Decal(sprite);
}

Image::Image(vi2d size)
{
	sprite = new olc::Sprite(size.x, size.y);
	decal = new olc::Decal(sprite);
}

void Image::update()
{
	decal->Update();
}