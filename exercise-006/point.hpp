#pragma once

#include <type_traits>
#include <utility>
#include <cmath>

template <typename T>
class Point {
    static_assert(std::is_arithmetic<T>::value, "Point<T>: T must be arithmetic");
public:
    T x;
    T y;

    Point() noexcept;
    Point(T x_, T y_) noexcept;

    void move(T dx, T dy) noexcept;

    using dist_t = std::common_type_t<T, double>;
    auto distance_to(const Point& other) const -> dist_t;

    bool operator==(const Point& rhs) const noexcept;
    bool operator!=(const Point& rhs) const noexcept;
    Point operator+(const Point& rhs) const noexcept;
    Point operator-(const Point& rhs) const noexcept;

    template <typename U>
    auto operator*(U s) const -> Point<std::common_type_t<T, U>>;
};

// Prevent implicit instantiation in other TUs for the concrete types we provide here
extern template class Point<int>;
extern template class Point<double>;

namespace fmt {
    // forward declare concrete formatter specializations (definitions in cpp)
    template<> struct formatter<Point<int>>;
    template<> struct formatter<Point<double>>;
}