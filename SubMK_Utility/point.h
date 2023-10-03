#ifndef POINT_H
#define POINT_H


class Point
{
public:
    Point(int argX = -1, int argY = -1);
    Point(const Point& other);

    Point operator +(const Point& other);
    Point& operator+=(const Point& other);

    void setPoint(const Point &point);

public:
    int x,y;
};

#endif // POINT_H
