#pragma once

#include "core/Types.hpp"
#include <vector>

namespace OpenClaw {

    class Collision {
    public:
        // Check if two AABB rectangles intersect
        static bool CheckAABB(const Rect& a, const Rect& b);

        // Calculate penetration depth and collision normal
        static CollisionResult ResolveAABB(const Rect& dynamicBox, const Vec2& velocity, const Rect& staticBox);

        // Check point inside rectangle
        static bool PointInRect(const Vec2& point, const Rect& rect);
    };

} // namespace OpenClaw
