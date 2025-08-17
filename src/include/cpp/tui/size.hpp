// HumbleOS file: size.hpp
// Purpose: Size struct for TUI

#pragma once

namespace tui {
    /// @brief A generic size.
    /// @tparam T The type to use for width and height.
    template <typename T> struct GenericSize {
        T width; // Width
        T height; // Height

        /// @brief Create the size.
        inline GenericSize(T width, T height) : width(width), height(height) {}
    };

    /// @brief A standard size.
    using Size = GenericSize<int>;
}