#pragma once

#include "../world/universe.hpp"

template<typename Universe>
class Visualizer {
    public:
    /// @brief Show all the particles on the screen.
    virtual void draw(Universe* universe) = 0;
};