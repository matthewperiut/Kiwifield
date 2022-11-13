#pragma once
#include "../const.h"
#include <ostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

struct Files
{
	static void Save(vector<vector<bool>>& collision, string filepath);
	static bool Load(vector<vector<bool>>& collision, string filepath);
};