#pragma once

#include "../../maths/vector.hpp"
#include "../particle.hpp"

/// @brief Virtual classes to apply general forces to the simulation.
template<unsigned int D>
class Force {
    public:
    /// @brief Apply a force to a particle.
    /// @param part The particle to apply forces to.
    /// @return the applied force.
    virtual Vector<double, D> computeForce(const Particle<D>& part) = 0;
};