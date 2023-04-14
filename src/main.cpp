#include "world/universe.hpp"
#include "world/interactions/gravity.hpp"
#include "visualizer/sdl/sdl_visulizer.hpp"

int main() {
    // create a universe, an interactor, visulizer, and register it
    Universe<2, 300, GravityInteractor<2>> universe(1.0, 0.5);

    // interactor
    GravityInteractor<2> interactor = GravityInteractor<2>();
    universe.registerInteractor(&interactor);

    // visulizer
    SDLVisulizer<Universe<2, 300, GravityInteractor<2>>> visulizer = SDLVisulizer<Universe<2, 300, GravityInteractor<2>>>();
    universe.registerVisulizer(&visulizer);

    // main simulation loop
    for(unsigned int i = 0; i < 5000; i++) {
        universe.step(0.0000001);
    }

}
