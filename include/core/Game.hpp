#pragma once

#include "core/Types.hpp"
#include "entities/Player.hpp"
#include "entities/Enemy.hpp"
#include "world/Level.hpp"
#include "graphics/Renderer.hpp"
#include <SDL.h>
#include <vector>
#include <memory>

namespace OpenClaw {

    class Game {
    public:
        Game();
        ~Game();

        bool Initialize(const char* title, int width, int height);
        void Run();
        void Shutdown();

    private:
        void ProcessInput();
        void Update(float dt);
        void Render();
        void ResetGame();

        bool m_isRunning{false};
        SDL_Window* m_window{nullptr};
        std::unique_ptr<Renderer> m_renderer;

        InputState m_input;
        std::unique_ptr<Player> m_player;
        Level m_level;
        std::vector<Enemy> m_enemies;

        const int SCREEN_WIDTH = 960;
        const int SCREEN_HEIGHT = 540;
    };

} // namespace OpenClaw
