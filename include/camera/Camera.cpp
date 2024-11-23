#include "Camera.h"

Camera::Camera()
{
    // camera modes related
    camera_attributes.aspect = 1.5f;
    camera_attributes.vfov = glm::radians(60.0f);
    camera_attributes.near = 0.1f;
    camera_attributes.far = 1000.0f;

    // camera status
    movements.up = false;
    movements.down = false;
    movements.left = false;
    movements.right = false;
    movements.forward = false;
    movements.backward = false;

    postures.yaw_left = false;
    postures.yaw_right = false;
    postures.pitch_up = false;
    postures.pitch_down = false;

    // camera settings
    sensitivity.kb_forward = 0.15f;
    sensitivity.kb_rightward = 0.1f;
    sensitivity.kb_upward = 0.08f;
    sensitivity.kb_yaw = 0.5f;
    sensitivity.kb_pitch = 0.25f;
    sensitivity.mouse_yaw = 0.1f;
    sensitivity.mouse_pitch = 0.1f;

    unit_angle = 1.f;
    unit_sensitivity = 0.001f;

    roll = 0.f;

    update_camera_vectors_and_angles_from_target(glm::vec3(0.0f, 0.0f, 4.0f), glm::vec3(0.0f, 0.0f, 0.0f));
}

Camera::~Camera()
{
}

void Camera::reset_camera_control_status()
{
    movements.up = false;
    movements.down = false;
    movements.left = false;
    movements.right = false;
    movements.forward = false;
    movements.backward = false;

    postures.yaw_left = false;
    postures.yaw_right = false;
    postures.pitch_up = false;
    postures.pitch_down = false;
}

void Camera::update_camera_vectors_and_angles_from_target(const glm::vec3 &new_camera_position, const glm::vec3 &new_target_position)
{
    target_position = new_target_position;
    position = new_camera_position;

    up = glm::vec3{0.0f, 1.0f, 0.0f};
    front = glm::normalize(target_position - position);
    right = glm::normalize(glm::cross(up, front));

    update_camera_eular_angles_from_vectors();
}

void Camera::update_camera_eular_angles_from_vectors()
{
    yaw = glm::degrees(glm::atan(front.x, front.z));
    pitch = glm::degrees(glm::asin(-front.y));
}

void Camera::update_camera_vectors_from_eular_angles()
{
    /* cr. https://learnopengl.com/Getting-started/Camera based on OpenGL coordinates (+Y up, -Z forward, +X right) */

    if (pitch > 89.f)
        pitch = 89.f;
    if (pitch < -89.f)
        pitch = -89.f;

    if (yaw > 180.f)
        yaw -= 360.f;
    if (yaw < -180.f)
        yaw += 360.f;

    const float yawRad = glm::radians(yaw);
    const float pitchRad = glm::radians(pitch);

    const float sy = sin(yawRad);
    const float cy = cos(yawRad);
    const float sp = sin(pitchRad);
    const float cp = cos(pitchRad);

    front.x = cy * cp;
    front.y = sp;
    front.z = sy * cp;

    front = glm::normalize(front);

    // up = glm::vec3{0.0f, 1.0f, 0.0f}; // up remains upward
    right = glm::normalize(glm::cross(front, up));
}

glm::mat4 Camera::get_view_matrix()
{
    return glm::lookAt(position,            // eye
                       position + front,    // target
                       up);                 // up
}

glm::mat4 Camera::get_perspective_matrix()
{
    return glm::perspective(
               camera_attributes.vfov,
               camera_attributes.aspect,
               camera_attributes.near,
               camera_attributes.far) *
           get_view_matrix();
}
