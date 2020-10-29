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
public:
    Editor(Stage& s, PixelGameEngine& g);
    
    void Manager();
    void EditCollision();

    // Sprites
    void ChooseSprite();
    void DrawSelectedSprite();
    void EditSprite();
    void DrawSprite();
    void SaveAllSprites();
    void CreateSprite();
    void RemoveSprite();
    void RenameSprite();
    bool MoveSprite();
    void DuplicateSprite();
    void LayerSprite(bool up);

    // Portal
    void EditPortal();
};

void FillCircleInSpr(int x, int y, int radius, Pixel p, Sprite& sprite);
void DrawLineIn2DBoolean(int& x1, int& y1, int& x2, int& y2, std::vector<std::vector<bool>>& collisionArray, uint32_t pattern = 0xFFFFFFFF);
void SaveSprite(Sprite* spr, std::string filepath);