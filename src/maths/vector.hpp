#pragma once

#include <iostream>
#include <typeinfo>
#include <functional>
#include <algorithm>
#include <array>

/// @brief Vector of any dimensions, from any types.
/// @tparam T the type of the vector. should implement addition and multiplication, and have a default value.
/// @tparam D the dimension of the vector.
template<typename T, unsigned int D>
class Vector {
    private:
    T values[D];

    public:
    /// @brief Creates a new vector with the default values of T.
    Vector() = default;
    /// @brief Creates a new vector with the given values.
    /// @param values the value to construct the vector with.
    Vector(T values [D])  {
        for(unsigned int i = 0; i < D; i++) {
            this->values[i] = values[i];
        }
    }
    /// @brief Creates a new vector with the given generator function.
    /// @param generator the function to evaluate for each element of the vector.
    Vector(std::function<T()> generator) {
        for(unsigned int i = 0; i < D; i++) {
            this->values[i] = generator();
        }
    }
    
    // intern operators
    public:
    inline T& operator[](unsigned int i) {
        return this->values[i];
    }

    public:
    inline T operator[](unsigned int i) const {
        return this->values[i];
    }

    void operator+=(const Vector<T, D>& v) {
        for(unsigned int i = 0; i < D-1; i++) {
            this->values[i] += v[i];
        }
    }

    void operator-=(const Vector<T, D>& v) {
        for(unsigned int i = 0; i < D-1; i++) {
            this->values[i] -= v[i];
        }
    }

    void operator*=(const T& s) {
        for(unsigned int i = 0; i < D-1; i++) {
            this->values[i] *= s;
        }
    }

    void operator/=(const T& s) {
        for(unsigned int i = 0; i < D-1; i++) {
            this->values[i] /= s;
        }
    }

    bool operator==(const Vector<T, D>& v2) {
        for(unsigned int i = 0; i < D; i++) {
            if(this->values[i] != v2[i]) {
                return false;
            }
        }
        return true;
    }

    // intern use functions

    /// @brief Returns the squared magnitude of the vector.
    ///         The result is not rooted, as we are not sure sqrt is defined for T.
    /// @return The sum of the squarred elements of values.
    T sq_magnitude() {
        T sum;
        for(auto elem: this->values) {
            sum += elem * elem;
        }
        return sum;
    }

    /// @brief reset the vector in place, sets all its value to zero
    void reset() {
        // set all values to default
        *this->values = {0};
    }

};

/// @brief Flux operator
/// @tparam T type of the vector
/// @tparam D size of the vector
/// @param os stream to print to
/// @param vec the vector to display
/// @return the stram, with the vector displayed in it
template<typename T, unsigned int D>
std::ostream& operator<<(std::ostream& os, const Vector<T, D>& vec) {
    os << "Vector<" << typeid(T).name() << "," << D << ">[";
    for(unsigned int i = 0; i < D-1; i++) {
        os << vec[i] << ",";
    }
    os << vec[D-1] << "]";
    return os;
}

/// @brief unary minus operator
/// @tparam T type of the vector
/// @tparam D size of the vector
/// @param v vector to oppose
/// @return -v
template<typename T, unsigned int D>
Vector<T, D> operator-(const Vector<T, D>& v) {
    T values [D];
    unsigned int i = 0;
    std::generate_n(values, D, [&] { return -v[i++]; });
    return Vector<T, D>(values);
}

/// @brief Addition operator
/// @tparam T type of the vector
/// @tparam D size of the vector
/// @param v1 first vector to add
/// @param v2 second vector to add
/// @return the sum of v1 and v2
template<typename T, unsigned int D>
Vector<T, D> operator+(const Vector<T, D>& v1, const Vector<T, D>& v2) {
    T values [D];
    unsigned int i = 0;
    std::generate_n(values, D, [&] { return v1[i++] + v2[i++]; });
    return Vector<T, D>(values);
}

/// @brief Substraction operator
/// @tparam T type of the vector
/// @tparam D size of the vector
/// @param v1 vector to substract from
/// @param v2 vector to substract
/// @return the difference between v1 and v2
template<typename T, unsigned int D>
Vector<T, D> operator-(const Vector<T, D>& v1, const Vector<T, D>& v2) {
    T values [D];
    unsigned int i = 0;
    std::generate_n(values, D, [&] { return v1[i++] - v2[i++]; });
    return Vector<T, D>(values);
}

/// @brief scalar multiplication operator
/// @tparam T type of the vector and the scalar
/// @tparam D size of the vector
/// @param v vector to mulitply
/// @param s the scalar to multiply by
/// @return the vector multiplied by the scalar
template<typename T, unsigned int D>
Vector<T, D> operator*(const Vector<T, D>& v, const T& s) {
    T values [D];
    unsigned int i = 0;
    std::generate_n(values, D, [&] { return v[i++] * s; });
    return Vector<T, D>(values);
}

/// @brief scalar division operator
/// @tparam T type of the vector and the scalar
/// @tparam D size of the vector
/// @param v vector to mulitply
/// @param s the scalar to divide by
/// @return the vector multiplied by the scalar
template<typename T, unsigned int D>
Vector<T, D> operator/(const Vector<T, D>& v, const T& s) {
    T values [D];
    unsigned int i = 0;
    std::generate_n(values, D, [&] { return v[i++] / s; });
    return Vector<T, D>(values);
}

/// @brief Concatenation operator. Append elements of v2 to v1, creating a new vector of size D + M
/// @tparam T the type of the vector
/// @tparam D the size of the first vector
/// @tparam M the size of the second vector
/// @param v1 the first vector
/// @param v2 the second vector
/// @return first and second vectors concatenated
template<typename T, unsigned int D, unsigned int M>
Vector<T, D + M> operator<<(const Vector<T, D>& v1, const Vector<T, M>& v2) {
    T values [D + M];
    unsigned int i = 0;
    std::generate_n(values, D, [&] { return v1[i++]; });
    std::generate_n(values, M, [&] { return v2[i++]; });
    return Vector<T, D + M>(values);
}

/// @brief Equality check operator
/// @tparam T type of the vector
/// @tparam D size of the vector
/// @param v1 vector to check from
/// @param v2 vector to check
/// @return if v1 == v2
template<typename T, unsigned int D>
bool operator==(const Vector<T, D>& v1, const Vector<T, D>& v2) {
    for(unsigned int i = 0; i < D; i++) {
        if(v1[i] != v2[i]) {
            return false;
        }
    }
    return true;
}

/// @brief Creates a hash function for the vector.
///         Got from https://stackoverflow.com/questions/17016175/c-unordered-map-using-a-custom-class-type-as-the-key.
namespace std {

    template<typename T, unsigned int D>
    struct hash<Vector<T, D>>
    {
        std::size_t operator()(const Vector<T, D>& v) const
        {
            using std::size_t;
            using std::hash;

            // Compute individual hash values for first,
            // second and third and combine them using XOR
            // and bit shifting:
            if(D == 0) {
                return 0;
            }

            std::size_t result = std::hash<T>()(v[0]);
            for(unsigned int i = 1; i < D; i++) {
                result ^= std::hash<T>()(v[i]);
            }

            return result;
        }
    };
}   

