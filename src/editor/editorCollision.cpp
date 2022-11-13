#include "editor.h"

void Editor::EditCollision()
{
    g->DrawStringDecal(vi2d(0, 0), "EditCollision");
    stage->DrawCollider();

    vi2d cam = { g->cam.GetX(), g->cam.GetY() };
    vi2d invcam = -cam;

    static vi2d start = { 0, 0 };
    static vi2d end = { 0, 0 };

    if (g->GetMouse(0).bHeld)
    {
        if (g->GetMouse(0).bPressed)
            start = g->GetMousePos() + invcam;
        else
            g->DrawLine(start - invcam, g->GetMousePos(), Pixel(255, 128, 0));
    }
    else if (g->GetMouse(0).bReleased)
    {
        end = g->GetMousePos() + invcam;
        if (start.x > -1 && start.x < stage->GetWidth() && start.y > -1 && start.y < stage->GetHeight())
            if (end.x > -1 && end.x < stage->GetWidth() && end.y > -1 && end.y < stage->GetHeight())
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
                if (colx > -1 && colx < stage->GetWidth() && coly > -1 && coly < stage->GetHeight())
                    if (stage->collision[colx][coly])
                        stage->collision[colx][coly] = false;
            }
    }
}