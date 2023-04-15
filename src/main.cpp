#include "world/universe.hpp"
#include "world/interactions/gravity.hpp"
#include "visualizer/sdl/sdl_visulizer.hpp"

int main() {
    // create a universe, an interactor, visulizer, and register it (using type def bc for now, need to pass it to the visu)
    typedef Universe<2, 3000, GravityInteractor<2>> MyUniverse;
    MyUniverse universe(1.0, 0.2);

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

}
