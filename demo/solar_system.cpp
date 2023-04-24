#include "../src/world/universe.hpp"
#include "../src/world/interactions/gravity.hpp"
#include "../src/visualizer/sdl/sdl_visulizer.hpp"


int main() {
    // create a universe, a visualizer, and register it

    typedef Universe<2, 4, GravityInteractor<2>> MyUniverse;
    std::array<Particle<2>, 4> arrayParticles;

    //Sun
    double sunPos[2];
    sunPos[0] = 0;
    sunPos[1] = 0;
    Vector<double, 2> sunPosVector = Vector<double, 2>(sunPos);
    double sunVel[2];
    sunVel[0] = 0;
    sunVel[1] = 0;
    Vector<double, 2> sunVelVector = Vector<double, 2>(sunVel);
    Vector<double, 2> sunForceVector = Vector<double, 2>();
    double sunMass = 1;

    //Earth
    double earthPos[2];
    earthPos[0] = 0;
    earthPos[1] = 1;
    Vector<double, 2> earthPosVector = Vector<double, 2>(earthPos);
    double earthVel[2];
    sunVel[0] = -1;
    sunVel[1] = 0;
    Vector<double, 2> earthVelVector = Vector<double, 2>(earthVel);
    Vector<double, 2> earthForceVector = Vector<double, 2>();
    double earthMass = 0.000003;

    //Jupiter
    double jupiterPos[2];
    jupiterPos[0] = 0;
    jupiterPos[1] = 5.36;
    Vector<double, 2> jupiterPosVector = Vector<double, 2>(jupiterPos);
    double jupiterVel[2];
    jupiterVel[0] = -0.425;
    jupiterVel[1] = 0;
    Vector<double, 2> jupiterVelVector = Vector<double, 2>(jupiterVel);
    Vector<double, 2> jupiterForceVector = Vector<double, 2>();
    double jupiterMass = 0.000955;

    //Haley
    double haleyPos[2];
    haleyPos[0] = 34.75;
    haleyPos[1] = 0;
    Vector<double, 2> haleyPosVector = Vector<double, 2>(haleyPos);
    double haleyVel[2];
    haleyVel[0] = 0;
    haleyVel[1] = 0.0296;
    Vector<double, 2> haleyVelVector = Vector<double, 2>(haleyVel);
    Vector<double, 2> haleyForceVector = Vector<double, 2>();
    double haleyMass = 0.00000000000001;

    arrayParticles[0] = Particle<2>(sunPosVector, sunVelVector, sunForceVector, sunMass);
    arrayParticles[1] = Particle<2>(earthPosVector, earthVelVector, earthForceVector, earthMass);
    arrayParticles[2] = Particle<2>(jupiterPosVector, jupiterVelVector, jupiterForceVector, jupiterMass);
    arrayParticles[3] = Particle<2>(haleyPosVector, haleyVelVector, haleyForceVector, haleyMass);
    MyUniverse universe(1.0, 0.2, arrayParticles);

    // interactor
    GravityInteractor<2> interactor = GravityInteractor<2>();
    universe.registerInteractor(&interactor);

    // visulizer
    SDLVisulizer<MyUniverse> visulizer = SDLVisulizer<MyUniverse>();
    universe.registerVisulizer(&visulizer);

    // main simulation loop
    for(unsigned int i = 0; i < 500; i++) {
        universe.step(0.0000001);
    }

//    Universe<2, 3, GravityInteractor<2>> universe(GravityInteractor<2>(), 1.0, 0.5);
//    SDLVisulizer<Universe<2, 3, GravityInteractor<2>>> visulizer = SDLVisulizer<Universe<2, 3, GravityInteractor<2>>>();
//    universe.registerVisulizer(&visulizer);
//
//    for(unsigned int i = 0; i < 50000; i++) {
//        universe.step(0.000001);
//    }

}
