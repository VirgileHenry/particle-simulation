#pragma once

#include <vector>
#include "particle.hpp"
#include "../maths/vector.hpp"

/// @brief A chunk of the universe. 
/// @tparam D The dimension of the universe the chunk is in.
template<unsigned int D>
class UniverseChunk {
    private:
    double chunkSize;
    std::vector<unsigned int> particlesIndex;

    public:
    UniverseChunk(double chunkSize) {
        this->chunkSize = chunkSize;
        this->particlesIndex = std::vector<unsigned int>();
    }

    public:
    void addParticle(unsigned int particle) {
        this->particlesIndex.push_back(particle);
    }

    inline auto getParticleBegin() {
        return this->particlesIndex.begin();
    }

    inline auto getParticleEnd() {
        return this->particlesIndex.end();
    }

};