#include "world/universe.hpp"
#include "world/interactions/no_interactions.hpp"
#include "visualizer/sdl/sdl_visulizer.hpp"

int main() {
    // create a universe, a visulizer, and register it
    Universe<2, 5000, NoInteractions<2>> universe(NoInteractions<2>(), 1.0, 0.5);
    SDLVisulizer<Universe<2, 5000, NoInteractions<2>>> visulizer = SDLVisulizer<Universe<2, 5000, NoInteractions<2>>>();
    universe.registerVisulizer(&visulizer);

    for(unsigned int i = 0; i < 100; i++) {
        universe.step(0.01);
    }

}
