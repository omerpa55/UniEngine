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

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/fwd.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/trigonometric.hpp"
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
#include <glm/glm.hpp>

namespace Loc {
    struct Loc {
        float X;
        float Y;
        float Z;
    };

    inline Loc setLoc(float X, float Y, float Z) {
        return { X, Y, Z };
    }

    inline Loc setLocToEmpty() {
        return setLoc(0.0f, 0.0f, 0.0f);
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
        Loc::Loc loc;

        inline bool isRoot() const { return parentId == -1; };
    };


}

namespace Registry {
    using Registry3D = std::vector<Entity::RegistryEntity3D>;

    inline Registry3D& getRegistry3D() {
        static Registry3D registry;
        return registry;
    }

    inline Entity::RegistryEntity3D& createRegistryEntity3D(Entity::EntityType3D type, Loc::Loc loc) {
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

        inline Loc::Loc getWorldLocation() const {
            Entity::RegistryEntity3D me = Registry::getRegisteryEntity3D(id);
            Entity::RegistryEntity3D myParent = Registry::getRegisteryEntity3D(me.parentId);
            return { me.loc.X + myParent.loc.X, me.loc.Y + myParent.loc.Y, me.loc.Z + myParent.loc.Z };
        }

        inline Loc::Loc getLoc() const {
            return Registry::getRegisteryEntity3D(id).loc;
        }

        inline int getID() const { return id; };
    };
}
namespace Shader {
    inline const char* defaultVertexShader2D = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    uniform vec3 loc;

    void main() {
        gl_Position = vec4(aPos + loc, 1.0f);
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

    struct FrameBuffer {
        int width;
        int height;
    };

    inline FrameBuffer& getBufferSize() {
        static FrameBuffer buffer{};
        return buffer;
    }

    inline Window createWindow(WindowProperties* properties) {
        FrameBuffer& buffer = getBufferSize();
        Window window = glfwCreateWindow(properties->w, properties->h, properties->title.c_str(), nullptr, nullptr);
        glfwGetFramebufferSize(window, &buffer.width, &buffer.height);
        return window;
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
        UniformLoc loc;
    };

    struct uniformLocations3D {
        UniformLoc projection;
        UniformLoc view;
        UniformLoc model;
        UniformLoc solidColor;
        UniformLoc loc;
    };

    struct objectData2D {
        Vbo vbo;
        Vao vao;
        std::size_t vertexCount;
        Color color;
        uniformLocations ulocs;
        Loc::Loc loc;
    };

    struct objectData3D {
        Vbo vbo;
        Vao vao;
        std::size_t vertexCount;
        Color color;
        uniformLocations3D ulocs;
        Loc::Loc loc;
    };

    struct cameraData3D {
        glm::vec3 cameraPos;
        glm::vec3 cameraFront;
        glm::vec3 cameraUp;
    };

    enum objectType {
        TRIANGLES = 0, QUADS = 1
    };

    template<std::size_t N>
    inline objectData2D createObject2D(float (&array)[N], objectType type, Color color, const Loc::Loc& loc) {
        if (type == TRIANGLES) {
            //Rule: TRIANGLES type arrays' data must be float * 3 by a coordinate
            objectData2D data{};

            program& programId = getProgram();

            data.ulocs.solidColor = glGetUniformLocation(programId, "solidColor");
            data.ulocs.loc = glGetUniformLocation(programId, "loc");

            data.vertexCount = N / 3;

            data.color[0] = color[0];
            data.color[1] = color[1];
            data.color[2] = color[2];

            data.loc = loc;
            
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

    inline cameraData3D& getMainCamera() {
        static cameraData3D mainCamera{};
        return mainCamera;
    }

    inline void createCamera3D(Loc::Loc pos, Loc::Loc rot, Loc::Loc up) {
        cameraData3D &data = getMainCamera();

        data.cameraUp.x = up.X;
        data.cameraUp.y = up.Y;
        data.cameraUp.z = up.Z;

        data.cameraFront.x = rot.X;
        data.cameraFront.y = rot.Y;
        data.cameraFront.z = rot.Z;

        data.cameraPos.x = pos.X;
        data.cameraPos.y = pos.Y;
        data.cameraPos.z = pos.Z;
    }

    template<std::size_t N>
    inline objectData3D createObject3D(float (&array)[N], objectType type, Color color, Loc::Loc loc) {
        if (type == TRIANGLES) {
            objectData3D data{};

            program& programId = getProgram();

            data.ulocs.solidColor = glGetUniformLocation(programId, "solidColor");
            data.ulocs.model = glGetUniformLocation(programId, "model");
            data.ulocs.projection = glGetUniformLocation(programId, "projection");
            data.ulocs.view = glGetUniformLocation(programId, "view");

            data.vertexCount = N / 3;

            data.color[0] = color[0];
            data.color[1] = color[1];
            data.color[2] = color[2];

            data.loc = loc;

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
            std::cerr << "[ERR]QUADS is deprecated";
            exit(1);
        }

        return {};
    }

    inline void preloadRate() {
        glUseProgram(getProgram());
    }

    inline void drawObject2D(objectData2D& data, objectType type) {
        glBindVertexArray(data.vao);
        
        glUniform3f(data.ulocs.solidColor, data.color[0], data.color[1], data.color[2]);
        glUniform3f(data.ulocs.loc, data.loc.X, data.loc.Y, data.loc.Z);

        if (type == TRIANGLES) {
            glDrawArrays(GL_TRIANGLES, 0, data.vertexCount);
        }

        if (type == QUADS) {
            std::cerr << "[ERR]QUADS is deprecated";
            exit(1);
        }

        glBindVertexArray(0);
    }

    inline void drawObject3D(objectData3D& data) {
        glBindVertexArray(data.vao);

        glUniform3f(data.ulocs.solidColor, data.color[0], data.color[1], data.color[2]);

        cameraData3D cameradata = getMainCamera();

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(data.loc.X, data.loc.Y, data.loc.Z));

        glm::mat4 view = glm::lookAt(
            cameradata.cameraPos, cameradata.cameraPos + cameradata.cameraFront, cameradata.cameraUp
        );

        FrameBuffer buffer = getBufferSize();

        glm::mat4 projection = glm::perspective(glm::radians(60.0f), (float)buffer.width / (float)buffer.height, 0.1f, 100.0f);

        glUniformMatrix4fv(data.ulocs.projection, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(data.ulocs.view, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(data.ulocs.model, 1, GL_FALSE, glm::value_ptr(model));

        glDrawArrays(GL_TRIANGLES, 0, data.vertexCount);

        glBindVertexArray(0);
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
