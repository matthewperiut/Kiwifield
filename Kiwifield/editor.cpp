#include "editor.h"

Editor::Editor(Stage& s, PixelGameEngine& g)
{
    this->stage = &s;
    this->g = &g;
}

void Editor::manager()
{
    static int mode = 0;
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
            editCollision();
            break;
        case 2:
            //Choose Sprite mode
            editSprite();
            break;
        case 3:
            //Place portal
            editPortal();
    }
}