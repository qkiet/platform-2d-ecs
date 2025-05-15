#ifndef SIMPLE_2D_ENGINE_ERROR_TYPE_H
#define SIMPLE_2D_ENGINE_ERROR_TYPE_H

namespace simple_2d {
    /**
     * @enum Error
     * @brief Represents error codes used throughout the engine.
     *
     * These error codes indicate the success or failure of various operations.
     */
    enum Error {
        OK, ///< Operation completed successfully.
        INIT, ///< Initialization error.
        LOAD_RESOURCES, ///< Error loading resources.
        NOT_EXISTS, ///< Resource does not exist.
        AUDIO, ///< Audio-related error.
        RENDER, ///< Render-related error.
    };
}; // simple_2d

#endif
