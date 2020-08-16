#include "collisionfiles.h"
#include <iostream>

void CollisionFiles::save(bool(&collisionArray)[wWidth][wHeight], string filepath)
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

void CollisionFiles::load(bool(&collisionArray)[wWidth][wHeight], string filepath)
{
	ifstream input_file(filepath);
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
}