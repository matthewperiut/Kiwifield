#pragma once
#include <vector>
#include "../lib/olcPixelGameEngine.h"
#include "../entities/dynamicPoint.h"
#include "../utility/img.h"
#include "../world/stage.h"

// Test weapon: hold F to shoot pellets that bounce around the stage, ESC clears them.
// Compiled out unless built with KIWIFIELD_PELLETS (cmake -DKIWIFIELD_PELLETS=ON).
// Without the flag this is an empty class, so callers need no #ifdefs.
class Pellets
{
public:
	// shooter: world position the pellets spawn from
	void Update(float dt, olc::PixelGameEngine& g, Stage& stage, olc::vi2d shooter);
	void Clear();

#ifdef KIWIFIELD_PELLETS
private:
	struct Pellet
	{
		DynamicPoint body;
		float life = 0;
		int bounces = 0;
	};

	void Fire(olc::PixelGameEngine& g, olc::vi2d shooter);

	std::vector<Pellet> pellets;
	float timer = 0;
	Img sprite{ "./assets/pellet.png" };
#endif
};
