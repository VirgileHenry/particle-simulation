#include "world/universe.hpp"
#include "world/interactions/gravity.hpp"
#include "visualizer/sdl/sdl_visulizer.hpp"

int main() {
    // create a universe, a visulizer, and register it
    Universe<2, 3, GravityInteractor<2>> universe(GravityInteractor<2>(), 1.0, 0.5);
    SDLVisulizer<Universe<2, 3, GravityInteractor<2>>> visulizer = SDLVisulizer<Universe<2, 3, GravityInteractor<2>>>();
    universe.registerVisulizer(&visulizer);

    for(unsigned int i = 0; i < 50000; i++) {
        universe.step(0.000001);
    }

}
