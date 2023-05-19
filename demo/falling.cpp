#include "../src/quark/quark.hpp"

/*
/!\ DISCLAIMER /!\

Currently, visualizers does not support view port and always draw particles in [0, 1]^D.
Because of this, this simulation is not fully visible, and hardly visible at all.


This compute a particle simulation of a collision of two opjects like so :



            +-----------+
            |           |
            |     +     |
            |     |     |
            |     v     |
            +-----------+

+-----------------------------------+
|                                   |
|                                   |
|                                   |
|                                   |
+-----------------------------------+


*/

int main() {
    // create a universe, a visualizer, and register it
    typedef Universe<2, 8000, 250.0, 2.5> MyUniverse;

    Particle<2> arrayParticles[8000];

    double spacing = 1.12246204831;

    // small cube 
    for(unsigned int i = 0; i < 40; i++) {
        for(unsigned int j = 0; j < 40; j++) {
            double pos[2] {125 - 20 * spacing + i * spacing, 20 + j * spacing};
            double vel[2] {0, 0};
            Vector<double, 2> pos_vec(pos);
            Vector<double, 2> vel_vec(vel);
            Vector<double, 2> force_vec = Vector<double, 2>();
            arrayParticles[i * 40 + j] = Particle<2>(pos_vec, vel_vec, force_vec, 1);
        }
    }

    // large rectangle
    for(unsigned int i = 0; i < 160; i++) {
        for(unsigned int j = 0; j < 40; j++) {
            double pos[2] {125 - 80 * spacing + i * spacing, 100 + j * spacing};
            double vel[2] {0, 0};
            Vector<double, 2> pos_vec(pos);
            Vector<double, 2> vel_vec(vel);
            Vector<double, 2> force_vec = Vector<double, 2>();
            arrayParticles[1600 + i * 40 + j] = Particle<2>(pos_vec, vel_vec, force_vec, 1);
        }
    }

    // create the universe
    MyUniverse universe(arrayParticles);


    // interactor
    GravityInteractor<2> grav_interactor = GravityInteractor<2>();
    universe.registerInteractor(&grav_interactor);
    LennardJonesInteractor<2> lj_interactor = LennardJonesInteractor<2>();
    universe.registerInteractor(&lj_interactor);

    // gravity
    GravityForce<2> grav_force = GravityForce<2>(-12);
    universe.registerForce(&grav_force);

    // visualizer
    SDLVisualizer<MyUniverse> visualizer = SDLVisualizer<MyUniverse>();
    universe.registerVisualizer(&visualizer);

    double size[2] = {250, 150};
    visualizer.setViewportSize(size);
    unsigned int dim[2] {0, 1};
    visualizer.setViewportDimensions(dim);

    // main simulation loop
    for(unsigned int i = 0; i < 400000; i++) {
        universe.step(0.00005); // simulate hour by hour 
    }


}
