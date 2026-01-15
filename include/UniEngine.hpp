/*

Copyright 2026 omerpa55

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation 
files (the “Software”), to deal in the Software without restriction, 
including without limitation the rights to use, copy, modify, merge, publish, distribute, 
sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.

*/

#ifndef UNIENGINE_HPP
#define UNIENGINE_HPP

#include <vector>
#if !defined (__cplusplus)
    #error This library is for C++
#endif

namespace Loc {
    struct Loc3D {
        float X;
        float Y;
        float Z;
    };

    inline Loc3D setLoc3D(float X, float Y, float Z) {
        return { X, Y, Z };
    }

    inline Loc3D setLoc3DToEmpty() {
        return setLoc3D(0.0f, 0.0f, 0.0f);
    }
}

namespace Entity {
    enum EntityType3D {
        PLAYER_3D = 0,
        MESH_2D = 1
    };

    struct Entity3D {
        int id;
        EntityType3D type;
        int parentId = -1;
        Loc::Loc3D loc;

        inline bool isRoot() const { return parentId == -1; };
    };
}

namespace Registry {
    using Registry3D = std::vector<Entity::Entity3D>;

    inline Registry3D& getRegistry3D() {
        static Registry3D registry;
        return registry;
    }

    inline Entity::Entity3D& createEntity3D(Entity::EntityType3D type, Loc::Loc3D loc) {
        Registry3D& reg = getRegistry3D();
        int newID = static_cast<int>(reg.size());

        reg.push_back({ newID, type, -1, loc });

        return reg.back();
    }
}

#endif