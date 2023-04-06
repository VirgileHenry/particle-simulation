#include <iterator>

template<typename Iterator>
class Visulizer {
    public:
    /// @brief Initialize the visualizer.
    virtual void init() = 0;
    /// @brief Show all the particles on the screen.
    virtual void show(Iterator begin, Iterator end) const = 0;
    /// @brief Shut down the visualizer.
    virtual void close() = 0;
};