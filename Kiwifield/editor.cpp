#include "editor.h"

Editor::Editor(Stage& s, PixelGameEngine& g)
{
    this->stage = &s;
    this->g = &g;
}

void Editor::manager()
{
    static int mode = 1;
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
            chooseSprite();
            break;
        case 3:
            //Choose Sprite mode
            editSprite();
            break;
    }
}

void Editor::editCollision()
{
    g->DrawString(vi2d(20,20),"EditCollision");
    if(g->GetMouse(0).bHeld)
    {
        vi2d mod = g->GetMousePos() + vi2d(-g->cam.getX(),-g->cam.getY());
        stage->collision[mod.x][mod.y] = true;
    }
}
void Editor::chooseSprite()
{
    g->DrawString(vi2d(0,0),"ChooseSprite");
    g->DrawString(vi2d(0,10),"Click on the Sprite");
    
}
void Editor::editSprite()
{
    g->DrawString(vi2d(0,0),"EditSprite");
    g->DrawString(vi2d(0,10),"X to remove, M to move");
}
