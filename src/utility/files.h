#pragma once
#include <string>
#include "../lib/olcPixelGameEngine.h"

// Writes the sprite to a PNG file, then re-compresses it.
void SaveSprite(olc::Sprite* spr, std::string filepath);
