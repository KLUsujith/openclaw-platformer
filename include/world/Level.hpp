#pragma once

#include "core/Types.hpp"
#include <vector>

namespace OpenClaw {

    class Player;

    struct Collectible {
        Rect bounds;
        int value{100};
        bool isHealth{false};
        bool collected{false};
    };

    struct Hazard {
        Rect bounds;
        int damage{100};
    };

    class Level {
    public:
        Level();

        void LoadLevel1();
        void CheckCollectibles(Player& player);
        bool CheckHazards(const Rect& playerBox) const;

        const std::vector<Rect>& GetSolidTiles() const { return m_solidTiles; }
        const std::vector<Collectible>& GetCollectibles() const { return m_collectibles; }
        const std::vector<Hazard>& GetHazards() const { return m_hazards; }
        Rect GetExitGate() const { return m_exitGate; }

        float GetWidth() const { return m_width; }
        float GetHeight() const { return m_height; }

    private:
        float m_width{3200.0f};
        float m_height{800.0f};
        std::vector<Rect> m_solidTiles;
        std::vector<Collectible> m_collectibles;
        std::vector<Hazard> m_hazards;
        Rect m_exitGate;
    };

} // namespace OpenClaw
