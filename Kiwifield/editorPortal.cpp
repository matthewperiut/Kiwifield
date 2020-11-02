#include "editor.h"

void Editor::EditPortal()
{
	g->DrawStringDecal(vi2d(0, 0), "EditPortal");
	g->DrawStringDecal(vi2d(0, 16), "Create C");
	g->DrawStringDecal(vi2d(0, 24), "Move M");
	g->DrawStringDecal(vi2d(0, 32), "Delete X");
	g->DrawStringDecal(vi2d(0, 40), "Cycle >");

	if (stage->portals.empty())
		return;

	
	static int mode = 0;
	enum modes { create = 1, remove, move, cycle };
	if (chosenPortal >= 0 && chosenPortal < stage->portals.size())
	{
		if(g->GetKey(Key::C).bPressed)
		{
			cout << "Hello";
			CreatePortal();
			cout << "bai";
			mode = create;
		}
		if (g->GetKey(Key::M).bPressed)
		{
			MovePortal(stage->portals[chosenPortal]);
			mode = move;
		}
		if (g->GetKey(Key::X).bPressed)
		{
			RemovePortal();
		}
		if (g->GetKey(Key::RIGHT).bPressed)
		{
			
		}
	}
	else
	{
		chosenPortal = 0;
	}
	if(mode == move)
	{
		if(MovePortal(stage->portals[chosenPortal]))
		{
			mode = 0;
		}
	}
}

bool Editor::CreatePortal()
{
	/*
	cout << "hello";
	static Portal* fr = new Portal(vi2d(1, 1));
	static Portal* af = new Portal(vi2d(1, 1));
	cout << "?";
	Portal& from = *fr;
	Portal& after = *af;
	cout << "oh";
	static int part = 0;
	
	if (part == 0)
	{
		//from position
		cout << "boink";
		from.destination = stage->name;
		from.Update(1, vi2d(100, 100), *g);
		from.desPos = vi2d(1, 1);
		if (MovePortal(from))
			part++;
		return false;
	}
	else if (part == 1)
	{
		from.Update(0, vi2d(100, -100), *g);
		g->DrawStringDecal(vf2d(0, float(g->ScreenHeight()) - 8), "CHECK CONSOLE");
		part++;
		return false;
	}
	else if(part == 2)
	{
		*changeStageBoolPtr = true;
		
		while (true)
		{
			cout << "Input a stage for the portal to lead to:";
			cin >> *changeStageNamePtr;

			if(fs::exists("./stages/" + *changeStageNamePtr))
				break;
		}

		part++;
		return false;
	}
	else if (part == 3)
	{
		//from position
		after.Update(0.001f, vi2d(100, -100), *g);
		if (MovePortal(after))
		{
			after.desPos = from.pos;
			from.desPos = after.pos;
			after.destination = from.destination;
			from.destination = stage->name;
			part++;
		}
		return false;
	}
	else if (part == 4)
	{
		std::ofstream myfile;

		myfile.open("./stages/" + after.destination + "/" + after.destination + ".scn", std::ios_base::app); // append instead of overwrite
		myfile << " p ";
		myfile << from.pos.x << ' ';
		myfile << from.pos.y << ' ';
		myfile << from.destination << ' ';
		myfile << from.desPos.x << ' ';
		myfile << from.desPos.y << ' ';
		myfile.close();
		
		myfile.open("./stages/" + from.destination + "/" + from.destination + ".scn", std::ios_base::app); // append instead of overwrite
		myfile << " p ";
		myfile << after.pos.x << ' ';
		myfile << after.pos.y << ' ';
		myfile << after.destination << ' ';
		myfile << after.desPos.x << ' ';
		myfile << after.desPos.y << ' ';
		myfile.close();

		part++;
	}
	else if(part == 5)
	{
		delete af;
		delete fr;
		*changeStageBoolPtr = true;
	}
	cout << "yoink";
	return true;
	*/
}

bool Editor::MovePortal(Portal& p)
{
	
	p.pos = g->GetMousePos() - vi2d(g->cam.GetX(), g->cam.GetY());

	if (g->GetMouse(0).bPressed)
	{
		return true;
	}
	
	return false;
}

void Editor::RemovePortal()
{
	if (!stage->portals.empty() && chosenPortal < stage->portals.size())
	{
		//stage->portals.erase(stage->portals.begin() + chosenPortal);
		if (chosenPortal > 1 && chosenPortal < stage->portals.size())
			chosenPortal--;
		else
			chosenPortal = 0;
	}
}