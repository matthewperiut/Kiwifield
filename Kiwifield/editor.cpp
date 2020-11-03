#include "editor.h"

Editor::Editor(Stage& s, PixelGameEngine& g)
{
    this->stage = &s;
    this->g = &g;
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