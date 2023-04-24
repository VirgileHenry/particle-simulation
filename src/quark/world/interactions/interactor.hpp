#pragma once

#include "../../maths/vector.hpp"
#include "../particle.hpp"

/// @brief Virtual class for any way that particles can interact.
/// @tparam D The number of dimensions of the simulation.
template<unsigned int D>
class Interactor {
    public:
    /// @brief Compute the forces that part2 exerce on part1.
    ///         This should be the opposite of computeInteractionForce(part2, part1).
    /// @param part1  The particle on which forces are exerced.
    /// @param part2  The particle exercing the force.
    /// @return the force that part2 exerce on part1.
    virtual Vector<double, D> computeInteractionForce(const Particle<D>& part1, const Particle<D>& part2) = 0;
};