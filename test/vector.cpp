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

    // test the magnitude
    assert(v_default.sq_magnitude() == 0);

    // test operator
    double first[3] = {1, 2, 3};
    Vector<double, 3> v_first = Vector<double, 3>(first);
    double second[3] = {4, 5, 6};
    Vector<double, 3> v_second = Vector<double, 3>(second);
    assert((-v_second)[0] == -4);
    assert((-v_second)[1] == -5);
    assert((-v_second)[2] == -6);

    assert((v_first+v_second)[0] == 5);
    assert((v_first+v_second)[1] == 7);
    assert((v_first+v_second)[2] == 9);

    assert((v_first-v_second)[0] == -3);
    assert((v_first-v_second)[1] == -3);
    assert((v_first-v_second)[2] == -3);

    assert((v_first*2.0)[0] == 2);
    assert((v_first*2.0)[1] == 4);
    assert((v_first*2.0)[2] == 6);

    assert((v_first/2.0)[0] == 0.5);
    assert((v_first/2.0)[1] == 1);
    assert((v_first/2.0)[2] == 1.5);

    v_first-=v_second;
    assert((v_first)[0] == -3);
    assert((v_first)[1] == -3);
    assert((v_first)[2] == -3);

    v_first+=v_second;
    assert((v_first)[0] == 1);
    assert((v_first)[1] == 2);
    assert((v_first)[2] == 3);

    v_first*=2;
    assert((v_first)[0] == 2);
    assert((v_first)[1] == 4);
    assert((v_first)[2] == 6);

    v_first/=4;
    assert((v_first)[0] == 0.5);
    assert((v_first)[1] == 1);
    assert((v_first)[2] == 1.5);


    v_first.reset();
    assert(v_first == v_zero);
}