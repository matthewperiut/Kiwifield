#include "editor.h"

Editor::Editor(Stage& s, PixelGameEngine& engine, TextPrompt& textPrompt)
    : stage(&s), g(&engine), prompt(textPrompt)
{
}

void Editor::SetStage(Stage& s)
{
    stage = &s;
    chosenSprite = 0;
    chosenPortal = 0;
}

void Editor::Manager()
{
    if (g->GetKey(Key::TAB).bPressed)
    {
        mode++;
        if(mode > 3)
        {
            mode = 0;
        }
    }

    // Leaving the portal tool drops a half-placed portal, so World isn't left blocked on it
    if (mode != 3)
        CancelPortal();

    switch(mode)
    {
        case 0:
            //Play mode
            break;
        case 1:
            //Edit collision mode
            EditCollision();
            break;
        case 2:
            //Choose Sprite mode
            EditSprite();
            break;
        case 3:
            //Place portal
            EditPortal();
            break;
    }
}
