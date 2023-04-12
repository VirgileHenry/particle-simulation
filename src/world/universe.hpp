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
    void updateParticleForces();
    void stromerVerletUpdate(double deltaTime);

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
        std::generate(this->particles.begin(), this->particles.end(), [&]() {
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
    this->stromerVerletUpdate(deltaTime);
    for(Visulizer<Universe<D, N, T>> *visulizer: this->registered_visulizer) {
        visulizer->draw(this);
    }
}

template<unsigned int D, unsigned int N, typename T>
void Universe<D, N, T>::updateParticleForces() {
    // reset all the forces to zero
    for(unsigned int i = 0; i < N; i++) {
        this->particles[i].resetForce();
    }
    // compute all new forces

    // we can start at 1, as the first particle would not compute with anyone
    for(unsigned int i = 1; i < N; i++) {
        // todo : loop only on particles on nearby chunks
        for(unsigned int j = 0; j < i; j++) {
            // this is the force that j particle exerce on i particule.
            Vector<double, D> force = this->interactor.computeInteractionForce(this->particles[i], this->particles[j]);
            this->particles[i].addForce(force);
            this->particles[j].addForce(-force);
        }
    }
}

template<unsigned int D, unsigned int N, typename T>
void Universe<D, N, T>::stromerVerletUpdate(double deltaTime) {
    // one step of the stromer verlet algorithm
    // update all particles, and store previous forces
    // is it ok to create this size array on the stack every frame ?
    Vector<double, D> f_old[N];
    // first update of stromer verlet
    for(unsigned int i = 0; i < N; i++) {
        this->particles[i].updatePosition(
            (this->particles[i].getVelocity() + this->particles[i].getForce() * 0.5 / (this->particles[i].getMass())) * deltaTime
        );
        f_old[i] = this->particles[i].getForce();
    }
    // compute new forces
    this->updateParticleForces();
    // second update of stromer verlet
    for(unsigned int i = 0; i < N; i++) {
        this->particles[i].updateVelocity(
            (this->particles[i].getForce() + f_old[i] * 0.5 / (this->particles[i].getMass())) * deltaTime
        );
    }
}

template<unsigned int D, unsigned int N, typename T>
void Universe<D, N, T>::registerVisulizer(Visulizer<Universe<D, N, T>> *visulizer) {
    this->registered_visulizer.push_back(visulizer);
}