#include "files.h"
#include <vector>
#include <iostream>
/*
void Files::save(bool(&collisionArray)[wWidth][wHeight], string filepath)
{
	ofstream file;
	file.open(filepath);
	for (int y = 0; y < wHeight; y++)
	{
		for (int x = 0; x < wWidth; x++)
		{
			file << collisionArray[x][y];
		}
		file << "e";
#ifdef _DEBUG
		file << "\n";
#endif
	}
	file.close();
}

bool Files::load(bool(&collisionArray)[wWidth][wHeight], string filepath)
{
	ifstream input_file(filepath);
	if (input_file.fail()) {
		return false;
	}
	char val;
	int x = 0;
	int y = 0;
	while (input_file >> val)
	{
		if (val == '0')
		{
			collisionArray[x][y] = false;
			x++;
		}
		else if (val == '1')
		{
			collisionArray[x][y] = true;
			x++;
		}
		else if (val == 'e')
		{
			x = 0;
			y++;
		}
	}
	return true;
}

bool Files::exists(string filepath)
{
	ifstream fileStream;
	if (fileStream.fail())
		return 0;
	else
		return 1;
}
*/

void Files::save(vector<vector<bool>>& collision, string filepath)
{
	ofstream file;
	file.open(filepath);

	int width = collision.size();
	std::cout << width << std::endl;
	int height = collision[0].size();

	for (int a = 0; a < width; a++)
	{
		for (int b = 0; b < height; b++)
		{
			file << collision[a][b];
		}
		file << "e\n";
	}
	file.close();
}

bool Files::load(vector<vector<bool>>& collision, string filepath)
{
	ifstream input_file(filepath);
	if (input_file.fail()) {
		return false;
	}
	char val;
	int x = 0;
	int y = 0;
	collision.push_back({});
	while (input_file >> val)
	{
		if (val == '0')
		{
			collision[y].push_back(false);
			x++;
		}
		else if (val == '1')
		{
			collision[y].push_back(true);
			x++;
		}
		else if (val == 'e')
		{
			x = 0;
			y++;
			collision.push_back({});
		}
	}
	return true;
}
