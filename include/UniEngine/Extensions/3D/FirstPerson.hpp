#ifndef FIRSTPERSON_HPP
#define FIRSTPERSON_HPP
#if !defined(__cplusplus)
#error This library needs C++
#endif
#include <UniEngine/UniEngine.hpp>

namespace FPS {
  inline float lastX = 300;
  inline float lastY = 300;
  inline float speed = 0.05f;
  inline float sensivity = 0.1f;
  inline float pitch = 0.0f;
  inline float yaw = -90.0f;

  inline void __callback([[maybe_unused]] GLFWwindow *window, double xpos, double ypos) noexcept {
    Game::cameraData3D &cam_data = Game::getMainCamera();

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;
    xoffset *= sensivity;
    yoffset *= sensivity;

    yaw += xoffset;
    pitch += yoffset;
    if (pitch > 89.0f)
      pitch = 89.0f;
    if (pitch < -89.0f)
      pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cam_data.cameraFront = glm::normalize(front);
}

  inline void activate(Game::Window window) {
    glfwSetCursorPosCallback(window, __callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  }

  inline void getWASD(Game::Window window) {
    Game::cameraData3D &cam_data = Game::getMainCamera();

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
      cam_data.cameraPos += speed * cam_data.cameraFront;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
      cam_data.cameraPos -= speed * cam_data.cameraFront;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
      cam_data.cameraPos -=
          glm::normalize(glm::cross(cam_data.cameraFront, cam_data.cameraUp)) *
          speed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
      cam_data.cameraPos +=
          glm::normalize(glm::cross(cam_data.cameraFront, cam_data.cameraUp)) *
          speed;
    }
  }
} // namespace FPS

#endif
