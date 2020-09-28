#pragma once
#include "const.h"
#include <ostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

struct Files
{
	static void save(vector<vector<bool>>& collision, string filepath);
	static bool load(vector<vector<bool>>& collision, string filepath);
};