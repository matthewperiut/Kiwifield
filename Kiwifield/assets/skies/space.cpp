#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

using namespace olc;

class Space : public olc::PixelGameEngine
{
public:
	Space()
	{
		sAppName = "Space";
	}

public:
	bool OnUserCreate() override
	{
		// Softmilk 32 pallette
		Clear(Pixel(35, 33, 61));
		Pixel pixels[3] = { Pixel(87,84,111),Pixel(118,126,125),Pixel(185,181,195) };

		srand(2002);
		for (int x = 0; x < ScreenWidth(); x++)
			for (int y = 0; y < ScreenHeight(); y++)
			{
			
					if (rand() % (200+(y*10))== 0 && y > 3)
					{
						int type = rand() % 3;
						DrawCircle(x, y, type, pixels[type]);
					}
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
	Space space;
	if (space.Construct(256, 144, 1, 1))
		space.Start();
	return 0;
}