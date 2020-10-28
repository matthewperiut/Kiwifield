#include "editor.h"

void Editor::chooseSprite()
{
    vi2d cam = { g->cam.getX(), g->cam.getY() };

    if (g->GetMouse(0).bPressed)
    {
        int loops = 0;
        for (int i = chosenSprite; i < stage->imgs.size(); i++)
        {
            vi2d pos = stage->imgs[i].position + cam;
            vi2d size = vi2d(stage->imgs[i].GetSprPtr()->width, stage->imgs[i].GetSprPtr()->height);
            vi2d corner = pos + size;

            if (g->GetMousePos().x > pos.x && g->GetMousePos().y > pos.y && !(chosenSprite == i))
            {
                if (g->GetMousePos().x < corner.x && g->GetMousePos().y < corner.y)
                {

                    chosenSprite = i;
                    break;
                }
            }
            if (i == stage->imgs.size() - 1)
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

void Editor::drawSelectedSprite()
{
    if (stage->imgs.size() > 0)
    {
        vi2d pos = stage->imgs[chosenSprite].position + vi2d(g->cam.getX(), g->cam.getY());
        vi2d size = vi2d(stage->imgs[chosenSprite].GetSprPtr()->width, stage->imgs[chosenSprite].GetSprPtr()->height);
        g->SetPixelMode(olc::Pixel::ALPHA);
        g->DrawRect(pos - vi2d(1, 1), size + vi2d(1, 1), Pixel(255, 255, 255, 128));
        g->SetPixelMode(olc::Pixel::NORMAL);
        g->DrawStringDecal(pos + vf2d(0,-8), to_string(chosenSprite));
    }
}

void Editor::editSprite()
{
    drawSelectedSprite();

    const static int size = 9;
    static int prevmode = 0;
    static int mode = 0;
    enum { ESC, C, X, E, R, M, Left, Right, Q };

    if (mode != M && mode != E)
    {
        chooseSprite();
    }

    string instructions[size] =
    { "EditSprite", "ESC-stop", "C-create", "X-delete", "E-edit", "R-rename", "M-move", "<>-layer", "Q-Duplicate" };

    for (int i = 0; i < size; i++)
    {
        g->DrawStringDecal(vi2d(0, i * 8), instructions[i]);
    }

    static bool usingConsole = false;

    if (g->GetKey(olc::ESCAPE).bPressed)
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
    else if (g->GetKey(olc::LEFT).bPressed)
        mode = Left;
    else if (g->GetKey(olc::RIGHT).bPressed)
        mode = Right;
    else if (g->GetKey(olc::Q).bPressed)
        mode = Q;

    // For mode A

    if (usingConsole)
    {
        usingConsole = false;
        g->DrawStringDecal(vf2d(0, float(g->ScreenHeight()) - 8), "CHECK CONSOLE");
    }
    else
        switch (mode)
        {
        case ESC:
            // Nothing
            break;
        case C:
            CreateSprite();
            mode = ESC;
            break;
        case X:
            RemoveSprite();
            mode = ESC;
            break;
        case E:
            EditSprite();
            break;
        case R:
            RenameSprite();
            mode = ESC;
            break;
        case M:
            if (MoveSprite())
                mode = ESC;
            break;
        case Left:
            LayerSprite(0);
            mode = ESC;
            break;
        case Right:
            LayerSprite(1);
            mode = ESC;
            break;
        case Q:
            DuplicateSprite();
            mode = M;
            break;
        }
    if (mode != E && prevmode == E)
    {
        saveSprite(stage->imgs[chosenSprite].GetSprPtr(), stage->imgs[chosenSprite].GetFilePath());
        for (int i = 0; i < stage->imgs.size(); i++)
        {
            if (i == chosenSprite)
                continue;
            if (stage->imgs[i].GetFilePath() == stage->imgs[chosenSprite].GetFilePath())
            {
                vi2d pos = stage->imgs[i].position;
                stage->imgs[i].SetFilePath(stage->imgs[chosenSprite].GetFilePath());
                stage->imgs[i].position = pos;
            }
        }
    }

    prevmode = mode;
}

void Editor::LayerSprite(bool up)
{
    if (!(chosenSprite < stage->imgs.size() && chosenSprite > -1))
        return;

    string currentPath = stage->imgs[chosenSprite].GetFilePath();
    vi2d currentPos = stage->imgs[chosenSprite].position;

    string flipperPath;
    vi2d flipperPos;

    if (up)
    {
        if (double(chosenSprite) + 1 < stage->imgs.size())
        {
            flipperPath = stage->imgs[double(chosenSprite) + 1].GetFilePath();
            flipperPos = stage->imgs[double(chosenSprite) + 1].position;

            stage->imgs[chosenSprite].SetFilePath(flipperPath);
            stage->imgs[chosenSprite].position = flipperPos;

            stage->imgs[double(chosenSprite) + 1].SetFilePath(currentPath);
            stage->imgs[double(chosenSprite) + 1].position = currentPos;

            chosenSprite++;
        }
    }
    else
    {
        if (double(chosenSprite) - 1 > -1)
        {
            flipperPath = stage->imgs[double(chosenSprite) - 1].GetFilePath();
            flipperPos = stage->imgs[double(chosenSprite) - 1].position;

            stage->imgs[chosenSprite].SetFilePath(flipperPath);
            stage->imgs[chosenSprite].position = flipperPos;

            stage->imgs[double(chosenSprite) - 1].SetFilePath(currentPath);
            stage->imgs[double(chosenSprite) - 1].position = currentPos;

            chosenSprite--;
        }
    }
    std::cout << chosenSprite << '\n';
}


void Editor::SaveAllSprites()
{
    // Untested
    for (int i = 0; i < stage->imgs.size(); i++)
    {
        saveSprite(stage->imgs[i].GetSprPtr(), stage->imgs[i].GetFilePath());
    }
}

void Editor::CreateSprite()
{
    string sprName = "default";
    vi2d sprSize = { 8, 8 };
    vi2d pos = vi2d(g->ScreenWidth(), g->ScreenHeight()) / 2;
    
    cout << "Input name:" << endl;
    cin >> sprName;

    // Check if exists
    string filePath = "./assets/" + sprName + ".png";
    if (filesystem::exists(filePath))
    {
        stage->imgs.push_back(Img(filePath));
        stage->imgs[stage->imgs.size() - 1].position = pos;
        return;
    }
    else
    {
        cout << "Input width:" << endl;
        cin >> sprSize.x;
        cout << "Input height:" << endl;
        cin >> sprSize.y;
    }
    
    stage->imgs.push_back(Img(sprSize));
    int index = stage->imgs.size() - 1;
    stage->imgs[index].position = pos;
    stage->imgs[index].filePath = filePath;
    for (int x = 0; x < sprSize.x; x++)
    {
        for (int y = 0; y < sprSize.y; y++)
        {
            stage->imgs[index].GetSprPtr()->SetPixel(x, y, olc::BLANK);
        }
    }
    stage->imgs[index].GetDecPtr()->Update();
    chosenSprite = index;
    
    saveSprite(stage->imgs[chosenSprite].GetSprPtr(), stage->imgs[chosenSprite].GetFilePath());
}
void Editor::RemoveSprite()
{
    if (stage->imgs.size() > 0 && chosenSprite <= stage->imgs.size() - 1)
    {
        stage->imgs.erase(stage->imgs.begin() + chosenSprite);
        if (chosenSprite > 1 && chosenSprite < stage->imgs.size())
            chosenSprite--;
        else
            chosenSprite = 0;
    }
}
void Editor::EditSprite()
{
    const static int colorpickerSize = 12;
    static Img eraserimg((string)"./assets/util/eraser.png", { g->ScreenWidth() - 16, g->ScreenHeight() - colorpickerSize - 16 });
    static bool eraser = false;

    vi2d m = g->GetMousePos();
    vi2d c = vi2d(g->cam.getX(), g->cam.getY());
    vi2d ic = -c;
    vi2d &pos = stage->imgs[chosenSprite].position;
    vi2d size = { stage->imgs[chosenSprite].GetSprPtr()->width, stage->imgs[chosenSprite].GetSprPtr()->height };

    // Change brush size
    static int radius = 0;
    if (g->GetMouseWheel() > 0 || g->GetKey(Key::UP).bPressed)
        radius++;
    if (g->GetMouseWheel() < 0 || g->GetKey(Key::DOWN).bPressed)
        radius--;

    // Draw brush
    g->DrawCircle(m, radius);

    // Color Picker
    static Pixel color = olc::GREY;
    for (int i = 0; i < colorpickerSize + 1; i += colorpickerSize / 3)
    {
        vi2d topleft = { 0, g->ScreenHeight() - i };

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
            Pixel buffer = stage->imgs[chosenSprite].GetSprPtr()->GetPixel(m.x - pos.x + ic.x, m.y - pos.y + ic.y);
            if (buffer.a == WHITE.a)
                color = buffer;
        }

    // Draw color
    const static int colorShowSize = 16;
    g->FillRect(vi2d(0, g->ScreenHeight() - colorShowSize - colorpickerSize), vi2d(colorShowSize, colorShowSize), color);

    // Draw eraser
    g->DrawDecal(eraserimg.position, eraserimg.GetDecPtr());

    enum { none, red, green, blue };
    static int colorpickermode = 0;
    if (g->GetMouse(0).bPressed)
    {
        if (m.x > eraserimg.position.x && m.y > eraserimg.position.y && m.x < eraserimg.position.x + eraserimg.GetSprPtr()->width && m.y < eraserimg.position.y + eraserimg.GetSprPtr()->height)
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
            else if (m.y > h - slidersize * 3 - 1)
            {
                colorpickermode = blue;
            }
        }
    }
    if (eraser)
    {
        g->DrawRect(eraserimg.position, vi2d(eraserimg.GetSprPtr()->width - 1, eraserimg.GetSprPtr()->height - 1));
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
            FillCircleInSpr(m.x - pos.x + ic.x, m.y - pos.y + ic.y, radius, olc::BLANK, *(stage->imgs[chosenSprite].GetSprPtr()));
        else
            FillCircleInSpr(m.x - pos.x + ic.x, m.y - pos.y + ic.y, radius, color, *(stage->imgs[chosenSprite].GetSprPtr()));

        stage->imgs[chosenSprite].GetDecPtr()->Update();
    }

}
void Editor::RenameSprite()
{
    if (stage->imgs.size() > 0 && chosenSprite <= stage->imgs.size() - 1)
    {
        string sprName = "";
        std::cout << "This will make the sprite into a new file" << std::endl;
        std::cout << "Previous path: " << stage->imgs[chosenSprite].filePath << std::endl;
        std::cout << "make sure to exclude the ./assets/ and .png, only need the name" << std::endl;
        std::cout << "Input new name:" << std::endl;
        std::cin >> sprName;

        stage->imgs[chosenSprite].filePath = "./assets/" + sprName + ".png";
        saveSprite(stage->imgs[chosenSprite].GetSprPtr(), stage->imgs[chosenSprite].GetFilePath());
    }
}
bool Editor::MoveSprite()
{
    vi2d sprSize = { stage->imgs[chosenSprite].GetSprPtr()->width, stage->imgs[chosenSprite].GetSprPtr()->height };
    stage->imgs[chosenSprite].position = g->GetMousePos() - sprSize / 2 - vi2d(g->cam.getX(), g->cam.getY());

    if (g->GetMouse(0).bPressed)
    {
        return true;
    }

    return false;
}

void Editor::DuplicateSprite()
{
    if (stage->imgs.size() > 0 && chosenSprite <= stage->imgs.size() - 1)
    {
        stage->imgs.push_back(Img());
        for (double i = stage->imgs.size() - 1; i > chosenSprite; i--)
        {
            vi2d pos = stage->imgs[i - 1].position;
            stage->imgs[i].SetFilePath(stage->imgs[i - 1].filePath);
            stage->imgs[i].position = pos;
        }
        int index = chosenSprite + 1;
        vi2d pos = stage->imgs[index].position;
        stage->imgs[chosenSprite].SetFilePath(stage->imgs[index].filePath);
        stage->imgs[chosenSprite].position = pos;
        LayerSprite(1);
    }
}