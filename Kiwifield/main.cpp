#define OLC_PGE_APPLICATION
#include <string>
#include <iostream>
#include "olcPixelGameEngine.h"
#include "player.h"
#include "stage.h"
#include "files.h"

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

	bool OnUserCreate() override
	{
		//stage = new Stage(vi2d(300, 144), *this);
        //stage->images.push_back(Image("./scene.png", vi2d(0,0)));
        //stage->save("wack");
		//stage->images.push_back(Image("./image.png", vi2d(260-64, 180 -64)));
		//stage->save("gamer");
		//stage = new Stage("gamer", *this);
        stage = new Stage("wack", *this);
		//cout << stage->collision.size() << " " << stage->collision[0].size();
		//cout << stage->images.size();

		player = new Player(vi2d(20, 20));
		return true;
	}

	bool OnUserDestroy() override
	{
		delete stage;
		delete player;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		Clear(olc::BLANK);
        
        if(GetMouse(0).bPressed)
        {
            std::cout << vi2d(0,144) - GetMousePos() << std::endl;
        }
        
        stage->cameraFollow(player->pos, *this);
        if(!GetKey(Key::SPACE).bHeld)
        {
            stage->drawImages(*this);
        }
		stage->drawCollider(*this);
        
		player->update(fElapsedTime, *stage, *this);

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
