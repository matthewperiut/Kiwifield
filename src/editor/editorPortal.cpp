#include "editor.h"

void Editor::EditPortal()
{
	g->DrawStringDecal(vi2d(0, 0), "EditPortal");
	g->DrawStringDecal(vi2d(0, 8), "Create C");
	
	if (!stage->portals.empty())
	{
		g->DrawStringDecal(vi2d(0, 16), "Delete X");
		g->DrawStringDecal(vi2d(0, 24), "Cycle >");
	}

	static int mode = 0;
	enum modes { create = 1, remove, move, cycle };

	if (g->GetKey(Key::C).bPressed)
	{
		mode = create;
	}
	
	if (chosenPortal >= 0 && chosenPortal < stage->portals.size() && !stage->portals.empty())
	{
		if (g->GetKey(Key::X).bPressed)
		{
			RemovePortal();
		}
		if (g->GetKey(Key::RIGHT).bPressed)
		{
			chosenPortal++;
			if(chosenPortal > stage->portals.size() - 1)
			{
				chosenPortal = 0;
			}
		}
		stage->portals[chosenPortal].DebugDraw(*g);
	}
	else
	{
		chosenPortal = 0;
	}
	if (mode == create)
	{
		if (CreatePortal())
			mode = 0;
	}
}

bool Editor::CreatePortal()
{
	static Portal start(vi2d(10, 10));
	static Portal end(vi2d(10, 10));
	std::ofstream current;
	std::ofstream second;
	static int part = 0;

	if(g->GetKey(Key::ESCAPE).bPressed)
	{
		part = 0;
		return true;
	}
	
	switch(part)
	{
	case 0:
		start.Update(0, vi2d(100, 100), *g);
		if (MovePortal(start))
		{
			start.destination = stage->name;
			start.desPos = vi2d(10, 10);
			part++;
		}
		break;
	case 1:
		start.Update(0, vi2d(100, -100), *g);
		g->DrawStringDecal(vf2d(0, float(g->ScreenHeight()) - 8), "CHECK CONSOLE");
		part++;
		break;
	case 2:
		*changeStageBoolPtr = true;
		while (true)
		{
			cout << "Input a stage for the portal to lead to:";
			cin >> *changeStageNamePtr;

			if (fs::exists("./assets/stages/" + *changeStageNamePtr))
				break;
		}
		part++;
		break;
	case 3:
		end.Update(0, vi2d(100, -100), *g);
		if (MovePortal(end))
		{
			end.desPos = start.pos;
			start.desPos = end.pos;
			end.destination = start.destination;
			start.destination = stage->name;
			part++;
		}
		break;
	case 4:
		current.open("./assets/stages/" + end.destination + "/" + end.destination + ".scn", std::ios_base::app); // append instead of overwrite
		current << " p ";
		current << start.pos.x << ' ';
		current << start.pos.y << ' ';
		current << start.destination << ' ';
		current << start.desPos.x << ' ';
		current << start.desPos.y << ' ';
		current.close();
		
		second.open("./assets/stages/" + start.destination + "/" + start.destination + ".scn", std::ios_base::app); // append instead of overwrite
		second << " p ";
		second << end.pos.x << ' ';
		second << end.pos.y << ' ';
		second << end.destination << ' ';
		second << end.desPos.x << ' ';
		second << end.desPos.y << ' ';
		second.close();

		part++;
		break;
	case 5:
		*changeStageBoolPtr = true;
		*changeStageNamePtr = stage->name;
		part = 0;
		return true;
	}
	return false;
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
	Portal& p = stage->portals[chosenPortal];

	string posStr = to_string(p.pos.x) + " " + to_string(p.pos.y);
	string desStr = to_string(p.desPos.x) + " " + to_string(p.desPos.y);
	
	string firstFind = "p " + posStr + " " + p.destination + " " + desStr;
	string secondFind = "p " + desStr + " " + stage->name + " " + posStr;
	
	string buffer;

	string first = "./assets/stages/" + stage->name + "/" + stage->name + ".scn";
	if (!fs::exists(first))
		return;
	std::ofstream firstWrite(first + 't', ios::out);
	ifstream firstRead(first);

	string word;
	string tester;
	while (firstRead >> word)
	{
		if(word == "p")
		{
			string x;
			string y;
			string s;
			string desX;
			string desY;
			firstRead >> x;
			firstRead >> y;
			firstRead >> s;
			firstRead >> desX;
			firstRead >> desY;
			tester += "p " + x + " " += y + " " += s + " " += desX + " " += desY;
			if (tester != firstFind)
				firstWrite << " " << tester << " ";
			tester = "";
		}
		else
		{
			firstWrite << word << " ";
		}
	}
	firstWrite.close();
	firstRead.close();
	
	string second = "./assets/stages/" + stage->portals[chosenPortal].destination + "/" + stage->portals[chosenPortal].destination + ".scn";
	if (!fs::exists(second))
		return;
	std::ofstream secondWrite(second + 't', ios::out);
	ifstream secondRead(second);

	while (secondRead >> word)
	{
		if (word == "p")
		{
			string x;
			string y;
			string s;
			string desX;
			string desY;
			secondRead >> x;
			secondRead >> y;
			secondRead >> s;
			secondRead >> desX;
			secondRead >> desY;
			tester += "p " + x + " " += y + " " += s + " " += desX + " " += desY;
			if (tester != secondFind)
				secondWrite << " " << tester << " ";
			tester = "";
		}
		else
		{
			secondWrite << word << " ";
		}
	}

	secondWrite.close();
	secondRead.close();
	
	try
	{
		fs::remove(first);
		fs::rename(first+'t', first);
		fs::remove(second);
		fs::rename(second + 't', second);
	}
	catch (const fs::filesystem_error& e)
	{
		std::cout << e.what() << '\n';
	}

	*changeStageBoolPtr = true;
	*changeStageNamePtr = stage->name;
}