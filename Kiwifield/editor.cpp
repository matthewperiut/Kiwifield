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
    g->DrawStringDecal(vi2d(0,0),"EditCollision");
    stage->drawCollider();

    vi2d cam = { g->cam.getX(), g->cam.getY() };
    vi2d invcam = -cam;

    static vi2d start = { 0, 0 };
    static vi2d end = { 0, 0 };

    if (g->GetMouse(0).bHeld)
    {
        if (g->GetMouse(0).bPressed)
            start = g->GetMousePos()+invcam;
        else
            g->DrawLine(start-invcam, g->GetMousePos(), olc::ORANGE);
    }
    else if (g->GetMouse(0).bReleased)
    {
        end = g->GetMousePos()+invcam;
        DrawLineIn2DBoolean(start.x, start.y, end.x, end.y, stage->collision);
    }

    if (g->GetMouse(1).bHeld)
    {
        vi2d m = g->GetMousePos()+invcam;
        if (g->GetMouse(1).bHeld)
        {
            int size = 5;
            for (int y = 0; y < size; y++)
                for (int x = 0; x < size; x++)
                {
                    int colx = m.x + x - int(size/2);
                    int coly = m.y + y - int(size/2);
                    if (colx > -1 && colx < stage->getWidth() && coly > -1 && coly < stage->getHeight())
                        if(stage->collision[colx][coly])
                            stage->collision[colx][coly] = false;
                }
        }
    }
}
void Editor::chooseSprite()
{
    g->DrawStringDecal(vi2d(0,0),"ChooseSprite");
    g->DrawStringDecal(vi2d(0,10),"Click on the Sprite");
    
}
void Editor::editSprite()
{
    g->DrawStringDecal(vi2d(0, 0),"EditSprite");
    g->DrawStringDecal(vi2d(0, 8),"X-delete");
    g->DrawStringDecal(vi2d(0,16), "E-edit");
    g->DrawStringDecal(vi2d(0,24), "R-rename");
    g->DrawStringDecal(vi2d(0,32), "M - move");
}
