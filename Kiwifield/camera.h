#pragma once

class Camera
{
public:
	int x = 0;
	int y = 0;
	Camera();
	Camera(int x, int y); 

	int getX();
	int getY();
};