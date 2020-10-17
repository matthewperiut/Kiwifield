#include "stage.h"

void inline Stage::createCollisionVector()
{
	for (int y = 0; y < stageSize.y; y++)
	{
		collision.push_back({});
		for (int x = 0; x < stageSize.x; x++)
		{
			collision[y].push_back(0);
		}
	}
	for (int x = 0; x < getWidth(); x++)
	{
		setCollision(vi2d(x, getHeight() - 1), true);
		setCollision(vi2d(x, 0), true);
	}
	for (int y = 0; y < getHeight(); y++)
	{
		setCollision(vi2d(getWidth() - 1, y), true);
		setCollision(vi2d(0, y), true);
	}
}

Stage::Stage(string name, vi2d size, PixelGameEngine& g)
{
	this->name = name;
	stageSize = vi2d(size.y,size.x);
	createCollisionVector();
	this->g = &g;
}


Stage::Stage(string name, PixelGameEngine& g)
{
	this->name = name;
	load(name);
	this->g = &g;
}

void Stage::save()
{
	string filename = name;
	const static string folder = "./stages/";
	string mkdir = (folder + filename + "/");

	if(!fs::exists(folder))
		fs::create_directory(folder);
	if (!fs::exists(mkdir))
		fs::create_directory(mkdir);
	string col = mkdir + filename + ".col";
	if (!fs::exists(col))
		fs::remove(col);
	string scn = mkdir + filename + ".scn";
	if (!fs::exists(scn))
		fs::remove(scn);

	ofstream myfile;
	myfile.open(scn);

	//s for size
	myfile << 's' << ' ';
	myfile << getWidth() << ' ';
	myfile << getHeight() << ' ';

	for (int i = 0; i < images.size(); i++)
	{
		// i = image
		myfile << 'i' << ' ';
		myfile << images[i].position.x << " ";
		myfile << images[i].position.y << " ";
		myfile << images[i].filepath << " ";
	}
	myfile.close();

	ofstream file;
	
	file.open(col);
	
	for (int y = 0; y < stageSize.y; y++)
	{
		for (int x = 0; x < stageSize.x; x++)
		{
			file << collision.at(y).at(x);
		}
		file << "e\n";
	}
	file.close();
}

void Stage::load(string filename)
{
	fstream myfile;
	
	char code;
	myfile.open("./stages/" + filename + "/" + filename + ".scn");

	while (myfile >> code)
	{
		string path;
		switch (code)
		{
		case 'i':
			int ix, iy;
			myfile >> ix;
			myfile >> iy;
			myfile >> path;
			images.push_back( Image( path, vi2d(ix,iy) ) );
			break;
		case 's':
			int sx, sy;

			myfile >> sx;
			myfile >> sy;
			stageSize.x = sy;
			stageSize.y = sx; 
			break;
		}
	}
	myfile.close();

	ifstream input_file("./stages/" + filename + "/" + filename + ".col");
	if (!input_file.fail())
	{
		char val;
		int x = 0;
		int y = 0;
		collision.push_back({});
		while (input_file >> val)
		{
			if (val == '0')
			{
				collision[y].push_back(false);
				//std::cout << 0;
				x++;
			}
			else if (val == '1')
			{
				collision[y].push_back(true);
				//std::cout << 1;
				x++;
			}
			else if (val == 'e')
			{
				x = 0;
				y++;
				//std::cout << '\n';
				collision.push_back({});
			}
		}
	}
	input_file.close();
}

bool Stage::inbound(vi2d pos)
{
	if (pos.x >= 0 && pos.x < getWidth())
		return true;
	else
		return false;
}
void Stage::setCollision(vi2d pos, bool boolean)
{
	if (pos.x >= 0 && pos.x < getWidth())
		if (pos.y >= 0 && pos.y < getHeight())
			collision[pos.x][pos.y] = boolean;
}
bool Stage::getCollision(vi2d pos)
{
	if(pos.x >= 0 && pos.x < getWidth())
		if (pos.y >= 0 && pos.y < getHeight())
			return collision[pos.x][pos.y];

	return false;
}
int Stage::getWidth()
{
	return stageSize.y;
}
int Stage::getHeight()
{
	return stageSize.x;
}

void Stage::cameraFollow(vi2d pos)
{
	static bool init = false;
	static bool small[2] = { false, false };

	if (!init)
	{
		if (getWidth() < g->ScreenWidth())
			small[0] = true;

		if (getHeight() < g->ScreenHeight())
			small[1] = true;

		init = true;
	}


	const vi2d boundingSize = { 100, 50 };
	vi2d middle = { pos.x - (g->ScreenWidth() / 2), pos.y - (g->ScreenHeight() / 2) };

	if (!small[0])
	{
		int expectedCamXLeft = pos.x - (g->ScreenWidth() / 2);
		int expectedCamXRight = expectedCamXLeft + g->ScreenWidth();
		if (expectedCamXLeft >= 0 && expectedCamXRight < getWidth() + 1)
		{
			g->cam.x = expectedCamXLeft;
		}
		else
		{
			if (expectedCamXLeft < 0)
				g->cam.x = 0;
			if (expectedCamXRight > getWidth())
				g->cam.x = getWidth() - g->ScreenWidth();
		}
	}
	else
	{
		int midstage = getWidth() / 2;
		int differenceToCamera = g->ScreenWidth() / 2;

		g->cam.x = midstage - differenceToCamera;
	}

	if (!small[1])
	{
		int expectedCamYTop = pos.y - (g->ScreenHeight() / 2);
		int expectedCamYBottom = expectedCamYTop + g->ScreenHeight();
		if (expectedCamYTop >= 0 && expectedCamYBottom < getHeight() + 1)
		{
			g->cam.y = expectedCamYTop;
		}
		else
		{
			if (expectedCamYTop < 0)
				g->cam.y = 0;
			if (expectedCamYBottom > getHeight())
				g->cam.y = getHeight() - g->ScreenHeight();
		}
	}
	else
	{
		int midstage = getHeight() / 2;
		int differenceToCamera = g->ScreenHeight() / 2;

		g->cam.y = midstage - differenceToCamera;
	}
}
void Stage::drawBackground(string img)
{
	static Image bg = Image(img, vi2d(0, 0));
	static bool init = false;
	static bool small[2] = { false, false };
	if (!init)
	{
		if (getWidth() < g->ScreenWidth())
			small[0] = true;

		if (getHeight() < g->ScreenHeight())
			small[1] = true;
		init = true;
	}
	g->EnableLayer(2, true);
	g->SetDrawTarget(2);

	g->SetPixelMode(Pixel::ALPHA);
	g->Clear(olc::BLANK);

	
	
	if (small[0])
	{
		
	}
	if (small[1])
	{

	}
	if(!small[0] && !small[1])
	{
		const static double changeRate = 1.25;
		int x = bg.position.x + (g->cam.getX() / changeRate);
		//int y = bg.position.y + (g->cam.getY() / changeRate);

		int reps = int(getWidth() / bg.getSprite()->width) + 1;

		// Drawing decals aren't expensive so I found this as the best method
		for (int i = 0; i < reps; i += 1)
		{
			g->DrawDecal(vi2d(x + (bg.getSprite()->width) * i, 0), bg.getDecal());
		}
	}
	g->EnableLayer(2, true);
	g->SetDrawTarget(nullptr);
}

void Stage::drawImages()
{
	g->EnableLayer(1, true);
	g->SetDrawTarget(1);
	
	//DrawDecal(vi2d(0, 0), d.decal);
	g->SetPixelMode(Pixel::ALPHA);
	g->Clear(olc::BLANK);

	for (int i = 0; i < images.size(); i++)
	{
		g->DrawDecal(vi2d(images[i].position.x + g->cam.getX(), images[i].position.y + g->cam.getY()), images[i].getDecal());
	}
	g->EnableLayer(1, true);
	g->SetDrawTarget(nullptr);
}

void Stage::drawCollider()
{
	vi2d ic = { -g->cam.getY(),-g->cam.getX() };
	for (int x = 0; x < g->ScreenHeight(); x++)
	{
		for (int y = 0; y < g->ScreenWidth(); y++)
		{
			int first = y + ic.y;
			int second = x + ic.x;
			if ((first > -1 && first < collision.size()) && (second > -1 && second < collision[y].size()))
				if (collision[first][second])
					g->Draw(vi2d(y, x), Pixel(255, 0, 0));
			
		}
	}
	
		
}
