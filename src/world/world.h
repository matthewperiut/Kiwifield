#pragma once
#include <memory>
#include "stage.h"
#include "../entities/player.h"
#include "../editor/editor.h"
#include "../utility/textprompt.h"
#include "../weapons/pellets.h"

class World
{
public:
	PixelGameEngine* g = nullptr;

	string wantedStage = "examplelevel";
	vi2d wantedPos = { 1, 1 };
	bool loadStage = true;

	World(PixelGameEngine& engine);
	World() = default;
	~World() = default;

	void Construct();
	bool ChangeStage();
	void Portals(float fElapsedTime);
	void Update(float fElapsedTime);

private:
	void Keyboard();
	void OpenOrCreateStage(const string& name);

	unique_ptr<Stage> stage;
	unique_ptr<Player> player;
	unique_ptr<Editor> editor;
	TextPrompt prompt;
	Pellets pellets;
};
