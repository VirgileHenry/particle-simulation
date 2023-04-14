#pragma once

#include "interactor.hpp"


template<unsigned int D>
class GravityInteractor : public Interactor<D> {
    public:
    GravityInteractor() = default;
    public:
    Vector<double, D> computeInteractionForce(const Particle<D>& part1, const Particle<D>& part2) {
        // gravity interaction is : 
        // F = m1 m2 * r12 / || r12 ||^3
        double F = part1.getMass() * part2.getMass() / pow((part2.getPosition() - part1.getPosition()).sq_magnitude(), 3/2);
        return (part2.getPosition() - part1.getPosition()) * F;
    }
};