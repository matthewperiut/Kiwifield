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
		
		
		stage = new Stage("examplelevel", *this);
        
		// Layer 0 is default starting
		CreateLayer(); // Layer 1
		CreateLayer(); // Layer 2
        
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

		if (GetKey(Key::CTRL).bHeld)
		{
			if (GetKey(Key::S).bPressed)
			{
				stage->save();
			}
			if (GetKey(Key::N).bPressed)
			{
				string stagename;
				std::cout << "Stage name:\n";
				std::cin >> stagename;
				if (fs::exists("./stages/" + stagename + "/"))
				{
					delete stage;
					stage = new Stage(stagename, *this);
					delete editor;
					editor = new Editor(*stage, *this);
					delete player;
					player = new Player(vi2d(20, 20), *this);
				}
				else
				{
					int x, y;
					std::cout << "Width:\n";
					std::cin >> x;
					std::cout << "Height:\n";
					std::cin >> y;
					delete stage;
					stage = new Stage(stagename, vi2d(x, y), *this);
					delete editor;
					editor = new Editor(*stage, *this);
					delete player;
					player = new Player(vi2d(20, 20), *this);
				}
			}
		}
		
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
