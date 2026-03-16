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

#include <cstddef>
#include <cstdlib>
#if !defined (__cplusplus)
    #error This library is for C++
#endif

#include <glad/glad.h>
#include <vector>
#include <string>
#include <GLFW/glfw3.h>
#include <iostream>

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
namespace Shader {
    inline const char* defaultVertexShader2D = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;

    void main() {
        gl_Position = vec4(aPos, 1.0f);
    }
    )";

    inline const char* defaultFragmentShader = R"(
    #version 330 core
    out vec4 fragColor;
    uniform vec3 solidColor;

    void main() {
        fragColor = vec4(solidColor, 1.0f);
    }
    )";
    
    inline const char* defaultVertexShader3D = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    uniform mat4 projection;
    uniform mat4 view;
    uniform mat4 model;

    void main() {
        gl_Position = projection * view * model * vec4(aPos, 1.0f);
    }
    )";
}

namespace Objects {
    inline float plane[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f,

        0.5f, -0.5f, 0.0f,
        0.5f,  0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f
    };
}

namespace Game {
    inline void initGame() {
        glfwInit();
    }

    enum GLstatus {
        GL_CORE = 0, GL_COMPATIBILITY = 1
    };

    enum WindowStatus {
        CLOSING = 0, ACTIVE = 1
    };

    inline void setGlVersion(int minor, int major, GLstatus status) {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);

        if (status == GL_CORE) {
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        }

        if (status == GL_COMPATIBILITY) {
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
        }

        #if defined (__APPLE__)
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        #endif
    }

    struct WindowProperties {
        int w;
        int h;
        std::string title;
    };

    typedef GLFWwindow* Window;

    inline Window createWindow(WindowProperties* properties) {
        return (Window) glfwCreateWindow(properties->w, properties->h, properties->title.c_str(), nullptr, nullptr);
    }

    inline void setDefaultWindow(Window window) {
        glfwMakeContextCurrent(window);
    }

    inline WindowStatus getWindowStatus(Window window) {
        if (glfwWindowShouldClose(window)) {
            return CLOSING;
        } else {
            return ACTIVE;
        }
    }

    inline void makeNewRate(Window window) {
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    inline void closeWindow(Window window) {
        glfwDestroyWindow(window);
    }

    inline void terminateGame() {
        glfwTerminate();
    }

    inline void byeToAll(Window window) {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    inline void getGlFunctions() {
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cerr << "[ERR] Failed to load gl funcs" << std::endl;
            exit(1);
        }
    }

    enum dimensionType {
        THREE_D = 0, TWO_D = 1
    };

    inline dimensionType& getDimension() {
        static dimensionType dimension = TWO_D;
        return dimension;
    }

    inline void enable3D() {
        glEnable(GL_DEPTH_TEST);
        getDimension() = THREE_D;
    }

    using program = unsigned int;
    using shader = unsigned int;

    inline program& getProgram() {
        static program programId = 0;
        if (programId == 0) {
            programId = glCreateProgram();
        }
        return programId;
    }

    inline void createMainPipeline(const char* vertexcode, const char* fragmentcode) {
        program& programId = Game::getProgram();

        shader vertexSh = glCreateShader(GL_VERTEX_SHADER);
        if (vertexcode == nullptr) {
            if (getDimension() == TWO_D) {
                glShaderSource(vertexSh, 1, &Shader::defaultVertexShader2D, NULL);
            }
            if (getDimension() == THREE_D) {
                glShaderSource(vertexSh, 1, &Shader::defaultVertexShader3D, NULL);
            }
        } else {
            glShaderSource(vertexSh, 1, &vertexcode, NULL);
        }
        glCompileShader(vertexSh);

        shader fragmentSh = glCreateShader(GL_FRAGMENT_SHADER);
        if (fragmentcode == nullptr) {
            glShaderSource(fragmentSh, 1, &Shader::defaultFragmentShader, NULL);
        } else {
            glShaderSource(fragmentSh, 1, &fragmentcode, NULL);
        }
        glCompileShader(fragmentSh);

        glAttachShader(programId, vertexSh);
        glAttachShader(programId, fragmentSh);

        glLinkProgram(programId);

        glDeleteShader(vertexSh);
        glDeleteShader(fragmentSh);
    }

    using Vbo = unsigned int;
    using Vao = unsigned int;
    using Color = float[3];
    using UniformLoc = int;

    struct uniformLocations {
        UniformLoc solidColor;
    };

    struct uniformLocationsCam {
        UniformLoc projection;
        UniformLoc view;
        UniformLoc model;
    };

    struct objectData {
        Vbo vbo;
        Vao vao;
        std::size_t vertexCount;
        Color color;
        uniformLocations ulocs;
    };

    enum objectType {
        TRIANGLES = 0, QUADS = 1
    };

    template<std::size_t N>
    inline objectData createObject(float (&array)[N], objectType type, Color color) {
        if (type == TRIANGLES) {
            //Rule: TRIANGLES type arrays' data must be float * 3 by a coordinate
            objectData data{};

            program& programId = getProgram();

            data.ulocs.solidColor = glGetUniformLocation(programId, "solidColor");

            data.vertexCount = N / 3;

            data.color[0] = color[0];
            data.color[1] = color[1];
            data.color[2] = color[2];
            
            glGenBuffers(1, &data.vbo);
            glGenVertexArrays(1, &data.vao);

            glBindVertexArray(data.vao);
            glBindBuffer(GL_ARRAY_BUFFER, data.vbo);

            glBufferData(GL_ARRAY_BUFFER, sizeof(array), array, GL_STATIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, NULL);
            glEnableVertexAttribArray(0);

            glBindVertexArray(0);

            return data;
        }

        if (type == QUADS) {
            std::cerr << "[ERR] QUADS is deprecated";
            exit(1);
        }

        return {};
    }

    inline void preloadRate() {
        glUseProgram(getProgram());
    }

    inline void drawObject(objectData& data, objectType type) {
        glBindVertexArray(data.vao);
        
        glUniform3f(data.ulocs.solidColor, data.color[0], data.color[1], data.color[2]);

        if (type == TRIANGLES) {
            glDrawArrays(GL_TRIANGLES, 0, data.vertexCount);
        }

        if (type == QUADS) {
            std::cerr << "[ERR]QUADS is deprecated";
            exit(1);
        }
    }

    inline Game::Window createWindowAndMakeReady(int w, int h, std::string title) {
        Game::setGlVersion(3, 3, GL_CORE);

        Game::WindowProperties properties = { w, h, title };

        Game::Window window = Game::createWindow(&properties);

        Game::setDefaultWindow(window);

        Game::getGlFunctions();

        return window;
    }

    inline void createColorRGB(Color color, int r, int g, int b) {
        color[0] = r / 255.0f;
        color[1] = g / 255.0f;
        color[2] = b / 255.0f;
    }
}

#endif
