#include "core/Game.hpp"
#include <iostream>

namespace OpenClaw {

    Game::Game() = default;

    Game::~Game() {
        Shutdown();
    }

    bool Game::Initialize(const char* title, int width, int height) {
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) != 0) {
            std::cerr << "SDL_Init Error: " << SDL_GetError() << "\n";
            return false;
        }

        m_window = SDL_CreateWindow(
            title,
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            width,
            height,
            SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
        );

        if (!m_window) {
            std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << "\n";
            return false;
        }

        m_renderer = std::make_unique<Renderer>();
        if (!m_renderer->Initialize(m_window, width, height)) {
            std::cerr << "Renderer initialization failed!\n";
            return false;
        }

        // Initialize Player & Enemies
        m_player = std::make_unique<Player>(80.0f, 400.0f);

        // Spawn Enemies across the fortress platforms
        m_enemies.emplace_back(460.0f, 290.0f, EnemyType::RatSoldier, 60.0f);
        m_enemies.emplace_back(1240.0f, 300.0f, EnemyType::RedCoatOfficer, 80.0f);
        m_enemies.emplace_back(2120.0f, 310.0f, EnemyType::RatSoldier, 50.0f);
        m_enemies.emplace_back(2580.0f, 170.0f, EnemyType::RedCoatOfficer, 70.0f);

        m_isRunning = true;
        return true;
    }

    void Game::ProcessInput() {
        SDL_Event event;
        m_input.jumpPressed = false;
        m_input.attackPressed = false;
        m_input.restartPressed = false;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                m_isRunning = false;
            } else if (event.type == SDL_KEYDOWN && !event.key.repeat) {
                switch (event.key.keysym.sym) {
                    case SDLK_SPACE:
                    case SDLK_w:
                    case SDLK_UP:
                        m_input.jumpPressed = true;
                        m_input.jumpHeld = true;
                        break;
                    case SDLK_j:
                    case SDLK_f:
                    case SDLK_LCTRL:
                        m_input.attackPressed = true;
                        break;
                    case SDLK_r:
                        m_input.restartPressed = true;
                        break;
                    case SDLK_ESCAPE:
                        m_isRunning = false;
                        break;
                    default:
                        break;
                }
            } else if (event.type == SDL_KEYUP) {
                switch (event.key.keysym.sym) {
                    case SDLK_SPACE:
                    case SDLK_w:
                    case SDLK_UP:
                        m_input.jumpHeld = false;
                        break;
                    default:
                        break;
                }
            }
        }

        // Continuous key states for smooth movement
        const Uint8* state = SDL_GetKeyboardState(nullptr);
        m_input.moveLeft = (state[SDL_SCANCODE_A] || state[SDL_SCANCODE_LEFT]);
        m_input.moveRight = (state[SDL_SCANCODE_D] || state[SDL_SCANCODE_RIGHT]);

        if (m_input.restartPressed) {
            ResetGame();
        }
    }

    void Game::Update(float dt) {
        if (m_player) {
            m_player->HandleInput(m_input);
            m_player->Update(dt, m_level, m_enemies);

            // Update all enemies
            for (auto& enemy : m_enemies) {
                enemy.Update(dt, m_level, *m_player);
            }

            // Smooth camera tracking
            m_renderer->UpdateCamera(m_player->GetPosition(), m_level.GetWidth(), m_level.GetHeight());
        }
    }

    void Game::Render() {
        if (m_renderer && m_player) {
            m_renderer->Render(*m_player, m_enemies, m_level);
        }
    }

    void Game::ResetGame() {
        m_player->Reset(80.0f, 400.0f);
        m_level.LoadLevel1();
        m_enemies.clear();
        m_enemies.emplace_back(460.0f, 290.0f, EnemyType::RatSoldier, 60.0f);
        m_enemies.emplace_back(1240.0f, 300.0f, EnemyType::RedCoatOfficer, 80.0f);
        m_enemies.emplace_back(2120.0f, 310.0f, EnemyType::RatSoldier, 50.0f);
        m_enemies.emplace_back(2580.0f, 170.0f, EnemyType::RedCoatOfficer, 70.0f);
    }

    void Game::Run() {
        Uint64 lastTime = SDL_GetPerformanceCounter();
        const double targetDelta = 1.0 / 60.0; // Fixed 60 FPS update
        double accumulator = 0.0;

        while (m_isRunning) {
            Uint64 currentTime = SDL_GetPerformanceCounter();
            double frameTime = static_cast<double>(currentTime - lastTime) / SDL_GetPerformanceFrequency();
            lastTime = currentTime;

            // Cap large delta spikes
            if (frameTime > 0.25) frameTime = 0.25;
            accumulator += frameTime;

            ProcessInput();

            while (accumulator >= targetDelta) {
                Update(static_cast<float>(targetDelta));
                accumulator -= targetDelta;
            }

            Render();
        }
    }

    void Game::Shutdown() {
        if (m_renderer) {
            m_renderer->Cleanup();
        }
        if (m_window) {
            SDL_DestroyWindow(m_window);
            m_window = nullptr;
        }
        SDL_Quit();
    }

} // namespace OpenClaw
