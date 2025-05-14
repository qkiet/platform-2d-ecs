#ifndef SIMPLE_2D_ENGINE_ERROR_TYPE_H
#define SIMPLE_2D_ENGINE_ERROR_TYPE_H

namespace simple_2d {
    enum Error {
        OK,
        INIT,
        LOAD_RESOURCES,
            NOT_EXISTS,
            AUDIO,
        };
}; // simple_2d

#endif
