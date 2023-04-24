#include "../src/quark/quark.hpp"


int main() {
    // create a universe, an interactor, visulizer, and register it (using type def bc for now, need to pass it to the visu)
    typedef Universe<2, 3000, 1.0, 0.2> MyUniverse;
    MyUniverse universe = MyUniverse();

    // interactor
    GravityInteractor<2> interactor = GravityInteractor<2>();
    universe.registerInteractor(&interactor);

    // visulizer
    SDLVisulizer<MyUniverse> visulizer = SDLVisulizer<MyUniverse>();
    universe.registerVisualizer(&visulizer);

    // main simulation loop
    for(unsigned int i = 0; i < 1000; i++) {
        universe.step(0.0000001);
    }


}
