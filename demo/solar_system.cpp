#include "quark/quark.hpp"

/*
/!\ DISCLAIMER /!\

With the latest impl of the border, the comet will be outside of the visible range (and universe range) and will be discarded.
*/

int main() {
    // create a universe, a visualizer, and register it
    typedef Universe<2, 4, 1.0, 1.0> MyUniverse;

    Particle<2> arrayParticles[4];

    // Sun
    double sunPos[2] {0, 0};
    Vector<double, 2> sunPosVector = Vector<double, 2>(sunPos);
    double sunVel[2] {0, 0};
    Vector<double, 2> sunVelVector = Vector<double, 2>(sunVel);
    Vector<double, 2> sunForceVector = Vector<double, 2>();
    double sunMass = 1;

    // Earth
    double earthPos[2] {0, 1};
    Vector<double, 2> earthPosVector = Vector<double, 2>(earthPos);
    double earthVel[2] {-1, 0};
    Vector<double, 2> earthVelVector = Vector<double, 2>(earthVel);
    Vector<double, 2> earthForceVector = Vector<double, 2>();
    double earthMass = 0.000003;

    // Jupiter
    double jupiterPos[2] {0, 5.36};
    Vector<double, 2> jupiterPosVector = Vector<double, 2>(jupiterPos);
    double jupiterVel[2] {-0.425, 0};
    Vector<double, 2> jupiterVelVector = Vector<double, 2>(jupiterVel);
    Vector<double, 2> jupiterForceVector = Vector<double, 2>();
    double jupiterMass = 0.000955;

    // Haley
    double haleyPos[2] {34.75, 0};
    Vector<double, 2> haleyPosVector = Vector<double, 2>(haleyPos);
    double haleyVel[2] {0, 0.0296};
    Vector<double, 2> haleyVelVector = Vector<double, 2>(haleyVel);
    Vector<double, 2> haleyForceVector = Vector<double, 2>();
    double haleyMass = 0.00000000000001;

    arrayParticles[0] = Particle<2>(sunPosVector, sunVelVector, sunForceVector, sunMass);
    arrayParticles[1] = Particle<2>(earthPosVector, earthVelVector, earthForceVector, earthMass);
    arrayParticles[2] = Particle<2>(jupiterPosVector, jupiterVelVector, jupiterForceVector, jupiterMass);
    arrayParticles[3] = Particle<2>(haleyPosVector, haleyVelVector, haleyForceVector, haleyMass);
    MyUniverse universe(arrayParticles);

    // interactor
    GravityInteractor<2> interactor = GravityInteractor<2>();
    universe.registerInteractor(&interactor);

    // visualizer
    SDLVisualizer<MyUniverse> visualizer = SDLVisualizer<MyUniverse>();
    //XMLVisualizer<MyUniverse> fileVisualizer = XMLVisualizer<MyUniverse>(4, 0, 2, "SolarSystem");
    universe.registerVisualizer(&visualizer);
    //universe.registerVisualizer(&fileVisualizer);
    double corner[2] = {-10, -10};
    visualizer.setViewportCorner(corner);
    double size[2] = {20, 20};
    visualizer.setViewportSize(size);
    unsigned int dim[2] {0, 1};
    visualizer.setViewportDimensions(dim);

    // main simulation loop
    for(unsigned int i = 0; i < 500000; i++) {
        universe.step(0.000001); // simulate hour by hour
    }


}
