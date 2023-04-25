#include "../src/quark/quark.hpp"


int main() {
    // create a universe, an interactor, visulizer, and register it (using type def bc for now, need to pass it to the visu)
    typedef Universe<2, 10000, 1.0, 0.02> MyUniverse;
    MyUniverse universe = MyUniverse();

    // interactor
    GravityInteractor<2> interactor = GravityInteractor<2>();
    universe.registerInteractor(&interactor);

    // visulizer
    SDLVisualizer<MyUniverse> visulizer = SDLVisualizer<MyUniverse>();
    universe.registerVisualizer(&visulizer);

    // main simulation loop
    for(unsigned int i = 0; i < 100; i++) {
        // std::cout << "step " << i << std::endl;
        universe.step(0.0000001);
    }
}
