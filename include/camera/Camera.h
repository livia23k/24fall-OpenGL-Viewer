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

    struct Camera_Sensitivity {
        float kb_forward;
        float kb_upward;
        float kb_rightward;
        float kb_yaw;
        float kb_pitch;
        float mouse_yaw;
        float mouse_pitch;
        bool sensitivity_increase;
        bool sensitivity_decrease;
    } sensitivity;

    /* cr. camera parameters learned from Learn OpenGL 
           https://learnopengl.com/Getting-started/Camera# */

    glm::vec3 position;
    glm::vec3 target_position;

    glm::vec3 front;
    glm::vec3 right;
    glm::vec3 up;

    float yaw;
    float pitch;
    float roll;

    float unit_angle;
    float unit_sensitivity;

    // =============================================
    // Helper Functions

    void reset_camera_control_status();
    void update_camera_vectors_and_angles_from_target(const glm::vec3 &new_camera_position, const glm::vec3 &new_target_position);
    void update_camera_eular_angles_from_vectors();
    void update_camera_vectors_from_eular_angles();

    glm::mat4 get_view_matrix();
    glm::mat4 get_perspective_matrix();
    void update_dynamic_camera(float time);

};