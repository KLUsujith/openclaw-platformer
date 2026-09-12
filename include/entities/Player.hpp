#pragma once

#include "core/Types.hpp"
#include <vector>

namespace OpenClaw {

    class Level;
    class Enemy;

    class Player {
    public:
        Player(float startX, float startY);

        void HandleInput(const InputState& input);
        void Update(float dt, Level& level, std::vector<Enemy>& enemies);
        void TakeDamage(int damage, const Vec2& knockback);
        void AddScore(int points);
        void Heal(int amount);

        // Accessors
        Vec2 GetPosition() const { return m_position; }
        Vec2 GetVelocity() const { return m_velocity; }
        Rect GetBoundingBox() const;
        Rect GetAttackBox() const;
        PlayerState GetState() const { return m_state; }
        Direction GetFacing() const { return m_facing; }
        int GetHealth() const { return m_health; }
        int GetMaxHealth() const { return m_maxHealth; }
        int GetScore() const { return m_score; }
        int GetGems() const { return m_gems; }
        bool IsAttacking() const { return m_state == PlayerState::Attacking; }
        bool IsInvulnerable() const { return m_invulnerableTimer > 0.0f; }
        bool IsDead() const { return m_state == PlayerState::Dead; }

        void Reset(float startX, float startY);

    private:
        Vec2 m_position;
        Vec2 m_velocity;
        Direction m_facing{Direction::Right};
        PlayerState m_state{PlayerState::Idle};

        // Platforming Physics Constants
        const float MOVE_SPEED = 240.0f;
        const float ACCELERATION = 1400.0f;
        const float FRICTION = 1100.0f;
        const float GRAVITY = 950.0f;
        const float JUMP_FORCE = -480.0f;
        const float MAX_FALL_SPEED = 650.0f;

        // Mechanics & Enhancements
        bool m_isGrounded{false};
        float m_coyoteTimer{0.0f};
        const float COYOTE_TIME = 0.12f;      // Grace period to jump after falling off ledge
        float m_jumpBufferTimer{0.0f};
        const float JUMP_BUFFER = 0.10f;      // Buffer jump press before touching ground

        // Attack mechanics
        float m_attackTimer{0.0f};
        const float ATTACK_DURATION = 0.25f;
        bool m_hasHitEnemyThisSlash{false};

        // Health & Status
        int m_health{100};
        int m_maxHealth{100};
        int m_score{0};
        int m_gems{0};
        float m_invulnerableTimer{0.0f};
        const float INVULNERABLE_DURATION = 1.0f;
    };

} // namespace OpenClaw
