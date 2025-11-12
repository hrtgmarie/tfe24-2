#include "point.hpp"

#include <cmath>

/*
  Definitions of Point<T> members
  ------------------------------
  The following template definitions implement the class declared in the header.
  Because these are templates, other TUs using new types would normally require
  the definitions to be visible in the header. To avoid exposing all definitions
  in the header we explicitly instantiate the concrete types used (int, double)
  at the end of this file.
*/

template <typename T>
Point<T>::Point() noexcept : x(static_cast<T>(0)), y(static_cast<T>(0)) {}

template <typename T>
Point<T>::Point(T x_, T y_) noexcept : x(x_), y(y_) {}

template <typename T>
void Point<T>::move(T dx, T dy) noexcept {
    // In-place mutation: add offset values to the existing coordinates.
    x += dx;
    y += dy;
}

template <typename T>
auto Point<T>::distance_to(const Point& other) const -> dist_t {
    // Compute Euclidean distance using a floating point-like accumulator
    // (std::common_type_t<T,double>) to avoid integer truncation for integer T.
    const auto dx = static_cast<dist_t>(x) - static_cast<dist_t>(other.x);
    const auto dy = static_cast<dist_t>(y) - static_cast<dist_t>(other.y);
    return std::hypot(dx, dy); // numerically stable hypot for sqrt(dx*dx + dy*dy)
}

template <typename T>
bool Point<T>::operator==(const Point& rhs) const noexcept {
    // Exact comparison. If T is floating-point and tolerances are needed,
    // tests should use Approx on distance instead.
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
    // Scalar multiplication returns a Point whose coordinate type is the
    // common type between T and U (e.g., int * int -> int; int * double -> double).
    using R = std::common_type_t<T, U>;
    return Point<R>(static_cast<R>(x) * static_cast<R>(s),
                    static_cast<R>(y) * static_cast<R>(s));
}

/*
  Explicit instantiations
  -----------------------
  Provide concrete instantiations for the types the application/tests use.
  This avoids requiring the header to expose all template definitions.
  Add more lines here if additional T combinations are required.
*/
template class Point<int>;
template class Point<double>;

// wenn operator* als member-template in point.cpp definiert ist, dann explizit instantiieren:
template auto Point<int>::operator*<int>(int) const -> Point<std::common_type_t<int,int>>;
template auto Point<double>::operator*<double>(double) const -> Point<std::common_type_t<double,double>>;