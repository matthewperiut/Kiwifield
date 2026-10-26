#include "editor.h"

#include <algorithm>
#include <fstream>
#include <sstream>

namespace
{
	string ScnPath(const string& stageName)
	{
		return "./assets/stages/" + stageName + "/" + stageName + ".scn";
	}

	// Appends a portal line to a stage's .scn. Save writes portals in the same format.
	void AppendPortalLine(const string& stageName, const Portal& p)
	{
		ofstream file(ScnPath(stageName), ios::app);
		file << " p " << p.pos.x << ' ' << p.pos.y << ' ' << p.destination << ' ' << p.desPos.x << ' ' << p.desPos.y << ' ';
	}

	// Rewrites a .scn without the portal line equal to `match`.
	void RemovePortalLine(const string& path, const string& match)
	{
		ifstream in(path);
		ostringstream out;
		string word;
		while (in >> word)
		{
			if (word == "p")
			{
				string x, y, dest, destX, destY;
				in >> x >> y >> dest >> destX >> destY;
				string line = "p " + x + " " + y + " " + dest + " " + destX + " " + destY;
				if (line != match)
					out << line << ' ';
			}
			else
			{
				out << word << ' ';
			}
		}
		in.close();

		ofstream file(path, ios::trunc);
		file << out.str();
	}
}

void Editor::EditPortal()
{
	g->DrawStringDecal(vi2d(0, 0), "EditPortal");
	g->DrawStringDecal(vi2d(0, 8), "Create C");

	if (!stage->portals.empty())
	{
		g->DrawStringDecal(vi2d(0, 16), "Delete X");
		g->DrawStringDecal(vi2d(0, 24), "Cycle >");
	}

	if (g->GetKey(Key::C).bPressed)
	{
		creatingPortal = true;
		portalPart = 0;
	}

	if (!stage->portals.empty())
	{
		chosenPortal = std::clamp(chosenPortal, 0, int(stage->portals.size()) - 1);

		if (g->GetKey(Key::X).bPressed)
		{
			RemovePortal();
		}
		if (!stage->portals.empty())
		{
			if (g->GetKey(Key::RIGHT).bPressed)
			{
				chosenPortal = (chosenPortal + 1) % int(stage->portals.size());
			}
			stage->portals[chosenPortal].DebugDraw(*g);
		}
	}
	else
	{
		chosenPortal = 0;
	}

	if (creatingPortal && CreatePortal())
	{
		creatingPortal = false;
	}
}

// Walks through the steps of placing a linked pair of portals. Returns true when finished or cancelled.
bool Editor::CreatePortal()
{
	if (g->GetKey(Key::ESCAPE).bPressed)
	{
		CancelPortal();
		return true;
	}

	switch (portalPart)
	{
	case 0:
		// Place the portal in this stage
		portalStart.Update(0, vi2d(100, 100), *g);
		if (MovePortal(portalStart))
		{
			portalStart.destination = stage->name;
			portalStart.desPos = vi2d(10, 10);
			portalPart++;
		}
		break;
	case 1:
		portalStart.Update(0, vi2d(100, -100), *g);
		portalPart++;
		break;
	case 2:
		// Stays on this step until a valid stage name is typed (callback advances the part) or cancelled
		AskPortalStage();
		break;
	case 3:
		// Place the return portal in the destination stage
		portalEnd.Update(0, vi2d(100, -100), *g);
		if (MovePortal(portalEnd))
		{
			portalEnd.desPos = portalStart.pos;
			portalStart.desPos = portalEnd.pos;
			portalEnd.destination = portalStart.destination;
			portalStart.destination = stage->name;
			portalPart++;
		}
		break;
	case 4:
		AppendPortalLine(portalEnd.destination, portalStart);
		AppendPortalLine(portalStart.destination, portalEnd);
		portalPart++;
		break;
	case 5:
		// Destination stage is the current one; add the return portal in memory instead of reloading
		stage->portals.push_back(portalEnd);
		portalPart = 0;
		return true;
	}
	return false;
}

void Editor::AskPortalStage()
{
	prompt.AskText("Stage to lead to:", [this](const string& name)
	{
		if (name.empty() || !fs::exists("./assets/stages/" + name))
		{
			AskPortalStage();
			return;
		}
		// Switching stages reloads from disk, so write this stage's edits first
		stage->Save();
		*changeStageNamePtr = name;
		*changeStageBoolPtr = true;
		portalPart = 3;
	}, [this]()
	{
		CancelPortal();
	});
}

void Editor::CancelPortal()
{
	portalPart = 0;
	creatingPortal = false;
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
	if (chosenPortal < 0 || chosenPortal >= int(stage->portals.size()))
		return;

	const Portal p = stage->portals[chosenPortal];
	const string posStr = to_string(p.pos.x) + " " + to_string(p.pos.y);
	const string desStr = to_string(p.desPos.x) + " " + to_string(p.desPos.y);

	const string here = ScnPath(stage->name);
	const string there = ScnPath(p.destination);
	if (!fs::exists(here) || !fs::exists(there))
		return;

	RemovePortalLine(here, "p " + posStr + " " + p.destination + " " + desStr);
	RemovePortalLine(there, "p " + desStr + " " + stage->name + " " + posStr);

	stage->portals.erase(stage->portals.begin() + chosenPortal);
	chosenPortal = 0;
}
