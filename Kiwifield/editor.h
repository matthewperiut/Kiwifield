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
    
    void manager();
    void editCollision();
    void chooseSprite();
    void editSprite();
};
