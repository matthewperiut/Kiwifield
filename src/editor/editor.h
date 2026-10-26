#pragma once
#include <iostream>
#include "../lib/olcPixelGameEngine.h"
#include "../world/stage.h"
#include "../utility/textprompt.h"

using namespace olc;

class Editor
{
public:
    Stage* stage;
    PixelGameEngine* g;
    TextPrompt& prompt;
    int chosenSprite = 0;
    int mode = 0;

    // Set by World: the portal tool asks World to switch stages through these
    bool* changeStageBoolPtr = nullptr;
    string* changeStageNamePtr = nullptr;
public:
    Editor(Stage& s, PixelGameEngine& engine, TextPrompt& textPrompt);

    // Called when World loads a different stage. Editor state that belongs to the old stage is reset.
    void SetStage(Stage& s);

    void Manager();
    void EditCollision();

    // Sprites
    void ChooseSprite();
    void DrawSelectedSprite();
    void EditSprite();
    void SaveAllSprites();
    void CreateSprite();
    void RemoveSprite();
    void ModifySprite();
    void RenameSprite();
    bool MoveSprite();
    void DuplicateSprite();
    void LayerSprite(bool up);

    // Portal
    int chosenPortal = 0;
    bool creatingPortal = false;
    int portalPart = 0;
    Portal portalStart{ vi2d(10, 10) };
    Portal portalEnd{ vi2d(10, 10) };

    void EditPortal();
    bool CreatePortal();
    void AskPortalStage();
    void CancelPortal();
    bool MovePortal(Portal& p);
    void RemovePortal();
};

void FillCircleInSpr(int x, int y, int radius, Pixel p, Sprite& sprite);
void DrawLineIn2DBoolean(int& x1, int& y1, int& x2, int& y2, std::vector<std::vector<bool>>& collisionArray, uint32_t pattern = 0xFFFFFFFF);
void SaveSprite(Sprite* spr, std::string filepath);
