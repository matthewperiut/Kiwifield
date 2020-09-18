#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <cmath>
using namespace olc;

class Day1 : public olc::PixelGameEngine
{
public:
	Day1()
	{
		sAppName = "Day1";
	}

public:
	bool OnUserCreate() override
	{
		for (int i = 0; i < ScreenWidth(); i++)
		{
			vi2d pos = vi2d(i, sin((i / (2 * 3.141592)) / 5) * 20 + ScreenHeight() / 2);
			DrawRect(vi2d(i, 0), pos, olc::Pixel(66, 91, 189));
			DrawRect(pos, vi2d(i, ScreenHeight()), olc::Pixel(69, 161, 222));
			FillCircle(pos, 32, Pixel(72,132,212));
		}
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		return true;
	}
};


int main()
{
	Day1 sky;
	if (sky.Construct(512, 288, 1, 1, false, true))
		sky.Start();

	return 0;
}