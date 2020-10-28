#pragma once
#include "olcPixelGameEngine.h"
#include <memory>
#include <string>

using namespace olc;
using namespace std;

class Img
{
public:
	string filePath;

private:
	unique_ptr<Sprite> spr;
	unique_ptr<Decal> dec;

public:
	vi2d position{ 0, 0 };

public:
	void SetFilePath(string path);
	void SetImgSize(vi2d size);

	Img();
	Img(vi2d size);
	Img(string path);
	Img(string path, vi2d pos);

	Sprite* GetSprPtr();
	Decal* GetDecPtr();
	string GetFilePath();
};