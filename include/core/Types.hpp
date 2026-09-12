#pragma once

#include <cstdint>
#include <string>

namespace OpenClaw {

    enum class Direction {
        Left = -1,
        Right = 1
    };

    enum class PlayerState {
        Idle,
        Running,
        Jumping,
        Falling,
        Attacking,
        Hurt,
        Dead
    };

    struct Vec2 {
        float x{0.0f};
        float y{0.0f};

        constexpr Vec2() = default;
        constexpr Vec2(float inX, float inY) : x(inX), y(inY) {}

        Vec2 operator+(const Vec2& other) const { return {x + other.x, y + other.y}; }
        Vec2 operator-(const Vec2& other) const { return {x - other.x, y - other.y}; }
        Vec2 operator*(float scalar) const { return {x * scalar, y * scalar}; }
        Vec2& operator+=(const Vec2& other) { x += other.x; y += other.y; return *this; }
    };

    struct Rect {
        float x{0.0f};
        float y{0.0f};
        float w{0.0f};
        float h{0.0f};

        constexpr Rect() = default;
        constexpr Rect(float inX, float inY, float inW, float inH)
            : x(inX), y(inY), w(inW), h(inH) {}

        float left()   const { return x; }
        float right()  const { return x + w; }
        float top()    const { return y; }
        float bottom() const { return y + h; }
    };

    struct CollisionResult {
        bool collided{false};
        Vec2 normal{0.0f, 0.0f};
        float penetration{0.0f};
    };

    struct InputState {
        bool moveLeft{false};
        bool moveRight{false};
        bool jumpPressed{false};
        bool jumpHeld{false};
        bool attackPressed{false};
        bool restartPressed{false};
    };

} // namespace OpenClaw
