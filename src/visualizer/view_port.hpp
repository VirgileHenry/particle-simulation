#pragma once

/// @brief A view port shows what part of the universe to show. 
class ViewPort {
    public:
    // would be more efficients to use our own vectors ?
    double x, y;
    double w, h;
    unsigned int X, Y;

    public:
    ViewPort(double x, double y, double w, double h, unsigned int X, unsigned int Y) {
        this->x = x;
        this->y = y;
        this->w = w;
        this->h = h;
        this->X = X;
        this->Y = Y;
    }

};