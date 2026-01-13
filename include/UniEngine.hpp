#ifndef UNIENGINE_HPP
#define UNIENGINE_HPP

enum EntityType2D {
    PLAYER_2D = 0,
    MESH_2D = 0
};

enum EntityType3D {
    PLAYER_3D = 0,
    MESH_3D = 0
};

typedef struct {
    double X;
    double Y;
} Loc2D;

typedef struct {
    double X;
    double Y;
    double Z;
} Loc3D;

namespace Loc {
    inline Loc2D createLoc2D(double X, double Y) {
        return Loc2D { X, Y };
    }
    
    inline Loc3D createLoc3D(double X, double Y, double Z) {
        return Loc3D { X, Y, Z };
    }
}

#endif