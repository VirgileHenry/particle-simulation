#include "../particle.hpp"
#include <cmath>

static const double epsilon = 1.0;
static const double sigma = 1.0;
static const double sigma_pow_6 = sigma * sigma * sigma * sigma * sigma * sigma;

// todo ; this is not what we are interested in
template<unsigned int D>
double lennard_jones_potential(const Particle<D>& part1, const Particle<D>& part2) {
    double distance_sq = (part1.position - part2.position).sq_magnitude();
    double sigma_over_dist_pow_6 = sigma_pow_6 / (distance_sq * distance_sq * distance_sq);
    return 4 * epsilon * sigma_over_dist_pow_6 * (sigma_over_dist_pow_6 - 1.0);
}