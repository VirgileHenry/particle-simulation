/// Unit tests for the vector class, and all functionnalities.
#include <cassert>
#include "quark/maths/vector.hpp"

int main() {
    // all vector test

    // test default constructor
    Vector<double, 3> v_default = Vector<double, 3>();
    double zeros[3] = {0, 0, 0};
    Vector<double, 3> v_zero(zeros);
    assert(v_default == v_zero);

    // todo : all vector unit tests
}