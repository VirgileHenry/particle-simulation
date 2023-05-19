#pragma once

#include "forces.hpp"


template<unsigned int D>
class GravityForce : public Force<D> {
    private:
    const double G;
    public:
    GravityForce(double g) : G(g) {};
    public:
    Vector<double, D> computeForce(const Particle<D>& part) {
        // How to model gravity in D dimensions ? 
        if(D <= 1) {
            return Vector<double, D>();
        }
        else {
            Vector<double, D> result = Vector<double, D>();
            result[1] = -G * part.getMass(); // negative because the y axis is going down
            return result;
        }
    }
};