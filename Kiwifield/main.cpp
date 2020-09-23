#define OLC_PGE_APPLICATION
#include <string>
#include <iostream>
#include "olcPixelGameEngine.h"
#include "player.h"
#include "stage.h"
#include "files.h"
#include "editor.h"

class Game : public olc::PixelGameEngine
{
public:
	Game()
	{
		sAppName = "Kiwifield";
	}
public:
	Player* player;
	Stage* stage;
    Editor* editor;

	bool OnUserCreate() override
	{
		stage = new Stage(vi2d(256, 144), *this);
        
		// Layer 0 is default starting
		CreateLayer(); // Layer 1
		CreateLayer(); // Layer 2

		stage->images.push_back(Image("./assets/mario.png", vi2d(100, 0) ));
		stage->images.push_back(Image("./assets/mario.png", vi2d(100, 10) ));
        stage->images.push_back(Image("./assets/mario.png", vi2d(100, 20) ));
        stage->images.push_back(Image("./assets/mario.png", vi2d(100, 30) ));
        
        editor = new Editor(*stage, *this);

		player = new Player(vi2d(20, 20), *this);
		return true;
	}

	bool OnUserDestroy() override
	{
		delete stage;
		delete player;
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		Clear(olc::BLANK);

		editor->manager();
		
		stage->cameraFollow(player->pos);
		stage->drawBackground("./assets/skies/skiesrepeating1.png");
		stage->drawImages();
        
        
        
		player->keyboardInput(fElapsedTime, *stage);
		
		return true;
	}
};


int main()
{
	Game game;
	if (game.Construct(wWidth, wHeight, 5, 5, false, true))
		game.Start();

	return 0;
}
