#pragma once
#include "const.h"
#include <ostream>
#include <fstream>
#include <string>

using namespace std;

struct Files
{
	static void save(bool(&collisionArray)[wWidth][wHeight], string filepath);
	static bool load(bool(&collisionArray)[wWidth][wHeight], string filepath);
	static bool exists(string filepath);
};