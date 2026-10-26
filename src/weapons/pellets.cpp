#include "pellets.h"

#ifdef KIWIFIELD_PELLETS

#include <cmath>

namespace
{
	constexpr float kFrequency = 0.2f;    // seconds between shots
	constexpr float kSpeed = 100.0f;      // pixels per second
	constexpr float kMaxLife = 100.0f;    // seconds before a pellet expires
	constexpr int kMaxBounces = 10;
}

void Pellets::Update(float dt, olc::PixelGameEngine& g, Stage& stage, olc::vi2d shooter)
{
	timer += dt;
	if (g.GetKey(olc::Key::F).bHeld && timer > kFrequency)
		Fire(g, shooter);

	// Walk backwards so erasing index i does not skip the pellet that shifts into it
	for (int i = int(pellets.size()) - 1; i >= 0; i--)
	{
		Pellet& p = pellets[i];
		DynamicPoint& body = p.body;

		if (body.Left() || body.Right())
		{
			body.vel.x = -body.vel.x;
			p.bounces++;
		}
		if (body.Up() || body.Down())
		{
			body.vel.y = -body.vel.y;
			p.bounces++;
		}

		p.life += dt;
		if (p.life > kMaxLife)
			p.bounces = kMaxBounces + 1;

		if (p.bounces > kMaxBounces)
		{
			pellets.erase(pellets.begin() + i);
			continue;
		}

		body.Move(dt, stage);
		g.DrawDecal(body.pos + olc::vf2d(float(g.cam.GetX()), float(g.cam.GetY())), sprite.GetDecPtr(), olc::vf2d(1, 1));
	}

	if (g.GetKey(olc::Key::ESCAPE).bPressed)
		Clear();
}

void Pellets::Fire(olc::PixelGameEngine& g, olc::vi2d shooter)
{
	timer = 0;

	// Aim from the shooter's screen position toward the mouse
	olc::vi2d from = shooter + olc::vi2d(g.cam.GetX(), g.cam.GetY());
	olc::vi2d mouse = g.GetMousePos();
	double dx = from.x - mouse.x;
	double dy = from.y - mouse.y;
	double greater = std::abs(dx) > std::abs(dy) ? dx : dy;

	// Mouse exactly on the shooter would divide by zero
	olc::vf2d dir = greater == 0
		? olc::vf2d(0, 0)
		: olc::vf2d(float(dx / std::abs(greater)), float(dy / std::abs(greater)));
	dir = -dir;

	Pellet p;
	p.body.pos = shooter;
	p.body.vel = dir * kSpeed;
	pellets.push_back(p);
}

void Pellets::Clear()
{
	pellets.clear();
	timer = 0;
}

#else

void Pellets::Update(float, olc::PixelGameEngine&, Stage&, olc::vi2d)
{
}

void Pellets::Clear()
{
}

#endif
