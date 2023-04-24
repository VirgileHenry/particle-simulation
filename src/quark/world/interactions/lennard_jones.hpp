#pragma once

#include "interactor.hpp"


template<unsigned int D>
class LennardJonesInteractor : public Interactor<D> {

    const double sigma = 1.0;
    const double epsilon = 1.0;
    const double sigma_sixth = sigma * sigma * sigma * sigma * sigma * sigma;
    const double epsilon_24 = epsilon * 24;

    /// @brief Compute the force that part2 exerce on part1, from the Lennard-Jones potential.
    /// @param part1 The particle on which the force is applied.
    /// @param part2 The particle applying the force.
    /// @return The computed force.
    Vector<double, D> computeInteractionForce(const Particle<D>& part1, const Particle<D>& part2) {
        Vector<double, D> rij = part2.getPosition() - part1.getPosition();
        double distance_sq = rij.sq_magnitude();
        double sigma_over_distance_sixth = sigma_sixth / (distance_sq * distance_sq * distance_sq);
        return rij * (epsilon_24 / distance_sq * sigma_over_distance_sixth * (1 - 2 * sigma_over_distance_sixth));
    }

};