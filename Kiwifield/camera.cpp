#include "camera.h"

Camera::Camera()
{

}
Camera::Camera(int x, int y)
{
	this->x = x;
	this->y = y;
}
int Camera::getX()
{
	return -x;
}
int Camera::getY()
{
	return -y;
}