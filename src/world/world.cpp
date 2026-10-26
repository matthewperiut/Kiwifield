#include "world.h"

// Stage moving, Stage 1 -> 2 -> 3 using teleport pads or something.

World::World(PixelGameEngine& engine)
{
	g = &engine;
	Construct();
}

void World::Construct()
{
	stage = make_unique<Stage>(wantedStage, *g);
	editor = make_unique<Editor>(*stage, *g, prompt);
	player = make_unique<Player>(vi2d(1, 1), *g);

	editor->changeStageBoolPtr = &loadStage;
	editor->changeStageNamePtr = &wantedStage;
}

void World::Keyboard()
{
	if (!g->GetKey(Key::SHIFT).bHeld)
		return;

	if (g->GetKey(Key::S).bPressed)
	{
		stage->Save();
	}
	// Stage swaps would break a portal that is half placed, so those keys wait
	if (editor->creatingPortal)
		return;

	if (g->GetKey(Key::N).bPressed)
	{
		prompt.AskText("Stage name:", [this](const string& name) { OpenOrCreateStage(name); });
	}
	if (g->GetKey(Key::R).bPressed)
	{
		loadStage = true;
	}
}

// Existing stage: load it. New name: ask for a size, then make a blank stage (save with Shift+S).
void World::OpenOrCreateStage(const string& name)
{
	if (name.empty())
		return;

	if (fs::exists("./assets/stages/" + name + "/"))
	{
		stage = make_unique<Stage>(name, *g);
		editor->SetStage(*stage);
		player = make_unique<Player>(vi2d(20, 20), *g);
		return;
	}

	prompt.AskInt("Width:", [this, name](int width)
	{
		prompt.AskInt("Height:", [this, name, width](int height)
		{
			stage = make_unique<Stage>(name, vi2d(width, height), *g);
			editor->SetStage(*stage);
			player = make_unique<Player>(vi2d(1, 1), *g);
			player->vel = { 0, 0 };
		});
	});
}

bool World::ChangeStage()
{
	if (loadStage)
	{
		stage = make_unique<Stage>(wantedStage, *g);
		editor->SetStage(*stage);
		pellets.Clear();

		player->pos = wantedPos;
		player->vel = { 0,0 };
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

	if (ChangeStage())
	{
		fElapsedTime = 0;
	}

	// While typing, gameplay and editor get no input. Stage still draws so the camera and background stay correct.
	if (prompt.Active())
	{
		prompt.Update(*g);
		stage->Update(fElapsedTime, player->pos);
	}
	else
	{
		// Shooter sits half a player-height above the feet, matching where the player sprite is drawn
		pellets.Update(fElapsedTime, *g, *stage, vi2d(int(player->pos.x), int(player->pos.y) - player->size.y / 2));

		// Walking into a portal mid-placement would swap stages under the editor
		if (!editor->creatingPortal)
			Portals(fElapsedTime);
		Keyboard();
		editor->Manager();

		stage->Update(fElapsedTime, player->pos);
		player->KeyboardInput(fElapsedTime, *stage);
	}

	prompt.Draw(*g);
}
