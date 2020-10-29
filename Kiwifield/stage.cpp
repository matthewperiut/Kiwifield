#include "stage.h"

void inline Stage::CreateCollisionVector()
{
	for (int y = 0; y < stageSize.y; y++)
	{
		collision.push_back({});
		for (int x = 0; x < stageSize.x; x++)
		{
			collision[y].push_back(0);
		}
	}
	for (int x = 0; x < GetWidth(); x++)
	{
		SetCollision(vi2d(x, GetHeight() - 1), true);
		SetCollision(vi2d(x, 0), true);
	}
	for (int y = 0; y < GetHeight(); y++)
	{
		SetCollision(vi2d(GetWidth() - 1, y), true);
		SetCollision(vi2d(0, y), true);
	}
}

Stage::Stage(string name, vi2d size, PixelGameEngine& g)
{
	this->name = name;
	stageSize = vi2d(size.y,size.x);
	CreateCollisionVector();
	this->g = &g;
}


Stage::Stage(string name, PixelGameEngine& g)
{
	this->name = name;
	Load(name);
	this->g = &g;
}

void Stage::Save()
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
	myfile << GetWidth() << ' ';
	myfile << GetHeight() << ' ';

	//b for background
	if (backgroundPath != "")
	{
		myfile << 'b' << ' ';
		myfile << backgroundPath << ' ';
	}

	for (int i = 0; i < imgs.size(); i++)
	{
		// i = image
		myfile << 'i' << ' ';
		myfile << imgs[i].position.x << ' ';
		myfile << imgs[i].position.y << ' ';
		myfile << imgs[i].filePath << ' ';
	}

	for (int p = 0; p < portals.size(); p++)
	{
		myfile << "p ";
		myfile << portals[p].pos.x << ' ';
		myfile << portals[p].pos.y << ' ';
		myfile << portals[p].destination << ' ';
		myfile << portals[p].desPos.x << ' ';
		myfile << portals[p].desPos.y << ' ';
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
	}
	file.close();
}

void Stage::Load(string filename)
{
	fstream myfile;
	
	char code;
	myfile.open("./stages/" + filename + "/" + filename + ".scn");

	string path;
	while (myfile >> code)
	{
		switch (code)
		{
		case 'i':
			int ix, iy;
			myfile >> ix;
			myfile >> iy;
			myfile >> path;
			imgs.push_back(Img(path));
			imgs[imgs.size() - 1].position = vi2d(ix, iy);
			break;
		case 's':
			int sx, sy;

			myfile >> sx;
			myfile >> sy;
			stageSize.x = sy;
			stageSize.y = sx; 
			break;
		case 'b':
			myfile >> backgroundPath;
			break;
		case 'p':
			int desx, desy;;
			int locx, locy;
			myfile >> locx;
			myfile >> locy;
			myfile >> path;
			myfile >> desx;
			myfile >> desy;
			Portal p(vi2d(locx, locy));
			p.Destination(path, vi2d(desx, desy));
			portals.push_back(p);
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
			if (x == stageSize.x)
			{
				x = 0;
				y++;
				collision.emplace_back();
			}
		}
	}
	input_file.close();
}

bool Stage::Inbound(vi2d pos)
{
	if (pos.x >= 0 && pos.x < GetWidth())
		return true;
	else
		return false;
}
void Stage::SetCollision(vi2d pos, bool boolean)
{
	if (pos.x >= 0 && pos.x < GetWidth())
		if (pos.y >= 0 && pos.y < GetHeight())
			collision[pos.x][pos.y] = boolean;
}
bool Stage::GetCollision(vi2d pos)
{
	if(pos.x >= 0 && pos.x < GetWidth())
		if (pos.y >= 0 && pos.y < GetHeight())
			return collision[pos.x][pos.y];

	return false;
}
int Stage::GetWidth()
{
	return stageSize.y;
}
int Stage::GetHeight()
{
	return stageSize.x;
}

void Stage::CameraFollow(vi2d pos)
{
	static bool init = false;
	static bool small[2] = { false, false };

	if (!init)
	{
		if (GetWidth() < g->ScreenWidth())
			small[0] = true;

		if (GetHeight() < g->ScreenHeight())
			small[1] = true;

		init = true;
	}


	const vi2d boundingSize = { 100, 50 };
	vi2d middle = { pos.x - (g->ScreenWidth() / 2), pos.y - (g->ScreenHeight() / 2) };

	if (!small[0])
	{
		int expectedCamXLeft = pos.x - (g->ScreenWidth() / 2);
		int expectedCamXRight = expectedCamXLeft + g->ScreenWidth();
		if (expectedCamXLeft >= 0 && expectedCamXRight < GetWidth() + 1)
		{
			g->cam.x = expectedCamXLeft;
		}
		else
		{
			if (expectedCamXLeft < 0)
				g->cam.x = 0;
			if (expectedCamXRight > GetWidth())
				g->cam.x = GetWidth() - g->ScreenWidth();
		}
	}
	else
	{
		int midstage = GetWidth() / 2;
		int differenceToCamera = g->ScreenWidth() / 2;

		g->cam.x = midstage - differenceToCamera;
	}

	if (!small[1])
	{
		int expectedCamYTop = pos.y - (g->ScreenHeight() / 2);
		int expectedCamYBottom = expectedCamYTop + g->ScreenHeight();
		if (expectedCamYTop >= 0 && expectedCamYBottom < GetHeight() + 1)
		{
			g->cam.y = expectedCamYTop;
		}
		else
		{
			if (expectedCamYTop < 0)
				g->cam.y = 0;
			if (expectedCamYBottom > GetHeight())
				g->cam.y = GetHeight() - g->ScreenHeight();
		}
	}
	else
	{
		int midstage = GetHeight() / 2;
		int differenceToCamera = g->ScreenHeight() / 2;

		g->cam.y = midstage - differenceToCamera;
	}
}
void Stage::DrawBackground(string img)
{
	if (img == "")
		return;

	static string imgpath;
	static Img bg;

	if (img != imgpath)
	{
		imgpath = img;
		bg = Img(img, vi2d(0, 0));
	}
		

	static bool init = false;
	static bool small[2] = { false, false };
	if (!init)
	{
		if (GetWidth() < g->ScreenWidth())
			small[0] = true;

		if (GetHeight() < g->ScreenHeight())
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
		int x = bg.position.x + (g->cam.GetX() / changeRate);
		//int y = bg.position.y + (g->cam.getY() / changeRate);

		int reps = int(GetWidth() / bg.GetSprPtr()->width) + 1;

		// Drawing decals aren't expensive so I found this as the best method
		for (int i = 0; i < reps; i += 1)
		{
			g->DrawDecal(vi2d(x + (bg.GetSprPtr()->width) * i, 0), bg.GetDecPtr());
		}
	}
	g->EnableLayer(2, true);
	g->SetDrawTarget(nullptr);
}

void Stage::Update(float fElapsedTime, vf2d& p)
{
	CameraFollow(p);
	DrawBackground(backgroundPath);

	g->EnableLayer(1, true);
	g->SetDrawTarget(1);
	
	//DrawDecal(vi2d(0, 0), d.decal);
	g->SetPixelMode(Pixel::ALPHA);
	g->Clear(olc::BLANK);

	for (int i = 0; i < imgs.size(); i++)
	{
		g->DrawDecal(vi2d(imgs[i].position.x + g->cam.GetX(), imgs[i].position.y + g->cam.GetY()), imgs[i].GetDecPtr());
	}
	g->EnableLayer(1, true);
	g->SetDrawTarget(nullptr);
}

void Stage::DrawCollider()
{
	vi2d ic = { -g->cam.GetY(),-g->cam.GetX() };
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
	/*
	for (int y = 0; y < collision.size(); y++)
	{
		for (int x = 0; x < collision[y].size(); x++)
		{
			g->Draw(vi2d(y + g->cam.getX(), x + g->cam.getY()), Pixel(255 * collision[y][x], 0, 0));
		}
	}
	*/
}
