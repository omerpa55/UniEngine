#ifndef UNIENGINE_HPP
#define UNIENGINE_HPP

namespace Entity {
    enum EntityType2D {
        PLAYER_2D = 0,
        MESH_2D = 1
    };

    enum EntityType3D {
        PLAYER_3D = 0,
        MESH_3D = 1
    };
}

namespace Loc {
    struct Loc2D {
        float X;
        float Y;
    };
    
    struct Loc3D {
        float X;
        float Y;
        float Z;
    };
}

#endif