#pragma once

#include <array>
#include <list>
#include <random>
#include <algorithm>
#include <unordered_map>
#include "../maths/vector.hpp"
#include "../maths/const_pow.hpp"
#include "../maths/const_div.hpp"
#include "particle.hpp"
#include "universe_chunk.hpp"
#include "interactions/interactor.hpp"
#include "forces/forces.hpp"
#include "../visualizer/visualizer.hpp"

enum BORDER_TYPE {
    absorbent, // default
    reflexive,
    periodic,
}; 

/// @brief Universe class.
/// @tparam D the number of dimensions of the universe.
/// @tparam N the number of particles in the universe.
/// @tparam LD the size of the universe cube
/// @tparam RCUT at what distance can we stop interactions
template<unsigned int D, unsigned int N, double LD, double RCUT>
class Universe {
    private:
    constexpr static unsigned int C = const_div(LD, RCUT);
    constexpr static unsigned int CHUNK_LENGTH = const_pow(C, D);
    constexpr static unsigned int CHUNK_IT_LENGTH = const_pow(3, D);
    // interactors and visulizers
    std::list<Interactor<D>*> registered_interactors;
    std::list<Force<D>*> registered_forces;
    std::list<Visualizer<Universe<D, N, LD, RCUT>>*> registered_visulizer;
    BORDER_TYPE border = BORDER_TYPE::absorbent;

    std::array<Particle<D>, N> particles;
    UniverseChunk<D> chunks[CHUNK_LENGTH];

    // created once for optimisation, allows to iterate over nearby chunks
    int chunk_proxy_it[CHUNK_IT_LENGTH];

    // target cinetic energy 
    bool restrain_cinetic_energy = false;
    unsigned int restrain_ce_counter = 1000;
    double Ecd = 1.;

    // getters and setters
    public:
    std::array<Particle<D>, N> getParticles(){
        return this->particles;
    }

    public:
    void step(double deltaTime);
    void registerInteractor(Interactor<D> *interactor);
    void registerForce(Force<D> *force);
    void registerVisualizer(Visualizer<Universe<D, N, LD, RCUT>> *visualizer);
    void restrainCineticEnergy(double target_energy) {
        restrain_cinetic_energy = true;
        Ecd = target_energy;
    }
    void set_border_type(BORDER_TYPE border) {
        this->border = border;
    }

    private:
    void updateParticleForces();
    void stromerVerletUpdate(double deltaTime);
    void verifyParticlesChunks();
    void targetCineticEnergy();

    public:
    /// @brief Creates a universe with random particles in the [0x1]^D hyper cube.
    /// @param interactor interactions for this universe
    /// @param ld caracteristic length of the simulation
    /// @param rcut max interactor distance
    Universe() {
        this->particles = std::array<Particle<D>, N>();

        // generate the chunks
        this->generateChunks();

        // create one random generator for all the particles, that the closure will capture.
        // this allow to avoid creating a random object for each particle
        std::default_random_engine rnd{std::random_device{}()};
        std::uniform_real_distribution<double> dist(0.0, 1.0);
        // fill the particle vec with new particles, generated from a random position
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
    Universe(Particle<D> particles[N]) {
        // from https://cplusplus.com/forum/beginner/200574/
        std::copy(particles, particles + N, this->particles.begin());

        this->generateChunks();
        this->populateChunks();
        this->generateChunkProxyIt();
    }

    private:
    // private funcs used for init
    void generateChunks();
    void populateChunks();
    void placeParticle(unsigned int particle_index, unsigned  int chunk_index);
    void generateChunkProxyIt();

    private:
    // utility
    int vecCoordToInt(Vector<int, D> vec);
    Vector<int, D> intCoordToVec(int coord);
    unsigned int getParticleChunk(unsigned int part);
};

/// @brief Generates the chunks for our universe.
/// @tparam D The number of dimensions of the Universe.
/// @tparam N The number of particles in the universe.
/// @tparam C The number of chunks in all dimensions of the universe.
template<unsigned int D, unsigned int N, double LD, double RCUT>
void Universe<D, N, LD, RCUT>::generateChunks() {
    // generate every chunk
    for(unsigned int chunk_index = 0; chunk_index < this->CHUNK_LENGTH; chunk_index++) {
        // create the vector from that index
        Vector<int, D> coordinates = this->intCoordToVec(chunk_index);
        // create a chunk at that coordinates
        this->chunks[chunk_index] = UniverseChunk<D>(coordinates);
    }
}

/// @brief Places all the particle in their respective chunks.
/// @tparam D The number of dimensions of the Universe.
/// @tparam N The number of particles in the universe.
/// @tparam C The number of chunks in all dimensions of the universe.
template<unsigned int D, unsigned int N, double LD, double RCUT>
void Universe<D, N, LD, RCUT>::populateChunks() {
    // fill our chunks with the particles
    for(unsigned int i = 0; i < N; i++) {
        // get the chunk of i particle, put it in it
        int part_chunk = this->getParticleChunk(i);
        if(part_chunk < 0) {
            continue; // particle is outside of chunks range
        }
        this->placeParticle(i, part_chunk);
    }
    // flush all chunks
    for(unsigned int chunk = 0; chunk < this->CHUNK_LENGTH; chunk++) {
        this->chunks[chunk].flush();
    }
}

template<unsigned int D, unsigned int N, double LD, double RCUT>
void Universe<D, N, LD, RCUT>::placeParticle(unsigned int particle_index, unsigned  int chunk_index) {
    this->chunks[chunk_index].addParticle(particle_index);
}

template<unsigned int D, unsigned int N, double LD, double RCUT>
unsigned int Universe<D, N, LD, RCUT>::getParticleChunk(unsigned int part) {
    // get the chunk of i particle
    Vector<double, D> pos = this->particles[part].getPosition();
    int result = 0;
    for(unsigned int i = 0; i < D; i++) {
        switch(this->border) {
            case BORDER_TYPE::absorbent:
                if(pos[i] < 0 || pos[i] >= LD) {
                    return -1; // forget this particle by not replacing it
                }
                break;
            case BORDER_TYPE::periodic:
                // while we are not in that range, replace it in the range
                while(pos[i] < 0) {
                    pos[i] += LD;
                }
                while(pos[i] >= LD) {
                    pos[i] -= LD;
                }
                break;
            case BORDER_TYPE::reflexive:
                // this will be handled by the apply force func.
                // experimentally, this does not work alone. let's add a hard switch to keep them in bound.
                break;
                if(pos[i] < 0) {
                    pos[i] = 0.0001;
                }
                else if(pos[i] >= LD) {
                    pos[i] = LD - 0.0001; // forget this particle by not replacing it
                }
                break;
        }
        result *= this->C;
        result += std::max(0, std::min((int)floor(pos[i] / RCUT), (int)this->C - 1));
    }
    return result;
}

/// @brief Generate an array of index offset. These offset represent the nearby chunks.
///         This allow quick iteration over chunks.
/// @tparam D dimension of the universe
/// @tparam N Number of particles 
/// @tparam LD size of the universe
/// @tparam RCUT max distance interaction
template<unsigned int D, unsigned int N, double LD, double RCUT>
void Universe<D, N, LD, RCUT>::generateChunkProxyIt() {
    // ? opti : this could be optimized ? as we are computing index to vec to index ? maybe faster way ?
    // not too worried about optimizing this, as it runs once at the creation of the universe
    // create the proxy chunk iterator
    for(unsigned int chunk_index = 0; chunk_index < this->CHUNK_IT_LENGTH; chunk_index++) {
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
        this->chunk_proxy_it[chunk_index] = this->vecCoordToInt(coordinates);
    }
}

template<unsigned int D, unsigned int N, double LD, double RCUT>
int Universe<D, N, LD, RCUT>::vecCoordToInt(Vector<int, D> vec) {
    int result = vec[0];
    for(unsigned int i = 1; i < D; i++) {
        result *= C;
        result += vec[i];
    }
    return result;
}

template<unsigned int D, unsigned int N, double LD, double RCUT>
Vector<int, D> Universe<D, N, LD, RCUT>::intCoordToVec(int coord) {
    Vector<int, D> result;
    for(unsigned int dim = 0; dim < D; dim++) {
        // get current dimension index
        result[dim] = coord % C;
        // "squash" that dimension down to get next one
        coord = (coord - result[dim]) / C;
    }
    return result;
}

template<unsigned int D, unsigned int N, double LD, double RCUT>
void Universe<D, N, LD, RCUT>::step(double deltaTime) {
    // compute the forces on all particles
    this->stromerVerletUpdate(deltaTime);

    // replace each particle in its chunk
    this->verifyParticlesChunks();

    // call each visulizer
    for(Visualizer<Universe<D, N, LD, RCUT>> *visulizer: this->registered_visulizer) {
        visulizer->draw(this);
    }

    // restrain target energy
    if(this->restrain_cinetic_energy) {
        this->restrain_ce_counter--;
        if(this->restrain_ce_counter <= 0) {
            this->restrain_ce_counter = 1000;
            this->targetCineticEnergy();
        }
    }
}

template<unsigned int D, unsigned int N, double LD, double RCUT>
void Universe<D, N, LD, RCUT>::updateParticleForces() {
    // reset all the forces to zero
    for(unsigned int i = 0; i < N; i++) {
        this->particles[i].resetForce();
    }
    
    // update particles force, taking into account the chunks
    // loop over every chunk, update every particle in that chunk
    for(unsigned int chunk = 0; chunk < this->CHUNK_LENGTH; chunk++) {
        for(auto part_i = this->chunks[chunk].getParticleBegin(); part_i != this->chunks[chunk].getParticleEnd(); ++part_i) {
            // update particle at index part_i
            // iterate over every nearby chunk
            for(unsigned int i = 0; i < this->CHUNK_IT_LENGTH; i++) {
                // chunk at chunk + offset may not exist
                if(0 <= chunk + chunk_proxy_it[i] && chunk + chunk_proxy_it[i] < (int)this->CHUNK_LENGTH) {
                    // loop over every particle of the chunk to compute force with
                    for(
                        auto part_j = this->chunks[chunk + chunk_proxy_it[i]].getParticleBegin();
                        part_j != this->chunks[chunk + chunk_proxy_it[i]].getParticleEnd();
                        ++part_j
                    ) {
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

    // also iterate over all unique forces
    for(unsigned int chunk = 0; chunk < this->CHUNK_LENGTH; chunk++) {
        for(auto part_i = this->chunks[chunk].getParticleBegin(); part_i != this->chunks[chunk].getParticleEnd(); ++part_i) {
            // update particle at index part_i
            // iterate over every nearby chunk
            for(Force<D> *force: this->registered_forces) {
                Vector<double, D> f = force->computeForce(this->particles[*part_i]);
                this->particles[*part_i].addForce(f);
            }
        }
    }

    // if the border type is set to relfexive, apply force to simulate this
    if(this->border == BORDER_TYPE::reflexive) {
        
        for(unsigned int chunk = 0; chunk < this->CHUNK_LENGTH; chunk++) {
            for(auto part_i = this->chunks[chunk].getParticleBegin(); part_i != this->chunks[chunk].getParticleEnd(); ++part_i) {
                // update particle at index part_i
                // iterate over every nearby chunk
                Vector<double, D> border_force = Vector<double, D>();
                Vector<double, D> pos = this->particles[*part_i].getPosition();
                for(unsigned int i = 0; i < D; i++) {
                    if(pos[i] < 1.1224) { // rcut is 2^(1/6)
                        double r = pos[i];
                        // here, eps = 1; sigma = 1.
                        // maybe add a nice way to do this in the future, but all examples have those values
                        // and we need to sync them with the different values of the different interactors... ugggh
                        double r_two_sixth = pow(2 * r, 6); // sounds like a star wars droid name
                        border_force [i]= 24 / (2 * r) / r_two_sixth * (1 - 2 / r_two_sixth);
                    }
                    else if(pos[i] >= LD - 1.1224) { // rcut is 2^(1/6)
                        double r = LD - pos[i];
                        // here, eps = 1; sigma = 1.
                        // maybe a nice way to do this in the future, but all examples have those values
                        // and we need to sync them with the different values of the different interactors... ugggh
                        double r_two_sixth = pow(2 * r, 6); // sounds like a star wars droid name
                        border_force[i] = -24 / (2 * r) / r_two_sixth * (1 - 2 / r_two_sixth);
                    }
                }
                this->particles[*part_i].addForce(border_force);
            }
        }
    }
}

template<unsigned int D, unsigned int N, double LD, double RCUT>
void Universe<D, N, LD, RCUT>::stromerVerletUpdate(double deltaTime) {
    // one step of the stromer verlet algorithm
    // update all particles, and store previous forces
    // is it ok to create this size array on the stack every frame ?
    Vector<double, D> f_old[N];
    // first update of stromer verlet
    for(unsigned int i = 0; i < N; i++) {
        this->particles[i].updatePosition(
            (this->particles[i].getVelocity() + this->particles[i].getForce() * 0.5 * deltaTime / (this->particles[i].getMass())) * deltaTime
        );
        f_old[i] = this->particles[i].getForce();
    }
    // compute new forces
    this->updateParticleForces();
    // second update of Stromer Verlet
    for(unsigned int i = 0; i < N; i++) {
        this->particles[i].updateVelocity(
            (this->particles[i].getForce() + f_old[i] * 0.5 / (this->particles[i].getMass())) * deltaTime
        );
    }
}

template<unsigned int D, unsigned int N, double LD, double RCUT>
void Universe<D, N, LD, RCUT>::targetCineticEnergy() {
    // compute beta
    double beta = 0;
    for(unsigned i = 0; i < N; i++) {
        beta += this->particles[i].getMass() * this->particles[i].getVelocity().sq_magnitude();
    }
    beta = sqrt(this->Ecd / (beta * 0.5));
    // compute 
}

template<unsigned int D, unsigned int N, double LD, double RCUT>
void Universe<D, N, LD, RCUT>::verifyParticlesChunks() {
    // loop through all chunks, all particles, check they are in the right chunk.
    for(unsigned int chunk = 0; chunk < this->CHUNK_LENGTH; chunk++) {
        for(auto part = this->chunks[chunk].getParticleBegin(); part != this->chunks[chunk].getParticleEnd(); ++part) {
            // check the particle is placed in the write spot in all dimensions
            unsigned int part_chunk = this->getParticleChunk(*part);
            if(chunk != part_chunk) {
                // -1 means do not replace the particle
                if(part_chunk >= 0) {
                    this->placeParticle(*part, part_chunk);
                }
                this->chunks[chunk].removeParticle(*part);
                break;
            }
        }
    }
    // flush all chunks
    for(unsigned int chunk = 0; chunk < this->CHUNK_LENGTH; chunk++) {
        this->chunks[chunk].flush();
    }
}


template<unsigned int D, unsigned int N, double LD, double RCUT>
void Universe<D, N, LD, RCUT>::registerInteractor(Interactor<D> *interactor) {
    this->registered_interactors.push_back(interactor);
}

template<unsigned int D, unsigned int N, double LD, double RCUT>
void Universe<D, N, LD, RCUT>::registerForce(Force<D> *force) {
    this->registered_forces.push_back(force);
}

template<unsigned int D, unsigned int N, double LD, double RCUT>
void Universe<D, N, LD, RCUT>::registerVisualizer(Visualizer<Universe<D, N, LD, RCUT>> *visualizer) {
    this->registered_visulizer.push_back(visualizer);
}