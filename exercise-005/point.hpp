#ifndef POINT_HPP
#define POINT_HPP

class Point {
    public: 
        Point();
        Point(double x, double y);
        ~Point() = default;
        void move(int dx, int dy);
        void print() const;
        double distance_to(const Point &other) const;
        double x_;
        double y_;
};

#endif // POINT_HPP