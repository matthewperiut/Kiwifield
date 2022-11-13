#include "../lib/olcPixelGameEngine.h"
#include "../const.h"
#include <vector>

using namespace olc;

void DrawLineInSpr(int& sx, int& ex, int& sy, int& ey, Pixel p, Sprite& file)
{
	for (int x = sx; x <= ex; x++)
		file.SetPixel(x, sy-ey, p);
};
//void DrawLineInSpr(int32_t x1, int32_t y1, int32_t x2, int32_t y2, Sprite& file, Pixel p, uint32_t pattern)
void DrawLineIn2DBoolean(int& x1, int& y1, int& x2, int& y2, std::vector<std::vector<bool>>& collisionArray, uint32_t pattern = 0xFFFFFFFF)
{
	auto Draw = [&](int x, int y, bool p)
	{
		collisionArray[x][y] = p;
	};

	bool p = 1;

	int x, y, dx, dy, dx1, dy1, px, py, xe, ye, i;
	dx = x2 - x1; dy = y2 - y1;

	auto rol = [&](void) { pattern = (pattern << 1) | (pattern >> 31); return pattern & 1; };

	// straight lines idea by gurkanctn
	if (dx == 0) // Line is vertical
	{
		if (y2 < y1) std::swap(y1, y2);
		for (y = y1; y <= y2; y++) if (rol()) Draw(x1, y, p);
		return;
	}

	if (dy == 0) // Line is horizontal
	{
		if (x2 < x1) std::swap(x1, x2);
		for (x = x1; x <= x2; x++) if (rol()) Draw(x, y1, p);
		return;
	}

	// Line is Funk-aye
	dx1 = abs(dx); dy1 = abs(dy);
	px = 2 * dy1 - dx1;	py = 2 * dx1 - dy1;
	if (dy1 <= dx1)
	{
		if (dx >= 0)
		{
			x = x1; y = y1; xe = x2;
		}
		else
		{
			x = x2; y = y2; xe = x1;
		}

		if (rol()) Draw(x, y, p);

		for (i = 0; x < xe; i++)
		{
			x = x + 1;
			if (px < 0)
				px = px + 2 * dy1;
			else
			{
				if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) y = y + 1; else y = y - 1;
				px = px + 2 * (dy1 - dx1);
			}
			if (rol()) Draw(x, y, p);
		}
	}
	else
	{
		if (dy >= 0)
		{
			x = x1; y = y1; ye = y2;
		}
		else
		{
			x = x2; y = y2; ye = y1;
		}

		if (rol()) Draw(x, y, p);

		for (i = 0; y < ye; i++)
		{
			y = y + 1;
			if (py <= 0)
				py = py + 2 * dx1;
			else
			{
				if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) x = x + 1; else x = x - 1;
				py = py + 2 * (dx1 - dy1);
			}



			if (rol()) Draw(x, y, p);
		}
	}
}

//void DrawLine(const olc::vi2d& pos1, const olc::vi2d& pos2, Pixel p = olc::WHITE, uint32_t pattern = 0xFFFFFFFF);
void DrawLineInSpr(int32_t x1, int32_t y1, int32_t x2, int32_t y2, Sprite& file, Pixel p, uint32_t pattern)
{
	auto Draw = [&](int x, int y, Pixel p)
	{
		file.SetPixel(x, y, p);
	};

	int x, y, dx, dy, dx1, dy1, px, py, xe, ye, i;
	dx = x2 - x1; dy = y2 - y1;

	auto rol = [&](void) { pattern = (pattern << 1) | (pattern >> 31); return pattern & 1; };

	// straight lines idea by gurkanctn
	if (dx == 0) // Line is vertical
	{
		if (y2 < y1) std::swap(y1, y2);
		for (y = y1; y <= y2; y++) if (rol()) Draw(x1, y, p);
		return;
	}

	if (dy == 0) // Line is horizontal
	{
		if (x2 < x1) std::swap(x1, x2);
		for (x = x1; x <= x2; x++) if (rol()) Draw(x, y1, p);
		return;
	}

	// Line is Funk-aye
	dx1 = abs(dx); dy1 = abs(dy);
	px = 2 * dy1 - dx1;	py = 2 * dx1 - dy1;
	if (dy1 <= dx1)
	{
		if (dx >= 0)
		{
			x = x1; y = y1; xe = x2;
		}
		else
		{
			x = x2; y = y2; xe = x1;
		}

		if (rol()) Draw(x, y, p);

		for (i = 0; x < xe; i++)
		{
			x = x + 1;
			if (px < 0)
				px = px + 2 * dy1;
			else
			{
				if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) y = y + 1; else y = y - 1;
				px = px + 2 * (dy1 - dx1);
			}
			if (rol()) Draw(x, y, p);
		}
	}
	else
	{
		if (dy >= 0)
		{
			x = x1; y = y1; ye = y2;
		}
		else
		{
			x = x2; y = y2; ye = y1;
		}

		if (rol()) Draw(x, y, p);

		for (i = 0; y < ye; i++)
		{
			y = y + 1;
			if (py <= 0)
				py = py + 2 * dx1;
			else
			{
				if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) x = x + 1; else x = x - 1;
				py = py + 2 * (dx1 - dy1);
			}

			

			if (rol()) Draw(x, y, p);
		}
	}
}

void FillCircleInSpr(int x, int y, int radius, Pixel p, Sprite& file)
{ // Thanks to IanM-Matrix1 #PR121
	if (radius > 0)
	{
		int x0 = 0;
		int y0 = radius;
		int d = 3 - 2 * radius;

		auto drawline = [&](int sx, int ex, int y)
		{
			for (int x = sx; x <= ex; x++)
				file.SetPixel(x, y, p);
		};

		while (y0 >= x0)
		{
			drawline(x - y0, x + y0, y - x0);
			if (x0 > 0)	drawline(x - y0, x + y0, y + x0);

			if (d < 0)
				d += 4 * x0++ + 6;
			else
			{
				if (x0 != y0)
				{
					drawline(x - x0, x + x0, y - y0);
					drawline(x - x0, x + x0, y + y0);
				}
				d += 4 * (x0++ - y0--) + 10;
			}
		}
	}
	else
		file.SetPixel(x, y, p);
}