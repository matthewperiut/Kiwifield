#pragma once
#include "olcPixelGameEngine.h"
#include "stage.h"
#include "const.h"
#include <vector>

class Character
{
protected:
	std::string imgpath;
public:
	vf2d pos = { 0, 0 };
	vf2d velocity = { 0, 0 };
protected:
	vi2d size = { 8, 8 };
	vf2d scale = { 1, 1 };
	bool gravity = true;
	bool jump = false;
	bool drawSprite = false;

	PixelGameEngine* g;

	bool flipper = false;

	Sprite* feet;
	Decal* feetdec;

	Sprite* sprite;
	Decal* decal;

	vi2d collisionPixel;
public:
	Character(vf2d pos, PixelGameEngine& g, string img);
	~Character();
	void draw();
	void logic(float time, Stage& stage);
	void move(float time, Stage& stage);
};