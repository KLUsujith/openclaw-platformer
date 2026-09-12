#include "world/Level.hpp"
#include "entities/Player.hpp"
#include "physics/Collision.hpp"

namespace OpenClaw {

    Level::Level() {
        LoadLevel1();
    }

    void Level::LoadLevel1() {
        m_solidTiles.clear();
        m_collectibles.clear();
        m_hazards.clear();

        // Ground floors
        m_solidTiles.emplace_back(0.0f, 520.0f, 800.0f, 100.0f);
        m_solidTiles.emplace_back(900.0f, 520.0f, 750.0f, 100.0f);
        m_solidTiles.emplace_back(1800.0f, 520.0f, 1200.0f, 100.0f);

        // Fortress Boundary Walls
        m_solidTiles.emplace_back(0.0f, 0.0f, 40.0f, 600.0f);
        m_solidTiles.emplace_back(2960.0f, 0.0f, 40.0f, 600.0f);

        // Elevated Castle Platforms (inspired by Captain Claw - Level 1: La Roca)
        m_solidTiles.emplace_back(220.0f, 420.0f, 140.0f, 24.0f);
        m_solidTiles.emplace_back(420.0f, 340.0f, 160.0f, 24.0f);
        m_solidTiles.emplace_back(640.0f, 270.0f, 140.0f, 24.0f);

        m_solidTiles.emplace_back(980.0f, 430.0f, 160.0f, 24.0f);
        m_solidTiles.emplace_back(1200.0f, 350.0f, 160.0f, 24.0f);
        m_solidTiles.emplace_back(1420.0f, 260.0f, 180.0f, 24.0f);

        m_solidTiles.emplace_back(1900.0f, 440.0f, 120.0f, 24.0f);
        m_solidTiles.emplace_back(2100.0f, 360.0f, 120.0f, 24.0f);
        m_solidTiles.emplace_back(2300.0f, 280.0f, 180.0f, 24.0f);
        m_solidTiles.emplace_back(2550.0f, 220.0f, 220.0f, 24.0f);

        // Hazards: Spikes in the pit
        m_hazards.push_back({Rect(800.0f, 580.0f, 100.0f, 40.0f), 100});
        m_hazards.push_back({Rect(1650.0f, 580.0f, 150.0f, 40.0f), 100});

        // Collectibles: Pirate Coins & Rubies
        m_collectibles.push_back({Rect(260.0f, 380.0f, 20.0f, 20.0f), 100, false, false});
        m_collectibles.push_back({Rect(480.0f, 300.0f, 20.0f, 20.0f), 250, false, false});
        m_collectibles.push_back({Rect(700.0f, 230.0f, 20.0f, 20.0f), 500, false, false}); // Red Ruby
        m_collectibles.push_back({Rect(1040.0f, 390.0f, 20.0f, 20.0f), 100, false, false});
        m_collectibles.push_back({Rect(1260.0f, 310.0f, 20.0f, 20.0f), 100, false, false});
        m_collectibles.push_back({Rect(1500.0f, 220.0f, 24.0f, 24.0f), 50, true, false});  // Health Potion
        m_collectibles.push_back({Rect(2350.0f, 240.0f, 20.0f, 20.0f), 500, false, false});
        m_collectibles.push_back({Rect(2650.0f, 180.0f, 20.0f, 20.0f), 1000, false, false}); // Treasure Crown

        // Exit Gate
        m_exitGate = Rect(2850.0f, 440.0f, 60.0f, 80.0f);
    }

    void Level::CheckCollectibles(Player& player) {
        Rect playerBox = player.GetBoundingBox();
        for (auto& item : m_collectibles) {
            if (!item.collected && Collision::CheckAABB(playerBox, item.bounds)) {
                item.collected = true;
                if (item.isHealth) {
                    player.Heal(item.value);
                } else {
                    player.AddScore(item.value);
                }
            }
        }
    }

    bool Level::CheckHazards(const Rect& playerBox) const {
        for (const auto& hazard : m_hazards) {
            if (Collision::CheckAABB(playerBox, hazard.bounds)) {
                return true;
            }
        }
        // Fall into abyss
        if (playerBox.y > 700.0f) {
            return true;
        }
        return false;
    }

} // namespace OpenClaw
