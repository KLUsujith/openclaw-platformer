#include "entities/Enemy.hpp"
#include "entities/Player.hpp"
#include "world/Level.hpp"
#include "physics/Collision.hpp"
#include <cmath>

namespace OpenClaw {

    Enemy::Enemy(float x, float y, EnemyType type, float patrolDistance)
        : m_position(x, y), m_type(type), m_startX(x), m_patrolDistance(patrolDistance) {
        if (type == EnemyType::RedCoatOfficer) {
            m_health = 80;
            m_speed = 85.0f;
        }
    }

    Rect Enemy::GetBoundingBox() const {
        return Rect(m_position.x + 6.0f, m_position.y + 4.0f, 28.0f, 44.0f);
    }

    void Enemy::Update(float dt, Level& level, Player& player) {
        if (IsDead()) return;

        if (m_attackCooldown > 0.0f) {
            m_attackCooldown -= dt;
        }

        // Horizontal Patrol
        float moveDelta = static_cast<float>(m_facing) * m_speed * dt;
        m_position.x += moveDelta;

        // Turn around if exceeded patrol range
        if (m_position.x > m_startX + m_patrolDistance) {
            m_facing = Direction::Left;
        } else if (m_position.x < m_startX - m_patrolDistance) {
            m_facing = Direction::Right;
        }

        // Wall collisions
        Rect box = GetBoundingBox();
        for (const auto& tile : level.GetSolidTiles()) {
            if (Collision::CheckAABB(box, tile)) {
                if (m_facing == Direction::Right) {
                    m_facing = Direction::Left;
                    m_position.x = tile.x - box.w - 6.0f;
                } else {
                    m_facing = Direction::Right;
                    m_position.x = tile.x + tile.w - 6.0f;
                }
                break;
            }
        }

        // Check collision with player
        if (!player.IsDead() && !player.IsInvulnerable()) {
            if (Collision::CheckAABB(GetBoundingBox(), player.GetBoundingBox())) {
                Vec2 knockback = (player.GetPosition().x > m_position.x) ? Vec2(220.0f, -200.0f) : Vec2(-220.0f, -200.0f);
                player.TakeDamage(15, knockback);
                m_attackCooldown = 1.0f;
            }
        }
    }

    void Enemy::TakeDamage(int damage, const Vec2& knockback) {
        m_health -= damage;
        m_position.x += knockback.x * 0.05f;
    }

} // namespace OpenClaw
