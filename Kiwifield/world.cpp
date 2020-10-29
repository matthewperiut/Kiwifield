#include "world.h"
#include "dynamicPoint.h"

// Stage moving, Stage 1 -> 2 -> 3 using teleport pads or something.

void World::Construct()
{
	player = new Player(vi2d(1, 1), *g);
	editor = new Editor(*stage, *g);
}

World::World(PixelGameEngine& engine)
{
	g = &engine;
	Construct();
}
World::World()
{

}

World::~World()
{
	delete stage;
	delete editor;
	delete player;
}
void World::Keyboard()
{
	if (g->GetKey(Key::CTRL).bHeld)
	{
		if (g->GetKey(Key::S).bPressed)
		{
			stage->save();
		}
		if (g->GetKey(Key::N).bPressed)
		{
			string stagename;
			std::cout << "Stage name:\n";
			std::cin >> stagename;
			if (fs::exists("./stages/" + stagename + "/"))
			{
				delete stage;
				stage = new Stage(stagename, *g);
				delete editor;
				editor = new Editor(*stage, *g);
				delete player;
				player = new Player(vi2d(20, 20), *g);
			}
			else
			{
				int x, y;
				std::cout << "Width:\n";
				std::cin >> x;
				std::cout << "Height:\n";
				std::cin >> y;
				delete stage;
				stage = new Stage(stagename, vi2d(x, y), *g);
				delete editor;
				editor = new Editor(*stage, *g);
				delete player;
				player = new Player(vi2d(1, 1), *g);
				player->vel = vi2d(0, 0);
			}
		}
		if (g->GetKey(Key::R).bPressed)
		{
			player->pos = { 1, 1 };
		}
	}
}

bool World::ChangeStage()
{
	if (loadStage)
	{
		delete stage;
		stage = new Stage(wantedStage, *g);
		delete editor;
		editor = new Editor(*stage, *g);

		player->pos = wantedPos;
		loadStage = false;
		return true;
	}
	return false;
}

void World::Portals(float fElapsedTime)
{
	for (int i = 0; i < stage->portals.size(); i++)
	{
		if (stage->portals[i].Update(fElapsedTime, player->pos, *g))
		{
			wantedStage = stage->portals[i].destination;
			wantedPos = stage->portals[i].desPos;
			loadStage = true;
		}
	}
}

void World::AttackDemo(float fElapsedTime, float frequency = 0.2, float velocityMultiplier = 100, float maxTime = 5, int maxBounces = 0)
{
	static vector<DynamicPoint> dps;
	static vector<float> dtimers;
	static vector<int> bounces;
	static float timer = 0;
	static Img dp("./assets/pellet.png");
	timer += fElapsedTime;
	if (g->GetKey(Key::SHIFT).bHeld && timer > frequency)
	{
		vi2d localplayer = player->pos + vi2d(g->cam.getX(), g->cam.getY() - player->size.y / 2);
		vi2d localmouse = g->GetMousePos();
		double y = (localplayer.y - localmouse.y);
		double x = (localplayer.x - localmouse.x);
		double greater;
		if (abs(x) > abs(y))
			greater = x;
		else
			greater = y;
		vf2d normalized = vf2d(x / abs(greater), y / abs(greater));
		normalized = -normalized;

		timer = 0;
		dps.push_back(DynamicPoint());
		dtimers.push_back(0.f);
		bounces.push_back(0);
		dps[dps.size() - 1].pos = localplayer - vi2d(g->cam.getX(), g->cam.getY());
		dps[dps.size() - 1].vel = normalized * velocityMultiplier;
	}

	for (int i = 0; i < dps.size(); i++)
	{
		if (dps[i].Left() || dps[i].Right())
		{
			dps[i].vel.x = -dps[i].vel.x;
			bounces[i]++;
		}
		if (dps[i].Up() || dps[i].Down())
		{
			dps[i].vel.y = -dps[i].vel.y;
			bounces[i]++;
		}
		
		dtimers[i] += fElapsedTime;
		if (dtimers[i] > maxTime)
		{
			bounces[i] = maxBounces + 1;
		}

		if (bounces[i] > maxBounces)
		{
			dps.erase(dps.begin() + i);
			dtimers.erase(dtimers.begin() + i);
			bounces.erase(bounces.begin() + i);
			continue;
		}

		dps[i].Move(fElapsedTime, *stage);
		g->DrawDecal(dps[i].pos + vi2d(g->cam.getX(), g->cam.getY()), dp.GetDecPtr(), vf2d(1, 1));
	}
	if (g->GetKey(Key::ESCAPE).bPressed)
	{
		dps.clear();
		dtimers.clear();
		bounces.clear();
	}
}

void World::Update(float fElapsedTime)
{
	g->Clear(olc::BLANK);
	
	if (ChangeStage())
	{
		fElapsedTime = 0;
	}
	
	AttackDemo(fElapsedTime);

	Portals(fElapsedTime);
	Keyboard();

	editor->manager();
	stage->Update(fElapsedTime, player->pos);

	

	player->keyboardInput(fElapsedTime, *stage);
}
