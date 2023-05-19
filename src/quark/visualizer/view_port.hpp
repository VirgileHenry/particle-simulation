#pragma once

#include "../maths/vector.hpp"

/// @brief A view port shows what part of the universe to show. 
class ViewPort {
    public:
    Vector<double, 2> corner;
    Vector<double, 2> size;
    Vector<unsigned int, 2> dimensions;

    public:
    ViewPort() {
        double size[2] = {1, 1};
        unsigned int dims[2] = {0, 1};
        this->corner = Vector<double, 2>();
        this->size = Vector<double, 2>(size);
        this->dimensions = Vector<unsigned int, 2>(dims);
    }
    ViewPort(Vector<double, 2> corner, Vector<double, 2> size, Vector<unsigned int, 2> dimensions) {
        this->corner = corner;
        this->size = size;
        this->dimensions = dimensions;
    }

};