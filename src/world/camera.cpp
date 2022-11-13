#include "camera.h"

Camera::Camera()
= default;

Camera::Camera(int x, int y)
{
	this->x = x;
	this->y = y;
}
int Camera::GetX()
{
	return -x;
}
int Camera::GetY()
{
	return -y;
}