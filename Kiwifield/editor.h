#pragma once
#include <iostream>
#include "olcPixelGameEngine.h"
#include "stage.h"

using namespace olc;

class Editor
{
public:
    Stage* stage;
    PixelGameEngine* g;
    int chosenSprite = 0;
    int mode = 0;

    bool* changeStageBoolPtr;
    string* changeStageNamePtr;
public:
    Editor(Stage& s, PixelGameEngine& g);
    
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
    void EditPortal();
    bool CreatePortal();
    bool MovePortal(Portal& p);
    void RemovePortal();
};

void FillCircleInSpr(int x, int y, int radius, Pixel p, Sprite& sprite);
void DrawLineIn2DBoolean(int& x1, int& y1, int& x2, int& y2, std::vector<std::vector<bool>>& collisionArray, uint32_t pattern = 0xFFFFFFFF);
void SaveSprite(Sprite* spr, std::string filepath);