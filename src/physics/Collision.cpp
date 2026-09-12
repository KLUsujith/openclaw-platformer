#include "physics/Collision.hpp"
#include <cmath>
#include <algorithm>

namespace OpenClaw {

    bool Collision::CheckAABB(const Rect& a, const Rect& b) {
        return (a.x < b.x + b.w &&
                a.x + a.w > b.x &&
                a.y < b.y + b.h &&
                a.y + a.h > b.y);
    }

    bool Collision::PointInRect(const Vec2& point, const Rect& rect) {
        return (point.x >= rect.x && point.x <= rect.x + rect.w &&
                point.y >= rect.y && point.y <= rect.y + rect.h);
    }

    CollisionResult Collision::ResolveAABB(const Rect& dynamicBox, const Vec2& velocity, const Rect& staticBox) {
        CollisionResult result;
        if (!CheckAABB(dynamicBox, staticBox)) {
            return result;
        }

        result.collided = true;

        float overlapLeft   = (dynamicBox.x + dynamicBox.w) - staticBox.x;
        float overlapRight  = (staticBox.x + staticBox.w) - dynamicBox.x;
        float overlapTop    = (dynamicBox.y + dynamicBox.h) - staticBox.y;
        float overlapBottom = (staticBox.y + staticBox.h) - dynamicBox.y;

        float minOverlapX = (overlapLeft < overlapRight) ? overlapLeft : overlapRight;
        float minOverlapY = (overlapTop < overlapBottom) ? overlapTop : overlapBottom;

        // Resolve along shallowest axis
        if (minOverlapX < minOverlapY) {
            result.penetration = minOverlapX;
            if (overlapLeft < overlapRight) {
                result.normal = Vec2(-1.0f, 0.0f);
            } else {
                result.normal = Vec2(1.0f, 0.0f);
            }
        } else {
            result.penetration = minOverlapY;
            if (overlapTop < overlapBottom) {
                result.normal = Vec2(0.0f, -1.0f); // Hit ceiling / landed on top
            } else {
                result.normal = Vec2(0.0f, 1.0f);
            }
        }

        return result;
    }

} // namespace OpenClaw
