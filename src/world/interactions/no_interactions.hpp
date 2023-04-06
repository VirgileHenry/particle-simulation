#include "interactor.hpp"


template<unsigned int D>
class NoInteractions : Interactor<D> {
    public:
    NoInteractions() = default;
    public:
    Vector<double, D> computeInteractionForce(const Particle<D>& part1, const Particle<D>& part2) {
        // no interaction, return null vector
        return Vector<double, D>();
    }
};