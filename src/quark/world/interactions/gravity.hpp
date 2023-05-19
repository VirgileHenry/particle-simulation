#pragma once

#include "interactor.hpp"


template<unsigned int D>
class GravityInteractor : public Interactor<D> {
    private:
    constexpr static const double G = 0.0000000000667430;
    public:
    GravityInteractor() = default;
    public:
    Vector<double, D> computeInteractionForce(const Particle<D>& part1, const Particle<D>& part2) {
        // gravity interaction is : 
        // F = m1 m2 * r12 / || r12 ||^3
        double distance_cubed = pow((part2.getPosition() - part1.getPosition()).sq_magnitude(), 3/2);
        double F = this->G * part1.getMass() * part2.getMass() / distance_cubed;
        return (part2.getPosition() - part1.getPosition()) * F;
    }
};