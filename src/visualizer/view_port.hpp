#pragma once

#include "../maths/vector.hpp"

/// @brief A view port shows what part of the universe to show. 
class ViewPort {
    public:
    Vector<double, 2> corner;
    Vector<double, 2> size;
    Vector<unsigned int, 2> dimensions;

    public:
    ViewPort(Vector<double, 2> corner, Vector<double, 2> size, Vector<unsigned int, 2> dimensions) {
        this->corner = corner;
        this->size = size;
        this->dimensions = dimensions;
    }

};