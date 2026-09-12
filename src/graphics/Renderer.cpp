#include "graphics/Renderer.hpp"
#include "entities/Player.hpp"
#include "entities/Enemy.hpp"
#include "world/Level.hpp"
#include <algorithm>
#include <cmath>

namespace OpenClaw {

    Renderer::Renderer() = default;

    Renderer::~Renderer() {
        Cleanup();
    }

    bool Renderer::Initialize(SDL_Window* window, int screenWidth, int screenHeight) {
        m_screenWidth = screenWidth;
        m_screenHeight = screenHeight;

        // Create hardware accelerated VSync renderer
        m_sdlRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (!m_sdlRenderer) {
            // Fallback to software renderer if GPU unavailable
            m_sdlRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
        }

        return (m_sdlRenderer != nullptr);
    }

    void Renderer::Cleanup() {
        if (m_sdlRenderer) {
            SDL_DestroyRenderer(m_sdlRenderer);
            m_sdlRenderer = nullptr;
        }
    }

    Vec2 Renderer::WorldToScreen(const Vec2& worldPos) const {
        return {worldPos.x - m_cameraPos.x, worldPos.y - m_cameraPos.y};
    }

    Rect Renderer::WorldToScreen(const Rect& worldRect) const {
        return {worldRect.x - m_cameraPos.x, worldRect.y - m_cameraPos.y, worldRect.w, worldRect.h};
    }

    void Renderer::UpdateCamera(const Vec2& targetPos, float levelWidth, float levelHeight) {
        // Smooth camera lerp centered around player
        float targetCamX = targetPos.x - m_screenWidth * 0.5f;
        float targetCamY = targetPos.y - m_screenHeight * 0.6f;

        m_cameraPos.x += (targetCamX - m_cameraPos.x) * 0.08f;
        m_cameraPos.y += (targetCamY - m_cameraPos.y) * 0.08f;

        // Clamp camera to level bounds
        m_cameraPos.x = std::max(0.0f, std::min(m_cameraPos.x, levelWidth - m_screenWidth));
        m_cameraPos.y = std::max(0.0f, std::min(m_cameraPos.y, levelHeight - m_screenHeight));

        m_animTimer += 0.016f;
    }

    void Renderer::DrawBackground() {
        // Gradient sky / dungeon atmosphere
        SDL_SetRenderDrawColor(m_sdlRenderer, 22, 28, 48, 255); // Night fortress blue
        SDL_RenderClear(m_sdlRenderer);

        // Parallax dungeon bricks in the background
        SDL_SetRenderDrawColor(m_sdlRenderer, 32, 38, 62, 255);
        float parallaxOffset = m_cameraPos.x * 0.2f;
        for (int i = 0; i < 20; ++i) {
            float x = i * 160.0f - std::fmod(parallaxOffset, 160.0f);
            SDL_Rect pillar = {static_cast<int>(x), 60, 40, m_screenHeight - 120};
            SDL_RenderFillRect(m_sdlRenderer, &pillar);
        }
    }

    void Renderer::DrawLevel(const Level& level) {
        // Solid stone tiles & platforms
        for (const auto& tile : level.GetSolidTiles()) {
            Rect sRect = WorldToScreen(tile);
            SDL_Rect r = {static_cast<int>(sRect.x), static_cast<int>(sRect.y), static_cast<int>(sRect.w), static_cast<int>(sRect.h)};

            // Stone brick fill
            SDL_SetRenderDrawColor(m_sdlRenderer, 78, 65, 58, 255);
            SDL_RenderFillRect(m_sdlRenderer, &r);

            // Platform top rim highlight
            SDL_SetRenderDrawColor(m_sdlRenderer, 120, 105, 95, 255);
            SDL_Rect topRim = {r.x, r.y, r.w, 4};
            SDL_RenderFillRect(m_sdlRenderer, &topRim);

            // Platform border
            SDL_SetRenderDrawColor(m_sdlRenderer, 45, 36, 32, 255);
            SDL_RenderDrawRect(m_sdlRenderer, &r);
        }

        // Hazards (Spikes)
        for (const auto& hazard : level.GetHazards()) {
            Rect sRect = WorldToScreen(hazard.bounds);
            SDL_Rect r = {static_cast<int>(sRect.x), static_cast<int>(sRect.y), static_cast<int>(sRect.w), static_cast<int>(sRect.h)};
            SDL_SetRenderDrawColor(m_sdlRenderer, 180, 40, 40, 255);
            SDL_RenderFillRect(m_sdlRenderer, &r);
        }

        // Collectibles (Gems and Coins)
        for (const auto& item : level.GetCollectibles()) {
            if (item.collected) continue;

            float bobbing = std::sin(m_animTimer * 5.0f + item.bounds.x) * 4.0f;
            Rect sRect = WorldToScreen(item.bounds);
            SDL_Rect r = {static_cast<int>(sRect.x), static_cast<int>(sRect.y + bobbing), static_cast<int>(sRect.w), static_cast<int>(sRect.h)};

            if (item.isHealth) {
                SDL_SetRenderDrawColor(m_sdlRenderer, 40, 220, 90, 255); // Health green elixir
            } else if (item.value >= 500) {
                SDL_SetRenderDrawColor(m_sdlRenderer, 230, 40, 50, 255); // Red pirate ruby
            } else {
                SDL_SetRenderDrawColor(m_sdlRenderer, 245, 190, 30, 255); // Gold coin
            }
            SDL_RenderFillRect(m_sdlRenderer, &r);
        }

        // Exit Gate
        Rect gateRect = WorldToScreen(level.GetExitGate());
        SDL_Rect gr = {static_cast<int>(gateRect.x), static_cast<int>(gateRect.y), static_cast<int>(gateRect.w), static_cast<int>(gateRect.h)};
        SDL_SetRenderDrawColor(m_sdlRenderer, 140, 95, 45, 255);
        SDL_RenderFillRect(m_sdlRenderer, &gr);
        SDL_SetRenderDrawColor(m_sdlRenderer, 255, 215, 0, 255);
        SDL_RenderDrawRect(m_sdlRenderer, &gr);
    }

    void Renderer::DrawPlayer(const Player& player) {
        if (player.IsDead()) return;

        // Hurt invulnerability blink
        if (player.IsInvulnerable() && (static_cast<int>(m_animTimer * 30.0f) % 2 == 0)) {
            return;
        }

        Rect sBox = WorldToScreen(player.GetBoundingBox());
        SDL_Rect r = {static_cast<int>(sBox.x), static_cast<int>(sBox.y), static_cast<int>(sBox.w), static_cast<int>(sBox.h)};

        // Claw's pirate coat (Navy Blue)
        SDL_SetRenderDrawColor(m_sdlRenderer, 24, 45, 95, 255);
        SDL_RenderFillRect(m_sdlRenderer, &r);

        // Claw's pirate bicorne hat (Dark Gold Trim)
        SDL_Rect hat = {r.x - 2, r.y - 8, r.w + 4, 10};
        SDL_SetRenderDrawColor(m_sdlRenderer, 212, 175, 55, 255);
        SDL_RenderFillRect(m_sdlRenderer, &hat);

        // Sword Slash Visual Arc
        if (player.IsAttacking()) {
            Rect aBox = WorldToScreen(player.GetAttackBox());
            SDL_Rect slash = {static_cast<int>(aBox.x), static_cast<int>(aBox.y), static_cast<int>(aBox.w), static_cast<int>(aBox.h)};
            SDL_SetRenderDrawColor(m_sdlRenderer, 255, 255, 220, 200);
            SDL_RenderFillRect(m_sdlRenderer, &slash);
        }
    }

    void Renderer::DrawEnemies(const std::vector<Enemy>& enemies) {
        for (const auto& enemy : enemies) {
            if (enemy.IsDead()) continue;

            Rect sBox = WorldToScreen(enemy.GetBoundingBox());
            SDL_Rect r = {static_cast<int>(sBox.x), static_cast<int>(sBox.y), static_cast<int>(sBox.w), static_cast<int>(sBox.h)};

            if (enemy.GetType() == EnemyType::RedCoatOfficer) {
                SDL_SetRenderDrawColor(m_sdlRenderer, 190, 35, 40, 255); // Red-coat guard
            } else {
                SDL_SetRenderDrawColor(m_sdlRenderer, 100, 75, 55, 255); // Pirate rat
            }
            SDL_RenderFillRect(m_sdlRenderer, &r);
        }
    }

    void Renderer::DrawHUD(const Player& player) {
        // Health bar background
        SDL_Rect hpBg = {20, 20, 204, 20};
        SDL_SetRenderDrawColor(m_sdlRenderer, 40, 40, 40, 255);
        SDL_RenderFillRect(m_sdlRenderer, &hpBg);

        // Health bar current
        float healthPct = static_cast<float>(player.GetHealth()) / static_cast<float>(player.GetMaxHealth());
        int currentHpWidth = static_cast<int>(200.0f * healthPct);
        SDL_Rect hpFill = {22, 22, std::max(0, currentHpWidth), 16};
        SDL_SetRenderDrawColor(m_sdlRenderer, 220, 45, 45, 255);
        SDL_RenderFillRect(m_sdlRenderer, &hpFill);

        // Gold border
        SDL_SetRenderDrawColor(m_sdlRenderer, 212, 175, 55, 255);
        SDL_RenderDrawRect(m_sdlRenderer, &hpBg);
    }

    void Renderer::Render(const Player& player, const std::vector<Enemy>& enemies, const Level& level) {
        DrawBackground();
        DrawLevel(level);
        DrawPlayer(player);
        DrawEnemies(enemies);
        DrawHUD(player);

        SDL_RenderPresent(m_sdlRenderer);
    }

} // namespace OpenClaw
