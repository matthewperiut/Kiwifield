#include "editor.h"
#include <iostream>
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
        if(mode > 2)
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
        if (start.x > -1 && start.x < stage->getWidth() && start.y > -1 && start.y < stage->getHeight())
            if (end.x > -1 && end.x < stage->getWidth() && end.y > -1 && end.y < stage->getHeight())
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
                    int colx = m.x + x;// - int(size/2);
                    int coly = m.y + y;// - int(size/2);
                    if (colx > -1 && colx < stage->getWidth() && coly > -1 && coly < stage->getHeight())
                        if(stage->collision[colx][coly])
                            stage->collision[colx][coly] = false;
                }
        }
    }
}
void Editor::chooseSprite()
{
    vi2d cam = { g->cam.getX(), g->cam.getY() };
    
    if (g->GetMouse(0).bPressed)
    {
        int loops = 0;
        for (int i = chosenSprite; i < stage->images.size(); i++)
        {
            vi2d pos = stage->images[i].position + cam;
            vi2d size = vi2d(stage->images[i].sprite->width, stage->images[i].sprite->height);
            vi2d corner = pos + size;

            if (g->GetMousePos().x > pos.x && g->GetMousePos().y > pos.y && !(chosenSprite == i))
            {
                if (g->GetMousePos().x < corner.x && g->GetMousePos().y < corner.y)
                {
                   
                    chosenSprite = i;
                    break;
                }
            }
            if (i == stage->images.size() - 1)
            {
                i = -1;
                loops++;
                if (loops == 2)
                {
                    loops = 0;
                    break;
                }
            }
        }
    }
}

void Editor::drawSelected()
{
    if (stage->images.size() > 0)
    {
        vi2d pos = stage->images[chosenSprite].position + vi2d(g->cam.getX(), g->cam.getY());
        vi2d size = vi2d(stage->images[chosenSprite].sprite->width, stage->images[chosenSprite].sprite->height);
        g->SetPixelMode(olc::Pixel::ALPHA);
        g->DrawRect(pos-vi2d(1,1), size+vi2d(1, 1), Pixel(255,255,255,128));
        g->SetPixelMode(olc::Pixel::NORMAL);
    }
}

void Editor::editSprite()
{
    drawSelected();

    const static int size = 7;
    static int prevmode = 0;
    static int mode = 0;
    enum {ESC,C,X,E,R,M};

    if (mode != M && mode != E)
    {
        chooseSprite();
    }

    string instructions[size] =
    { "EditSprite", "ESC-stop", "C-create", "X-delete", "E-edit", "R-rename", "M-move" };

    for (int i = 0; i < size; i++)
    {
        g->DrawStringDecal(vi2d(0, i * 8), instructions[i]);
    }

    static bool usingConsole = false;

    if(g->GetKey(olc::ESCAPE).bPressed)
        mode = ESC;
    else if (g->GetKey(olc::C).bPressed)
    {
        mode = C;
        usingConsole = true;
    }
    else if (g->GetKey(olc::X).bPressed)
        mode = X;
    else if (g->GetKey(olc::E).bPressed)
        mode = E;
    else if (g->GetKey(olc::R).bPressed)
    {
        mode = R;
        usingConsole = true;
    } 
    else if (g->GetKey(olc::M).bPressed)
        mode = M;

    // For mode A
    
    if (usingConsole)
    {
        usingConsole = false;
        g->DrawString(vf2d(0, int(g->ScreenHeight()-8)), "CHECK CONSOLE");
    }
    else
        switch (mode)
        {
        case ESC:
            // Nothing
            break;
        case C:
            Create();
            mode = ESC;
            break;
        case X:
            Remove();
            mode = ESC;
            break;
        case E:
            Edit();
            break;
        case R:
            Rename();
            mode = ESC;
            break;
        case M:
            if (Move())
                mode = ESC;
            break;
        }
    if (mode != E && prevmode == E)
    {
        saveSprite(stage->images[chosenSprite].sprite, stage->images[chosenSprite].filepath);
        for (int i = 0; i < stage->images.size(); i++)
        {
            if (i == chosenSprite)
                continue;
            if (stage->images[i].filepath == stage->images[chosenSprite].filepath)
            {
                stage->images[i] = Image(stage->images[chosenSprite].filepath, stage->images[i].position);
            }
        }
    }

    prevmode = mode;
}

void Editor::SaveAllSprites()
{
    for (int i = 0; i < stage->images.size(); i++)
    {
        saveSprite(stage->images[i].sprite, stage->images[i].filepath);
    }
}

void Editor::Create()
{
    string sprName = "";
    vi2d sprSize = { 0, 0 };
    vi2d pos = { 50, 20 };
    std::cout << "Input name:" << std::endl;
    std::cin >> sprName;

    // Check if exists
    string filepath = "./assets/" + sprName + ".png";
    if (std::filesystem::exists(filepath))
    {
        stage->images.push_back(Image(filepath, vi2d(g->ScreenWidth(),g->ScreenHeight())/2));
        return;
    }

    std::cout << "Input width:" << std::endl;
    std::cin >> sprSize.x;
    std::cout << "Input height:" << std::endl;
    std::cin >> sprSize.y;

    Image push = Image(sprSize, pos);
    for (int x = 0; x < sprSize.x; x++)
    {
        for (int y = 0; y < sprSize.y; y++)
        {
            push.sprite->SetPixel(x, y, olc::BLANK);
        }
    }
    push.filepath = filepath;
    push.sprite->SetPixel(sprSize.x/2, sprSize.y/2, olc::RED);
    push.update();
    
    chosenSprite = stage->images.size();
    stage->images.push_back(push);
    
    saveSprite(stage->images[chosenSprite].sprite, stage->images[chosenSprite].filepath);
}
void Editor::Remove()
{
    if (stage->images.size() > 0 && chosenSprite <= stage->images.size() - 1)
    {
        stage->images.erase(stage->images.begin() + (chosenSprite));
        chosenSprite = 0;
    }
}
void Editor::Edit()
{
    const static int colorpickerSize = 12;
    const static Image eraserimg((string)"./assets/util/eraser.png", vi2d(g->ScreenWidth() - 16, g->ScreenHeight() - colorpickerSize - 16));
    static bool eraser = false;

    vi2d m = g->GetMousePos();
    vi2d c = vi2d(g->cam.getX(), g->cam.getY());
    vi2d ic = -c;
    vi2d pos = stage->images[chosenSprite].position;
    vi2d size = { stage->images[chosenSprite].sprite->width, stage->images[chosenSprite].sprite->height };

    // Change brush size
    static int radius = 0;
    if (g->GetMouseWheel() > 0)
        radius++;
    if (g->GetMouseWheel() < 0)
        radius--;

    // Draw brush
    g->DrawCircle(m, radius);

    // Color Picker
    static Pixel color = olc::GREY;
    for (int i = 0; i < colorpickerSize + 1; i+=colorpickerSize / 3)
    {
        vi2d topleft = { 0, g->ScreenHeight()-i };

        for (int x = 0; x < g->ScreenWidth(); x++)
        {
            int variance = x;
            Pixel show = olc::BLACK;
            switch (i)
            {
            case colorpickerSize / 3:
                show.r = variance;
                if (show.r == color.r && !eraser)
                    show = olc::WHITE;
                break;
            case colorpickerSize / 3 * 2:
                show.g = variance;
                if (show.g == color.g && !eraser)
                    show = olc::WHITE;
                break;
            case colorpickerSize / 3 * 3:
                show.b = variance;
                if (show.b == color.b && !eraser)
                    show = olc::WHITE;
                break;
            }

            g->DrawRect(vi2d(x, topleft.y), vi2d(1, colorpickerSize / 3 - 1), show);
        }
    }

    // Eyedropper
    if (g->GetMouse(1).bPressed)
        if (m.x + ic.x > pos.x - 1 && m.y + ic.y > pos.y - 1 && m.x + ic.x < pos.x + size.x && m.y + ic.y < pos.y + size.y)
        {
            color = stage->images[chosenSprite].sprite->GetPixel(m.x - pos.x, m.y - pos.y);
        }

    // Draw color
    const static int colorShowSize = 16;
    g->FillRect(vi2d(0, g->ScreenHeight() - colorShowSize - colorpickerSize), vi2d(colorShowSize, colorShowSize), color);

    // Draw eraser
    g->DrawDecal(eraserimg.position,eraserimg.decal);

    enum {none, red, green, blue};
    static int colorpickermode = 0;
    if (g->GetMouse(0).bPressed)
    {
        if(m.x > eraserimg.position.x && m.y > eraserimg.position.y && m.x < eraserimg.position.x + eraserimg.sprite->width && m.y < eraserimg.position.y + eraserimg.sprite->height)
            eraser = !eraser;

        if (m.y > g->ScreenHeight() - (colorpickerSize + 1))
        {
            eraser = false;

            int slidersize = colorpickerSize / 3;
            int h = g->ScreenHeight();
            if (m.y > h - slidersize - 1)
            {
                colorpickermode = red;
            }
            else if (m.y > h - slidersize * 2 - 1)
            {
                colorpickermode = green;
            }
            else if (m.y > h - slidersize * 3 - 1 )
            {
                colorpickermode = blue;
            }
        }
    }
    if (eraser)
    {
        g->DrawRect(eraserimg.position, vi2d(eraserimg.sprite->width-1, eraserimg.sprite->height-1));
    }

    if (colorpickermode != 0)
    {
        switch (colorpickermode)
        {
        case red:
            color.r = m.x;
            break;
        case green:
            color.g = m.x;
            break;
        case blue:
            color.b = m.x;
            break;
        }
    }
    
    if (g->GetMouse(0).bReleased)
        if (colorpickermode != 0)
        {
            colorpickermode = 0;
        }
    
    // Drawing
    if (g->GetMouse(0).bHeld)
    {
        if (eraser)
            FillCircleInSpr(m.x - pos.x + ic.x, m.y - pos.y + ic.y, radius, olc::BLANK, *(stage->images[chosenSprite].sprite));
        else
            FillCircleInSpr(m.x - pos.x + ic.x, m.y - pos.y + ic.y, radius, color, *(stage->images[chosenSprite].sprite));

        stage->images[chosenSprite].update();
    }
        
}
void Editor::Rename()
{
    if (stage->images.size() > 0 && chosenSprite <= stage->images.size() - 1)
    {
        string sprName = "";
        std::cout << "This will make the sprite into a new file" << std::endl;
        std::cout << "Previous path: " << stage->images[chosenSprite].filepath << std::endl;
        std::cout << "make sure to exclude the ./assets/ and .png, only need the name" << std::endl;
        std::cout << "Input new name:" << std::endl;
        std::cin >> sprName;

        stage->images[chosenSprite].filepath = "./assets/" + sprName + ".png";
        saveSprite(stage->images[chosenSprite].sprite, stage->images[chosenSprite].filepath);
    }
}
bool Editor::Move()
{
    vi2d sprSize = { stage->images[chosenSprite].sprite->width, stage->images[chosenSprite].sprite->height };
    stage->images[chosenSprite].position = g->GetMousePos() - sprSize / 2 - vi2d(g->cam.getX(), g->cam.getY());

    if (g->GetMouse(0).bPressed)
    {
        return true;
    }

    return false;
}