#pragma once

#include "../world/universe.hpp"

template<typename Universe>
class Visulizer {
    public:
    /// @brief Show all the particles on the screen.
    virtual void draw(Universe* universe) const = 0;
};