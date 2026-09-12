#pragma once

#include "core/Types.hpp"

namespace OpenClaw {

    class Level;
    class Player;

    enum class EnemyType {
        RatSoldier,
        RedCoatOfficer
    };

    class Enemy {
    public:
        Enemy(float x, float y, EnemyType type = EnemyType::RatSoldier, float patrolDistance = 160.0f);

        void Update(float dt, Level& level, Player& player);
        void TakeDamage(int damage, const Vec2& knockback);

        Rect GetBoundingBox() const;
        bool IsDead() const { return m_health <= 0; }
        Direction GetFacing() const { return m_facing; }
        EnemyType GetType() const { return m_type; }

    private:
        Vec2 m_position;
        Vec2 m_velocity;
        EnemyType m_type;
        float m_startX;
        float m_patrolDistance;
        Direction m_facing{Direction::Left};
        int m_health{50};
        float m_speed{65.0f};
        float m_attackCooldown{0.0f};
    };

} // namespace OpenClaw
