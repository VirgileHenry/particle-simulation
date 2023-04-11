#pragma once

#include <array>
#include <list>
#include <random>
#include <algorithm>
#include <unordered_map>
#include "../maths/vector.hpp"
#include "particle.hpp"
#include "universe_chunk.hpp"
#include "interactions/interactor.hpp"
#include "../visualizer/visulizer.hpp"

/// @brief Universe class.
/// @tparam D the number of dimensions of the universe.
/// @tparam N the number of particles in the universe.
template<unsigned int D, unsigned int N, typename T>
class Universe {
    private:
    // interactors and visulizers
    T interactor;
    std::list<Visulizer<Universe<D, N, T>>*> registered_visulizer;

    const double ld; // longueur caractéristique
    const double rcut; // rayon de coupe
    std::array<Particle<D>, N> particles;
    std::unordered_map<Vector<unsigned int, D>, UniverseChunk<D>> chunks;

    // getters and setters
    public:
    std::array<Particle<D>, N> getParticles(){
        return this->particles;
    }

    public:
    void step(double deltaTime);
    void registerVisulizer(Visulizer<Universe<D, N, T>> *visulizer);

    private:
    void updateParticleForces(double deltaTime);    
    void updateParticleVel(double deltaTime);
    void updateParticlePos(double deltaTime);

    public:
    Universe() = delete;    
    Universe(T interactor, double ld, double rcut): interactor(interactor), ld(ld), rcut(rcut) {
        this->particles = std::array<Particle<D>, N>();

        this->chunks = std::unordered_map<Vector<unsigned int, D>, UniverseChunk<D>>();
        // Create all the chunks ! (have been tested and works properly)
        unsigned int chunk_length = (int)std::ceil(this->ld / this->rcut);
        unsigned int chunk_count = std::pow(chunk_length, D);
        // we got the total chunk count !
        for(unsigned int chunk_index = 0; chunk_index < chunk_count; chunk_index++) {
            // create the vector from that index
            unsigned int current_index = chunk_index;
            Vector<unsigned int, D> coordinates;
            for(unsigned int dim = 0; dim < D; dim++) {
                // get current dimension index
                coordinates[dim] = current_index % chunk_length;
                // "squash" that dimension down to get next one
                current_index = (current_index - coordinates[dim]) / chunk_length;
            }
            // create a chunk at that coordinates
            this->chunks.insert({ Vector<unsigned int, D>(coordinates), UniverseChunk<D>(this->rcut) });
        }

        // create one random generator for all the particles, that the closure will capture.
        // this allow to avoid creating a random object for each particle
        std::default_random_engine rnd{std::random_device{}()};
        std::uniform_real_distribution<double> dist(0.0, 1.0);
        // fill the partcle vec with new particles, generated from a random position
        std::ranges::generate(this->particles, [&]() {
            return Particle<D>([&]() {
                return Vector<double, D>([&]() {
                    return dist(rnd);
                });
            });
        });

        // fill our chunks with the particles
        for(unsigned int i = 0; i < N; i++) {
            // get the chunk of i particle, put it in it
            Vector<unsigned int, D> coordinates;
            for(unsigned int coord = 0; coord < D; coord++) {
                coordinates[coord] = (unsigned int)std::floor(this->particles[i].getPosition()[coord] / this->rcut);
            }
            this->chunks.at(coordinates).addParticle(i);
        }
    }
};


template<unsigned int D, unsigned int N, typename T>
void Universe<D, N, T>::step(double deltaTime) {
    // compute the forces on all particles
    this->updateParticleForces(deltaTime);
    // update particles velocity
    this->updateParticleVel(deltaTime);
    // update particles positions
    this->updateParticlePos(deltaTime);
    // trigger all visulizers
    for(Visulizer<Universe<D, N, T>> *visulizer: this->registered_visulizer) {
        visulizer->draw(this);
    }
}

template<unsigned int D, unsigned int N, typename T>
void Universe<D, N, T>::updateParticleForces(double deltaTime) {
    // reset all the forces to zero
    for(auto particle: this->particles) {
        particle.resetForce();
    }
    // compute all new forces

    // temporary to make work the interactor
    for(unsigned int i = 0; i < N; i++) {
        for(unsigned int j = 0; j < i; j++) {
            // this is the force that j particle exerce on i particule.
            Vector<double, D> force = this->interactor.computeInteractionForce(this->particles[i], this->particles[j]);
            // maybe don't do this if forces are not symmetrical ?
            this->particles[i].addForce(force);
            this->particles[j].addForce(-force);
        }
    }
}

template<unsigned int D, unsigned int N, typename T>
void Universe<D, N, T>::updateParticleVel(double deltaTime) {
    // loop through every particle, and update velocity with force
    for(auto particle: this->particles) {
        particle.updateVelocity(deltaTime);
    }
}

template<unsigned int D, unsigned int N, typename T>
void Universe<D, N, T>::updateParticlePos(double deltaTime) {
    // loop over every particle, and update pos by velocity
    for(auto particle: this->particles) {
        particle.updatePosition(deltaTime);
    }
}

template<unsigned int D, unsigned int N, typename T>
void Universe<D, N, T>::registerVisulizer(Visulizer<Universe<D, N, T>> *visulizer) {
    this->registered_visulizer.push_back(visulizer);
}