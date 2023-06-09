#pragma once


#include <functional>
#include <random>
#include "../maths/vector.hpp"

/// @brief Particle class.
/// @param D the number of dimensions in which the particle is represented.
template<unsigned int D>
class Particle {
    private:
    /// @brief  last used id of the particles.
    static int last_id;

    private:
    int id; // id for this particle
    Vector<double, D> position; // position of the particle, in 3 dimension space
    Vector<double, D> velocity; // velocity of that particle, in 3 dimension space
    Vector<double, D> force; // force of that particle, in 3 dimension space
    double mass; // mass of that particle. 
    short unsigned int type;


    public:
    /// @brief Create a particle with all params to default.
    Particle() = default;
    /// @brief Creates a particle from the given position generator function.
    Particle(std::function<Vector<double, D>()> positionGenerator) {
        this->id = Particle::last_id++;
        this->position = positionGenerator();
        this->velocity = Vector<double, D>();
        this->force = Vector<double, D>();
        this->mass = 1.0;
    }
    /// @brief Creates a particle with all sets params.
    Particle(Vector<double, D> pos, Vector<double, D> vel, Vector<double, D> force, double mass) {
        this->id = Particle::last_id++;
        this->position = pos;
        this->velocity = vel;
        this->force = force;
        this->mass = mass;
    }

    // getters
    public:
    Vector<double, D> getPosition() const {
        return this->position;
    }

    Vector<double, D> getVelocity() const {
        return this->velocity;
    }

    Vector<double, D> getForce() const {
        return this->force;
    }

    double getMass() const {
        return this->mass;
    }

    // updating methods
    void updateVelocity(Vector<double, D> ammount) {
        this->velocity += ammount;
    }

    void updatePosition(Vector<double, D> ammount) {
        this->position += ammount;
    }

    void resetForce() {
        this->force.reset();
    }

    void addForce(Vector<double, D> force) {
        this->force += force;
    }

};

// static definition and initialization of last particle id
template<unsigned int D> int Particle<D>::last_id = 0; 

