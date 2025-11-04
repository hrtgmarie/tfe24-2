#include "point.hpp"

#include <cmath>

// Definitionen der Template-Methoden
template <typename T>
Point<T>::Point() noexcept : x(static_cast<T>(0)), y(static_cast<T>(0)) {}

template <typename T>
Point<T>::Point(T x_, T y_) noexcept : x(x_), y(y_) {}

template <typename T>
void Point<T>::move(T dx, T dy) noexcept {
    x += dx;
    y += dy;
}

template <typename T>
auto Point<T>::distance_to(const Point& other) const -> dist_t {
    const auto dx = static_cast<dist_t>(x) - static_cast<dist_t>(other.x);
    const auto dy = static_cast<dist_t>(y) - static_cast<dist_t>(other.y);
    return std::hypot(dx, dy);
}

template <typename T>
bool Point<T>::operator==(const Point& rhs) const noexcept {
    return x == rhs.x && y == rhs.y;
}

template <typename T>
bool Point<T>::operator!=(const Point& rhs) const noexcept {
    return !(*this == rhs);
}

template <typename T>
Point<T> Point<T>::operator+(const Point& rhs) const noexcept {
    return Point(x + rhs.x, y + rhs.y);
}

template <typename T>
Point<T> Point<T>::operator-(const Point& rhs) const noexcept {
    return Point(x - rhs.x, y - rhs.y);
}

template <typename T>
template <typename U>
auto Point<T>::operator*(U s) const -> Point<std::common_type_t<T, U>> {
    using R = std::common_type_t<T, U>;
    return Point<R>(static_cast<R>(x) * static_cast<R>(s),
                    static_cast<R>(y) * static_cast<R>(s));
}

// Explizite Instantiierung der gewünschten Typen
template class Point<int>;
template class Point<double>;

// wenn operator* als member-template in point.cpp definiert ist, dann explizit instantiieren:
template auto Point<int>::operator*<int>(int) const -> Point<std::common_type_t<int,int>>;
template auto Point<double>::operator*<double>(double) const -> Point<std::common_type_t<double,double>>;