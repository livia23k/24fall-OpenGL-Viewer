#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdint.h>
#include <string>
#include <cassert>

struct Camera
{
    Camera();
    ~Camera();
    Camera(const Camera &) = delete;

    struct Camera_Attributes {
        float aspect;
        float vfov;
        float near;
        float far;
    } camera_attributes;

    // =============================================
    // USER mode related variables 

    /* cr. movement related handle learned from CMU 15666 Computer Game Programming code base
           https://github.com/15-466/15-466-f24-base2/blob/b7584e87b2498e4491e6438770f4b4a8d593bbde/PlayMode.cpp#L70 */

    struct Camera_Movement {
        bool left;
        bool right;
        bool up;
        bool down;
        bool forward;
        bool backward;
    } movements;

    struct Camera_Posture {
        bool yaw_left;
        bool yaw_right;
        bool pitch_up;
        bool pitch_down;
    } postures;

    /* cr. camera parameters learned from Learn OpenGL 
           https://learnopengl.com/Getting-started/Camera# */

    glm::vec3 position;

    glm::vec3 front;
    glm::vec3 right;
    glm::vec3 world_up;

    float yaw;
    float pitch;

    float unit_angle;
    float unit_sensitivity;

    float move_sensitivity;
    float mouse_sensitivity;

    // =============================================
    // Helper Functions

    void reset_eular_angles();
    void reset_camera_control_status();
    void look_at_model(const glm::vec3 &model_center, float model_radius);
    void update_camera_vectors_from_eular_angles();

    glm::mat4 get_view_matrix();
    glm::mat4 get_perspective_matrix();

    void ProcessMouseMovement(float xoffset, float yoffset);
    void ProcessKeyboard(float deltaTime);
};