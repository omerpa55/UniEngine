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

//C için derlemeyi engelliyoruz

#if !defined (__cplusplus)
    #error This Library is for C++
#endif

#include <vector>

//Eğer DEBUG tanımlıysa gerekli kütüphaneleri al
#if defined (UNI_ENGINE_DEBUG)
    #include <string>
    #include <type_traits>
#endif

//Entity için türetilmeyen sınıf oluşturuyoruz
namespace Entity {
    //Entitylere tipler tanımlıyoruz
    enum EntityType2D {
        PLAYER_2D = 0,
        MESH_2D = 1
    };

    enum EntityType3D {
        PLAYER_3D = 0,
        MESH_3D = 1
    };

    //Entityleri tanımlıyoruz
    class Entity2D {
    private:
        int id;
        EntityType2D type;
        Entity2D* parent = nullptr;
        int childCount = 0;
        std::vector<Entity2D*> children;
    public:
        inline int getID() const {
            return id;
        }
        
        inline int getChildCount() const {
            return childCount;
        }
        
        inline EntityType2D getType() const {
            return type;
        }

        inline void addChild(Entity2D *child) {
            if (child) {
                child->parent = this;
                children.push_back(child);
            }
        }

        inline bool isRoot() const {
            return parent == nullptr;
        }

        inline const std::vector<Entity2D*>& getChildren() const {
            return children;
        }

        Entity2D(int id, EntityType2D type) : id(id), type(type) {};
    };

    //Aynı şekilde 3 boyutlu bir Entity sınıfı da oluşturuyoruz
    class Entity3D {
    private:
        int id;
        EntityType3D type;
        Entity3D* parent = nullptr;
        int childCount = 0;
        std::vector<Entity3D*> children;
    public:
        inline int getId() const {
            return id;
        }
        
        inline EntityType3D getType() const {
            return type;
        }

        inline bool isRoot() const {
            return parent == nullptr;
        }

        inline int getChildCount() const {
            return childCount;
        }

        inline void addChild(Entity3D* child) {
            if (child) {
                child->parent = this;
                children.push_back(child);
            }
        }

        inline const std::vector<Entity3D*>& getChildren() const {
            return children;
        }

        Entity3D(int id, EntityType3D type) : id(id), type(type) {};
    };
}

//Locationlar için türetilmeyen sınıf oluşturuyoruz
namespace Loc {
    //2D bir lokasyon tipi oluşturuyoruz (vector<float>())
    //Hız önceliğimiz olacağı için düşük seviyeli olan struct tipini kullanıyoruz
    struct Loc2D {
        float X;
        float Y;
        //Eğer DEBUG tanımlanmışsa log için isim oluşturuyoruz
        #if defined (UNI_ENGINE_DEBUG)
            std::string name;
        #endif
    };

    //3D bir lokasyon tipi oluşturuyoruz
    struct Loc3D {
        float X;
        float Y;
        float Z;
        //Aynı şekilde buraya da yapıyoruz
        #if defined (UNI_ENGINE_DEBUG)
            std::string name;
        #endif
    };

    //Lokasyonlar için oluşturucu fonksiyonlar yazıyoruz
    inline Loc2D createLoc2D(float X, float Y) {
        return Loc2D { X, Y };
    }

    inline Loc3D createLoc3D(float X, float Y, float Z) {
        return Loc3D { X, Y, Z };
    }

    inline Loc2D createEmptyLoc2D() {
        return Loc2D {};
    }

    inline Loc3D createEmptyLoc3D() {
        return Loc3D {};
    }

    //Eğer DEBUG tanımlıysa log sistemlerini açıyoruz
    #if defined (UNI_ENGINE_DEBUG)

    //Loc2D için log
    inline void logLocL(Loc2D loc) {
        std::cout << "[INFO] " << loc.name << " X: " << loc.X << " Y: " << loc.Y << std::endl;
    }

    //Loc3D için log
    inline void logLocL(Loc3D loc) {
        std::cout << "[INFO] " << loc.name << " X: " << loc.X << " Y: " << loc.Y << " Z: " << loc.Z << std::endl;
    }

    //Akıllı seçim yapan fonksiyon
    template <typename T, 
        typename = std::enable_if_t<std::is_same<T, Loc2D>::value || 
        std::is_same<T, Loc3D>::value>>
    inline void logLoc(T loc) {
        logLocL(loc);
    }

    #endif


}

#endif