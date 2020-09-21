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
		stage = new Stage(vi2d(300, 200), *this);
        
		CreateLayer();
		CreateLayer();

		std::vector<LayerDesc>& gamer = GetLayers();
		for (int i = 0; i < gamer.size(); i++)
		{
			std::cout << gamer[i].nResID - 2 << std::endl;
		}

		stage->images.push_back(Image("./image.png", vi2d(0, 0) ));
		stage->images.push_back(Image("./image.png", vi2d(50, 20) ));
        stage->images.push_back(Image("./image.png", vi2d(100, 30) ));
        stage->images.push_back(Image("./image.png", vi2d(30, 100) ));
        
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
        
		
		stage->cameraFollow(player->pos);
		stage->drawBackground("./assets/skies/skiesrepeating1.png");
		stage->drawImages();
		stage->drawCollider();
        
        editor->manager();
        
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
