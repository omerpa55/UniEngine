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

#if !defined (__cplusplus)
    #error This library is for C++
#endif

#include <vector>
#include <string>
#include <GLFW/glfw3.h>

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

    struct RegistryEntity3D {
        int id;
        EntityType3D type;
        int parentId = -1;
        Loc::Loc3D loc;

        inline bool isRoot() const { return parentId == -1; };
    };


}

namespace Registry {
    using Registry3D = std::vector<Entity::RegistryEntity3D>;

    inline Registry3D& getRegistry3D() {
        static Registry3D registry;
        return registry;
    }

    inline Entity::RegistryEntity3D& createRegistryEntity3D(Entity::EntityType3D type, Loc::Loc3D loc) {
        Registry3D& reg = getRegistry3D();
        int newID = static_cast<int>(reg.size());

        reg.push_back({ newID, type, -1, loc });

        return reg.back();
    }

    inline Entity::RegistryEntity3D& getRegisteryEntity3D(int id) {
        return getRegistry3D()[id];
    }
}

namespace Entity {
    class Entity3D {
    private:
        int id;
    public:
        Entity3D(int id) : id(id) {};

        inline void setLocation(float X, float Y, float Z) {
            Entity::RegistryEntity3D& realEntity = Registry::getRegisteryEntity3D(id);
            realEntity.loc.X = X;
            realEntity.loc.Y = Y;
            realEntity.loc.Z = Z;
        }

        inline void setParent(Entity3D parent) {
            Entity::RegistryEntity3D& me = Registry::getRegisteryEntity3D(id);
            me.parentId = parent.getID();
        }

        inline Loc::Loc3D getWorldLocation() const {
            Entity::RegistryEntity3D me = Registry::getRegisteryEntity3D(id);
            Entity::RegistryEntity3D myParent = Registry::getRegisteryEntity3D(me.parentId);
            return { me.loc.X + myParent.loc.X, me.loc.Y + myParent.loc.Y, me.loc.Z + myParent.loc.Z };
        }

        inline Loc::Loc3D getLoc() const {
            return Registry::getRegisteryEntity3D(id).loc;
        }

        inline int getID() const { return id; };
    };
}

namespace Game {
    struct WindowProperties {
        int w;
        int h;
        std::string title;
    };

    typedef GLFWwindow* Window;

    Window createWindow(WindowProperties* properties) {
        return (Window) glfwCreateWindow(properties->w, properties->h, properties->title.c_str(), nullptr, nullptr);
    }
}

#endif
