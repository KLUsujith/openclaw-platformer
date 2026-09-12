#pragma once

#include "core/Types.hpp"
#include <SDL.h>
#include <vector>

namespace OpenClaw {

    class Player;
    class Enemy;
    class Level;

    class Renderer {
    public:
        Renderer();
        ~Renderer();

        bool Initialize(SDL_Window* window, int screenWidth, int screenHeight);
        void Cleanup();

        void UpdateCamera(const Vec2& targetPos, float levelWidth, float levelHeight);
        void Render(const Player& player, const std::vector<Enemy>& enemies, const Level& level);

        Vec2 WorldToScreen(const Vec2& worldPos) const;
        Rect WorldToScreen(const Rect& worldRect) const;

    private:
        void DrawBackground();
        void DrawLevel(const Level& level);
        void DrawPlayer(const Player& player);
        void DrawEnemies(const std::vector<Enemy>& enemies);
        void DrawHUD(const Player& player);

        SDL_Renderer* m_sdlRenderer{nullptr};
        int m_screenWidth{960};
        int m_screenHeight{540};
        Vec2 m_cameraPos{0.0f, 0.0f};
        float m_animTimer{0.0f};
    };

} // namespace OpenClaw
