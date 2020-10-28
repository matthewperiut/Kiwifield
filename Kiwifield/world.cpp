#include "world.h"

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
				player->velocity = vi2d(0, 0);
			}
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

void World::Update(float fElapsedTime)
{
	g->Clear(olc::BLANK);

	//std::cout << player->pos << std::endl;

	if (ChangeStage())
	{
		fElapsedTime = 0;
	}

	Portals(fElapsedTime);
	Keyboard();

	editor->manager();
	stage->Update(fElapsedTime, player->pos);
	player->keyboardInput(fElapsedTime, *stage);
}
