#pragma once
#include "const.h"
#include <ostream>
#include <fstream>
#include <string>

using namespace std;

struct CollisionFiles
{
	static void save(bool(&collisionArray)[wWidth][wHeight], string filepath);
	static void load(bool(&collisionArray)[wWidth][wHeight], string filepath);
};