#pragma once

#include <array>
#include <list>
#include <random>
#include <algorithm>
#include <unordered_map>
#include "../maths/vector.hpp"
#include "../maths/const_pow.hpp"
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
    std::list<Interactor<D>*> registered_interactors;
    std::list<Visulizer<Universe<D, N, T>>*> registered_visulizer;

    const double ld; // longueur caractéristique
    const double rcut; // rayon de coupe
    std::array<Particle<D>, N> particles;
    // got to go with ints, because the nearby iterator is with int and we need to do index maths
    std::unordered_map<Vector<int, D>, UniverseChunk> chunks;

    // created once for optimisation, allows to iterate over nearby chunks
    Vector<int, D> chunk_proxy_it[const_pow(3, D)];

    // getters and setters
    public:
    std::array<Particle<D>, N> getParticles(){
        return this->particles;
    }

    public:
    void step(double deltaTime);
    void registerInteractor(Interactor<D> *interactor);
    void registerVisulizer(Visulizer<Universe<D, N, T>> *visulizer);

    private:
    void updateParticleForces();
    void stromerVerletUpdate(double deltaTime);
    void verifyParticlesChunks();

    public:
    Universe() = delete;
    /// @brief Creates a universe with random particles in the [0x1]^D hyper cube.
    /// @param interactor interactions for this universe
    /// @param ld caracteristic length of the simulation
    /// @param rcut max interactor distance
    Universe(double ld, double rcut): ld(ld), rcut(rcut) {
        this->particles = std::array<Particle<D>, N>();

        // generate the chunks
        this->generateChunks();

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

        // put all the particles in the corresponding chunks
        this->populateChunks();
        // generate the chunk nearby iterator
        this->generateChunkProxyIt();
    }
    /// @brief Creates a universe with given particles. 
    /// @param interactor the interactor for this universe.
    /// @param ld caracteristic length
    /// @param rcut max intercation distance
    /// @param particles the particles to populate the universe with
    Universe(double ld, double rcut, Particle<D> particles[N]): ld(ld), rcut(rcut) {
        this->particles = particles;

        this->generateChunks();
        this->populateChunks();
        this->generateChunkProxyIt();
    }

    private:
    // private funcs used for init
    void generateChunks();
    void populateChunks();
    void placeParticle(unsigned int particle_index);
    void generateChunkProxyIt();
};


template<unsigned int D, unsigned int N, typename T>
void Universe<D, N, T>::generateChunks() {
    // if we now at compile time the size and rcut of our universe, we could in theory build this as an array at compile time.
    // then, our chunk proxy iterator could be an array of ints, giving offsets on the chunks array.

    this->chunks = std::unordered_map<Vector<int, D>, UniverseChunk>();
    // Create all the chunks ! (have been tested and works properly)
    unsigned int chunk_length = (int)std::ceil(this->ld / this->rcut);
    unsigned int chunk_count = std::pow(chunk_length, D);
    // we got the total chunk count !
    for(unsigned int chunk_index = 0; chunk_index < chunk_count; chunk_index++) {
        // create the vector from that index
        unsigned int current_index = chunk_index;
        Vector<int, D> coordinates;
        for(unsigned int dim = 0; dim < D; dim++) {
            // get current dimension index
            coordinates[dim] = current_index % chunk_length;
            // "squash" that dimension down to get next one
            current_index = (current_index - coordinates[dim]) / chunk_length;
        }
        // create a chunk at that coordinates
        this->chunks.insert({ coordinates, UniverseChunk() });
    }
}

template<unsigned int D, unsigned int N, typename T>
void Universe<D, N, T>::populateChunks() {
    // fill our chunks with the particles
    for(unsigned int i = 0; i < N; i++) {
        // get the chunk of i particle, put it in it
        this->placeParticle(i);
    }
    // flush all chunks
    for(auto chunk = this->chunks.begin(); chunk != this->chunks.end(); chunk++) {
        chunk->second.flush();
    }
}

template<unsigned int D, unsigned int N, typename T>
void Universe<D, N, T>::placeParticle(unsigned int particle_index) {
    // get the chunk of i particle, put it in it
    Vector<int, D> coordinates;
    for(unsigned int coord = 0; coord < D; coord++) {
        int part_pos = (int)std::floor(this->particles[particle_index].getPosition()[coord] / this->rcut);
        int chunk_count = (int)std::ceil(this->ld / this->rcut);
        part_pos = std::max(0, std::min(chunk_count - 1, part_pos));
        coordinates[coord] = part_pos;
    }
    this->chunks.at(coordinates).addParticle(particle_index);
}

template<unsigned int D, unsigned int N, typename T>
void Universe<D, N, T>::generateChunkProxyIt() {
    // create the proxy chunk iterator
    for(unsigned int chunk_index = 0; chunk_index < const_pow(3, D); chunk_index++) {
        // create the vector from that index. Basically same code than for all chunks 
        unsigned int current_index = chunk_index;
        Vector<int, D> coordinates;
        for(unsigned int dim = 0; dim < D; dim++) {
            // get current dimension index
            coordinates[dim] = current_index % 3 - 1;
            // "squash" that dimension down to get next one
            current_index = (current_index - coordinates[dim]) / 3;
        }
        // create a chunk at that coordinates
        this->chunk_proxy_it[chunk_index] = coordinates;
    }
}


template<unsigned int D, unsigned int N, typename T>
void Universe<D, N, T>::step(double deltaTime) {
    // compute the forces on all particles
    this->stromerVerletUpdate(deltaTime);

    // replace each particle in it's chunk
    this->verifyParticlesChunks();

    // call each visulizer
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
    
    // update particles force, taking into account the chunks
    // loop over every chunk, update every particle in that chunk
    for(auto chunk = this->chunks.begin(); chunk != this->chunks.end(); chunk++) {
        for(auto part_i = chunk->second.getParticleBegin(); part_i != chunk->second.getParticleEnd(); ++part_i) {
            // update particle at index part_i
            // iterate over every nearby chunk
            for(unsigned int i = 0; i < const_pow(3, D); i++) {
                // chunk at chunk + offset may not exist
                if(this->chunks.contains(chunk->first + this->chunk_proxy_it[i])) {
                    // loop over every particle of the chunk to compute force with
                    UniverseChunk lookup_chunk = this->chunks.at(chunk->first + this->chunk_proxy_it[i]);
                    for(auto part_j = lookup_chunk.getParticleBegin(); part_j != lookup_chunk.getParticleEnd(); ++part_j) {
                        // only compute forces if j < i: this allows to divide calcs per 2, and don't compute with ourselves
                        if(*part_i <= *part_j) {
                            continue;
                        }
                        // compute force that part j apply on part i
                        Vector<double, D> force = Vector<double, D>();
                        for(Interactor<D> *interactor: this->registered_interactors) {
                            force += interactor->computeInteractionForce(this->particles[*part_i], this->particles[*part_j]);
                        }
                        this->particles[*part_i].addForce(force);
                        this->particles[*part_j].addForce(-force);
                    }
                }
            }
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
void Universe<D, N, T>::verifyParticlesChunks() {
    // loop through all chunks, all particles, check they are in the right chunk.
    for(auto chunk = this->chunks.begin(); chunk != this->chunks.end(); chunk++) {
        for(auto part = chunk->second.getParticleBegin(); part != chunk->second.getParticleEnd(); ++part) {
            // check the particle is writly placed in all dimensions
            bool correct_place = true;
            for(unsigned int coord = 0; coord < D; coord++) {
                double particle_pos = std::max(0.0, std::min(this->ld, this->particles[*part].getPosition()[coord]));
                correct_place &= (chunk->first[coord] * rcut <= particle_pos && particle_pos < (chunk->first[coord] + 1) * rcut);
                // we can put the switching chunk code here, it's a slight optim
                if(!correct_place) {
                    // the particle is out of it's chunk, move it
                    chunk->second.removeParticle(*part);
                    this->placeParticle(*part);
                    break;
                }
            }
        }
    }
    // flush all chunks
    for(auto chunk = this->chunks.begin(); chunk != this->chunks.end(); chunk++) {
        chunk->second.flush();
    }
}


template<unsigned int D, unsigned int N, typename T>
void Universe<D, N, T>::registerInteractor(Interactor<D> *interactor) {
    this->registered_interactors.push_back(interactor);
}

template<unsigned int D, unsigned int N, typename T>
void Universe<D, N, T>::registerVisulizer(Visulizer<Universe<D, N, T>> *visulizer) {
    this->registered_visulizer.push_back(visulizer);
}