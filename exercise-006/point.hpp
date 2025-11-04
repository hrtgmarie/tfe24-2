#pragma once

#include <type_traits>
#include <utility>
#include <cmath>
#include <fmt/format.h>

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

// explizite Instantiierungen (Deklaration) für die in cpp instanzierten Typen
extern template class Point<int>;
extern template class Point<double>;

// fmt formatter template — muss im Header stehen, damit fmt::print funktioniert
namespace fmt {
template <typename T>
struct formatter<Point<T>> {
    template <typename ParseContext>
    constexpr auto parse(ParseContext& ctx) { return ctx.begin(); }

    template <typename FormatContext>
    auto format(const Point<T>& p, FormatContext& ctx) const {
        return format_to(ctx.out(), "({}, {})", p.x, p.y);
    }
};
} // namespace fmt