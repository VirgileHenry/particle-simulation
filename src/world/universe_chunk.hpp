#pragma once

#include <unordered_set>
#include "particle.hpp"
#include "../maths/vector.hpp"

/// @brief A chunk of the universe.
///         Seeing the state of it, it's mostly a wrapper around a collection of particle indexes.
/// @tparam D The dimension of the universe the chunk is in.
class UniverseChunk {
    private:
    std::unordered_set<unsigned int> particles_index;
    std::unordered_set<unsigned int> invalid_particles;
    std::unordered_set<unsigned int> incoming_particles;

    public:
    UniverseChunk() {
        this->particles_index = std::unordered_set<unsigned int>();
        this->incoming_particles = std::unordered_set<unsigned int>();
    }

    public:
    inline void addParticle(unsigned int particle) {
        this->incoming_particles.insert(particle);
    }

    inline void removeParticle(unsigned int particle) {
        this->invalid_particles.insert(particle);
    }

    inline bool containsParticle(unsigned int particle) {
        return this->particles_index.contains(particle);
    }

    inline auto getParticleBegin() {
        return this->particles_index.begin();
    }

    inline auto getParticleEnd() {
        return this->particles_index.end();
    }

    inline auto getParticleNumber() {
        return this->particles_index.size();
    }

    /// @brief Clean the chunk collections.
    ///         The multiple collections allow deletion and insertion while iteration.
    void flush() {
        for(auto part = this->invalid_particles.begin(); part != this->particles_index.end(); ++part) {
            this->particles_index.erase(*part);
        }
        // for some reason next line does not work ? so we have to iterate manually
        // this->particles_index.erase(this->invalid_particles.begin(), this->invalid_particles.end());
        this->invalid_particles.clear();
        this->particles_index.insert(this->incoming_particles.begin(), this->incoming_particles.end());
        this->incoming_particles.clear();
    }

};