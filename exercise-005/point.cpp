#include "point.hpp"
#include <fmt/core.h>
#include <cmath>

Point::Point(){
    x_ = 0;
    y_ = 0;
}

Point::Point(double x, double y){
    x_ = x;
    y_ = y;
}


void Point::print() const
{
    fmt::print("({}, {})\n", x_, y_);
}

void Point::move(int dx, int dy)
{
    x_ += dx;
    y_ += dy;
}

double Point::distance_to(const Point &other) const
{
    const double dx = static_cast<double>(x_ - other.x_);
    const double dy = static_cast<double>(y_ - other.y_);
    return std::sqrt(dx * dx + dy * dy);
}