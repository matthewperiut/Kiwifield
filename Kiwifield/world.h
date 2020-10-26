#pragma once
#include "stage.h"
#include "player.h"
#include "editor.h"

class World
{
public:
	PixelGameEngine* g;
	Player* player;
	Stage* stage;
	Editor* editor;

	string wantedStage = "examplelevel";
	vi2d wantedPos = { 1, 1 };
	bool loadStage = true;

	World(PixelGameEngine& engine);
	World();
	~World();

private:
	void Keyboard();

public:
	void Construct();
	bool ChangeStage();
	void Portals(float fElapsedTime);
	void Update(float fElapsedTime);
};