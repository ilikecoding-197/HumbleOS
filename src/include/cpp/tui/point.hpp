// HumbleOS file: point.hpp
// Purpose: Point struct for TUI

#pragma once

namespace tui {
    /// @brief A generic point.
    /// @tparam T The type to use for X and Y.
    template <typename T> struct GenericPoint {
        T x; // X axis
        T y; // Y axis

        /// @brief Create the point.
        inline GenericPoint(T x, T y) : x(x), y(y) {}
    };

    /// @brief A standard point.
    using Point = GenericPoint<int>;
}