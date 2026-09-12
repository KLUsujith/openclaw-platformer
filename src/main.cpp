#define SDL_MAIN_HANDLED
#include "core/Game.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;

    std::cout << "========================================\n";
    std::cout << "  OpenClaw 2D Platform Game Engine      \n";
    std::cout << "  Inspired by Captain Claw (1997)       \n";
    std::cout << "========================================\n";

    OpenClaw::Game game;

    if (!game.Initialize("Captain Claw - 2D Platformer (OpenClaw Engine)", 960, 540)) {
        std::cerr << "Failed to initialize game engine!\n";
        return 1;
    }

    std::cout << "Engine initialized successfully. Starting game loop...\n";
    game.Run();

    std::cout << "Game exited cleanly.\n";
    return 0;
}
