#include "camera.h"

Camera::Camera()
{

}
Camera::Camera(int x, int y)
{
	this->x = x;
	this->y = y;
}

void Camera::u(int& x, int& y)
{
	x = x - this->x;
	y = y - this->y;
}