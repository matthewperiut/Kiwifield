#include "editor.h"

void Editor::editCollision()
{
    g->DrawStringDecal(vi2d(0, 0), "EditCollision");
    stage->drawCollider();

    vi2d cam = { g->cam.getX(), g->cam.getY() };
    vi2d invcam = -cam;

    static vi2d start = { 0, 0 };
    static vi2d end = { 0, 0 };

    if (g->GetMouse(0).bHeld)
    {
        if (g->GetMouse(0).bPressed)
            start = g->GetMousePos() + invcam;
        else
            g->DrawLine(start - invcam, g->GetMousePos(), olc::ORANGE);
    }
    else if (g->GetMouse(0).bReleased)
    {
        end = g->GetMousePos() + invcam;
        if (start.x > -1 && start.x < stage->getWidth() && start.y > -1 && start.y < stage->getHeight())
            if (end.x > -1 && end.x < stage->getWidth() && end.y > -1 && end.y < stage->getHeight())
                DrawLineIn2DBoolean(start.x, start.y, end.x, end.y, stage->collision);
    }

    if (g->GetMouse(1).bHeld)
    {
        vi2d m = g->GetMousePos() + invcam;
        int size = 5;
        for (int y = 0; y < size; y++)
            for (int x = 0; x < size; x++)
            {
                int colx = m.x + x;// - int(size/2);
                int coly = m.y + y;// - int(size/2);
                if (colx > -1 && colx < stage->getWidth() && coly > -1 && coly < stage->getHeight())
                    if (stage->collision[colx][coly])
                        stage->collision[colx][coly] = false;
            }
    }
}