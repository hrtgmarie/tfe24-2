#include "point.hpp"

#include <fmt/core.h>
#include <cmath>
#include <type_traits>

// Definitions for template members
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

// Explicit instantiations for the types we want to support
template class Point<int>;
template class Point<double>;

// fmt formatter specializations for concrete Point types
namespace fmt {

template <>
struct formatter<Point<int>> {
    // parse not needed; use default behavior
    template <typename ParseContext>
    constexpr auto parse(ParseContext& ctx) { return ctx.begin(); }

    template <typename FormatContext>
    auto format(const Point<int>& p, FormatContext& ctx) const {
        return format_to(ctx.out(), "({}, {})", p.x, p.y);
    }
};

template <>
struct formatter<Point<double>> {
    template <typename ParseContext>
    constexpr auto parse(ParseContext& ctx) { return ctx.begin(); }

    template <typename FormatContext>
    auto format(const Point<double>& p, FormatContext& ctx) const {
        return format_to(ctx.out(), "({}, {})", p.x, p.y);
    }
};

} // namespace fmt