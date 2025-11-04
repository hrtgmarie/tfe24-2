#pragma once

#include <cmath>
#include <fmt/core.h>
#include <nlohmann/json.hpp>

class Point
{
public:
    double x_{0.0};
    double y_{0.0};

    Point() noexcept = default;
    Point(double x, double y) noexcept : x_{x}, y_{y} {}
    ~Point() = default;

    void move(double dx, double dy) noexcept { x_ += dx; y_ += dy; }
    void print() const { fmt::print("({}, {})\n", x_, y_); }
    double distance_to(const Point &other) const noexcept {
        const double dx = x_ - other.x_;
        const double dy = y_ - other.y_;
        return std::sqrt(dx*dx + dy*dy);
    }
};

// JSON helpers (nlohmann::json)
inline void to_json(nlohmann::json &j, const Point &p) {
    j = nlohmann::json{{"x", p.x_}, {"y", p.y_}};
}
inline void from_json(const nlohmann::json &j, Point &p) {
    j.at("x").get_to(p.x_);
    j.at("y").get_to(p.y_);
}