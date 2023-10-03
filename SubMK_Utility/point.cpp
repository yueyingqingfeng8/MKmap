#include "point.h"

Point::Point(int argX, int argY)
    : x(argX), y(argY)
{

}

Point::Point(const Point& other)
    : x(other.x), y(other.y)
{

}

Point Point::operator+(const Point& other)
{
    Point result;
    result.x = x + other.x;
    result.y = y + other.y;
    return result;
}

Point& Point::operator+=(const Point& other)
{
    x += other.x;
    y += other.y;
    return *this;
}

void Point::setPoint(const Point& point)
{
    x = point.x;
    y = point.y;
}
