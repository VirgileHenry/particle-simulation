#pragma once

#include "../../maths/vector.hpp"
#include "../particle.hpp"

template<unsigned int D>
class Interactor {
    public:
    virtual Vector<double, D> computeInteractionForce(const Particle<D>& part1, const Particle<D>& part2) = 0;
};