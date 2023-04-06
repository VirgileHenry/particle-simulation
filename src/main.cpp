#include "world/universe.hpp"
#include "world/interactions/no_interactions.hpp"

int main() {
    Universe<3, 5000, NoInteractions<3>> universe(NoInteractions<3>(), 1.0, 0.5);
    for(int i = 0; i < 100; i++) {
        universe.step(0.01);
    }
}