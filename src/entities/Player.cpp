#include "entities/Player.hpp"
#include "entities/Enemy.hpp"
#include "world/Level.hpp"
#include "physics/Collision.hpp"
#include <cmath>
#include <algorithm>

namespace OpenClaw {

    Player::Player(float startX, float startY)
        : m_position(startX, startY), m_velocity(0.0f, 0.0f) {}

    Rect Player::GetBoundingBox() const {
        return Rect(m_position.x + 8.0f, m_position.y + 4.0f, 28.0f, 44.0f);
    }

    Rect Player::GetAttackBox() const {
        if (m_facing == Direction::Right) {
            return Rect(m_position.x + 32.0f, m_position.y + 10.0f, 32.0f, 30.0f);
        } else {
            return Rect(m_position.x - 20.0f, m_position.y + 10.0f, 32.0f, 30.0f);
        }
    }

    void Player::HandleInput(const InputState& input) {
        if (m_state == PlayerState::Dead) return;

        // Jump buffer
        if (input.jumpPressed) {
            m_jumpBufferTimer = JUMP_BUFFER;
        }

        // Horizontal input
        if (input.moveLeft && !input.moveRight) {
            m_facing = Direction::Left;
            m_velocity.x = std::max(m_velocity.x - ACCELERATION * 0.016f, -MOVE_SPEED);
        } else if (input.moveRight && !input.moveLeft) {
            m_facing = Direction::Right;
            m_velocity.x = std::min(m_velocity.x + ACCELERATION * 0.016f, MOVE_SPEED);
        } else {
            // Apply friction when no input
            if (m_velocity.x > 0.0f) {
                m_velocity.x = std::max(0.0f, m_velocity.x - FRICTION * 0.016f);
            } else if (m_velocity.x < 0.0f) {
                m_velocity.x = std::min(0.0f, m_velocity.x + FRICTION * 0.016f);
            }
        }

        // Sword attack trigger
        if (input.attackPressed && m_attackTimer <= 0.0f) {
            m_attackTimer = ATTACK_DURATION;
            m_state = PlayerState::Attacking;
            m_hasHitEnemyThisSlash = false;
        }

        // Variable jump height cut when release jump button
        if (!input.jumpHeld && m_velocity.y < -150.0f) {
            m_velocity.y = -150.0f;
        }
    }

    void Player::Update(float dt, Level& level, std::vector<Enemy>& enemies) {
        if (m_state == PlayerState::Dead) return;

        // Timers
        if (m_jumpBufferTimer > 0.0f) m_jumpBufferTimer -= dt;
        if (m_coyoteTimer > 0.0f) m_coyoteTimer -= dt;
        if (m_invulnerableTimer > 0.0f) m_invulnerableTimer -= dt;

        // Attack update
        if (m_attackTimer > 0.0f) {
            m_attackTimer -= dt;
            if (m_attackTimer <= 0.0f) {
                m_state = m_isGrounded ? PlayerState::Idle : PlayerState::Falling;
            }
        }

        // Jump execution (Coyote Time + Jump Buffering)
        if (m_jumpBufferTimer > 0.0f && (m_isGrounded || m_coyoteTimer > 0.0f)) {
            m_velocity.y = JUMP_FORCE;
            m_isGrounded = false;
            m_coyoteTimer = 0.0f;
            m_jumpBufferTimer = 0.0f;
        }

        // Apply Gravity
        m_velocity.y = std::min(m_velocity.y + GRAVITY * dt, MAX_FALL_SPEED);

        // Move Horizontal and resolve collisions
        m_position.x += m_velocity.x * dt;
        Rect boxX = GetBoundingBox();
        for (const auto& tile : level.GetSolidTiles()) {
            if (Collision::CheckAABB(boxX, tile)) {
                if (m_velocity.x > 0.0f) {
                    m_position.x = tile.x - boxX.w - 8.0f;
                } else if (m_velocity.x < 0.0f) {
                    m_position.x = tile.x + tile.w - 8.0f;
                }
                m_velocity.x = 0.0f;
                boxX = GetBoundingBox();
            }
        }

        // Move Vertical and resolve collisions
        m_position.y += m_velocity.y * dt;
        Rect boxY = GetBoundingBox();
        bool wasGrounded = m_isGrounded;
        m_isGrounded = false;

        for (const auto& tile : level.GetSolidTiles()) {
            if (Collision::CheckAABB(boxY, tile)) {
                if (m_velocity.y > 0.0f) { // Landing
                    m_position.y = tile.y - boxY.h - 4.0f;
                    m_velocity.y = 0.0f;
                    m_isGrounded = true;
                    m_coyoteTimer = COYOTE_TIME;
                } else if (m_velocity.y < 0.0f) { // Head bump
                    m_position.y = tile.y + tile.h - 4.0f;
                    m_velocity.y = 0.0f;
                }
                boxY = GetBoundingBox();
            }
        }

        // If just left the ground without jumping, start coyote timer
        if (wasGrounded && !m_isGrounded && m_velocity.y >= 0.0f) {
            m_coyoteTimer = COYOTE_TIME;
        }

        // Check platform collectibles & hazards
        level.CheckCollectibles(*this);
        if (level.CheckHazards(GetBoundingBox())) {
            TakeDamage(100, Vec2(0, -300.0f)); // Instant hazard defeat
        }

        // Attack enemies check
        if (IsAttacking() && !m_hasHitEnemyThisSlash) {
            Rect slashBox = GetAttackBox();
            for (auto& enemy : enemies) {
                if (!enemy.IsDead() && Collision::CheckAABB(slashBox, enemy.GetBoundingBox())) {
                    enemy.TakeDamage(35, m_facing == Direction::Right ? Vec2(250.0f, -150.0f) : Vec2(-250.0f, -150.0f));
                    AddScore(150);
                    m_hasHitEnemyThisSlash = true;
                }
            }
        }

        // State update
        if (m_attackTimer <= 0.0f) {
            if (!m_isGrounded) {
                m_state = (m_velocity.y < 0.0f) ? PlayerState::Jumping : PlayerState::Falling;
            } else if (std::abs(m_velocity.x) > 10.0f) {
                m_state = PlayerState::Running;
            } else {
                m_state = PlayerState::Idle;
            }
        }
    }

    void Player::TakeDamage(int damage, const Vec2& knockback) {
        if (m_invulnerableTimer > 0.0f || m_state == PlayerState::Dead) return;

        m_health -= damage;
        m_invulnerableTimer = INVULNERABLE_DURATION;
        m_velocity = knockback;

        if (m_health <= 0) {
            m_health = 0;
            m_state = PlayerState::Dead;
        } else {
            m_state = PlayerState::Hurt;
        }
    }

    void Player::AddScore(int points) {
        m_score += points;
        m_gems++;
    }

    void Player::Heal(int amount) {
        m_health = std::min(m_maxHealth, m_health + amount);
    }

    void Player::Reset(float startX, float startY) {
        m_position = Vec2(startX, startY);
        m_velocity = Vec2(0.0f, 0.0f);
        m_health = m_maxHealth;
        m_state = PlayerState::Idle;
        m_facing = Direction::Right;
        m_invulnerableTimer = 0.0f;
    }

} // namespace OpenClaw
