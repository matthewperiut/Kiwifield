#include "img.h"
//string filePath
//unique_ptr<Sprite> spr
//unique_ptr<Decal> dec
void Img::SetFilePath(string path)
{
	filePath = path;
	spr = std::make_unique<olc::Sprite>(filePath);
	dec = std::make_unique<olc::Decal>(spr.get());
}
void Img::SetImgSize(vi2d size)
{
	spr = std::make_unique<olc::Sprite>(size.x, size.y);
	dec = std::make_unique<olc::Decal>(spr.get());
}

Img::Img()
{
	SetImgSize({ 10, 10 });
}
Img::Img(vi2d size)
{
	SetImgSize(size);
}
Img::Img(string path)
{
	SetFilePath(path);
}
Img::Img(string path, vi2d pos)
{
	SetFilePath(path);
	position = pos;
}

Sprite* Img::GetSprPtr()
{
	return spr.get();
}
Decal* Img::GetDecPtr()
{
	return dec.get();
}
string Img::GetFilePath()
{
	return filePath;
}