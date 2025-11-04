#pragma once

#include <type_traits>
#include <utility>
#include <cmath>
#include <fmt/format.h>

/*
  Point<T> declaration (header-only for formatter)
  ------------------------------------------------
  This header declares a generic 2D Point<T> class that supports:
  - Default and value constructors
  - in-place movement (move)
  - computing Euclidean distance (distance_to) with a return type
    that is the common type of T and double (to avoid integer truncation)
  - equality/inequality and vector-like operators (+, -)
  - a member-template scalar multiplication operator (operator*)
  
  Important design notes:
  - The class templates' member function *definitions* are provided in
    point.cpp while the formatter/template declarations remain here.
    Because templates normally require definitions visible at the point of use,
    we provide explicit instantiations in point.cpp for the concrete types
    used in this project (int and double). If you add other T types,
    you must either:
      * move the template definitions back into this header, or
      * add explicit instantiations for the new types in point.cpp.
  - fmt formatter for Point<T> is provided as a template here so fmt::print
    can be used in other translation units without requiring the formatter
    implementation to be visible in those TUs.
*/

template <typename T>
class Point {
    static_assert(std::is_arithmetic<T>::value, "Point<T>: T must be arithmetic");
public:
    T x;
    T y;

    // default constructor: sets coordinates to zero (definition in point.cpp)
    Point() noexcept;

    // value constructor: sets x and y to provided values (definition in point.cpp)
    Point(T x_, T y_) noexcept;

    // move: add dx/dy to the current coordinates (definition in point.cpp)
    void move(T dx, T dy) noexcept;

    // distance_to: returns Euclidean distance as a 'dist_t' (definition in point.cpp)
    using dist_t = std::common_type_t<T, double>;
    auto distance_to(const Point& other) const -> dist_t;

    // comparison operators (definitions in point.cpp)
    bool operator==(const Point& rhs) const noexcept;
    bool operator!=(const Point& rhs) const noexcept;

    // vector-like operators (definitions in point.cpp)
    Point operator+(const Point& rhs) const noexcept;
    Point operator-(const Point& rhs) const noexcept;

    // scalar multiplication operator: templated to accept different scalar types.
    // NOTE: the member-template definition can be placed in the header to
    // make it usable for all combinations, or explicitly instantiated in point.cpp.
    template <typename U>
    auto operator*(U s) const -> Point<std::common_type_t<T, U>>;
};

// Explicit class instantiation declarations for the T types that are
// defined/instantiated in point.cpp. These tell other translation units
// that concrete instantiations exist elsewhere (avoid link errors).
extern template class Point<int>;
extern template class Point<double>;

/*
  fmt formatter template for Point<T>
  -----------------------------------
  This generic formatter allows using fmt::format/print with Point<T>
  for any arithmetic T. The implementation is provided inline here because
  fmt requires the formatter to be available at compile-time in the TU that
  calls fmt::print/format.
*/
namespace fmt {
template <typename T>
struct formatter<Point<T>> {
    // No custom parse logic; accept default format spec.
    template <typename ParseContext>
    constexpr auto parse(ParseContext& ctx) { return ctx.begin(); }

    // Format a Point<T> as "(x, y)". Using format_to to write into the context.
    template <typename FormatContext>
    auto format(const Point<T>& p, FormatContext& ctx) const {
        return format_to(ctx.out(), "({}, {})", p.x, p.y);
    }
};
} // namespace fmt