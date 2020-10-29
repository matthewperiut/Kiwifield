#include "editor.h"

void Editor::EditPortal()
{
	int chosenPortal = 0;
	
	g->DrawStringDecal(vi2d(0, 0), "EditPortal");
	g->DrawStringDecal(vi2d(0, 8), "Create C");
	g->DrawStringDecal(vi2d(0, 16), "Delete X");
	g->DrawStringDecal(vi2d(0, 24), "Cycle >");

	if (stage->portals.empty())
		return;

	if (chosenPortal >= 0 && chosenPortal < stage->portals.size())
	{

	}
	else
	{
		chosenPortal = 0;
	}
}