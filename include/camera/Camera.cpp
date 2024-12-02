#include "Camera.h"
#include <glad/glad.h>

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
    yaw = -90.0f;
    pitch = 0.0f;

    world_up = glm::vec3{0.0f, 1.0f, 0.0f};

    unit_angle = 1.f;
    unit_sensitivity = 0.001f;

    move_sensitivity = 0.5f;
    mouse_sensitivity = 0.1f;

    this->look_at_model(glm::vec3(0.0f, 0.0f, 0.0f), 1.0f);
}

Camera::~Camera()
{
}

void Camera::reset_eular_angles()
{
    yaw = -90.0f;
    pitch = 0.0f;

    update_camera_vectors_from_eular_angles();
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

void Camera::look_at_model(const glm::vec3 &model_center, float model_radius)
{
    float distance = (model_radius / glm::tan(camera_attributes.vfov * 0.5f)) + model_radius;

    position = model_center + glm::vec3(0.0f, 0.0f, distance);

    front = glm::normalize(model_center - position);
    right = glm::normalize(glm::cross(world_up, front));
}

void Camera::update_camera_vectors_from_eular_angles()
{
    /* cr. https://learnopengl.com/Getting-started/Camera based on OpenGL coordinates (+Y up, -Z forward, +X right) */

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

    right = glm::normalize(glm::cross(front, world_up));
}

glm::mat4 Camera::get_view_matrix()
{
    return glm::lookAt(position,         // eye
                       position + front, // target
                       world_up);        // up
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

// processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void Camera::ProcessMouseMovement(float x_offset, float y_offset)
{
    x_offset *= mouse_sensitivity;
    y_offset *= mouse_sensitivity;

    yaw   += x_offset;
    pitch += y_offset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    update_camera_vectors_from_eular_angles();
}

void Camera::ProcessKeyboard(float deltaTime)
{
    float velocity = move_sensitivity * deltaTime;

    if (movements.forward)
        position += front * velocity;
    if (movements.backward)
        position -= front * velocity;
    if (movements.left)
        position -= right * velocity;
    if (movements.right)
        position += right * velocity;
    if (movements.up)
        position += world_up * velocity;
    if (movements.down)
        position -= world_up * velocity;
}

void Camera::SetTopView(const glm::vec3& target, float distance)
{
    position = target + glm::vec3(0.0f, distance, 0.0f); // Move camera directly above the target
    front = glm::normalize(target - position); // Look at the target
    right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 0.0f, 1.0f))); // Adjust right vector
    world_up = glm::vec3(0.0f, 0.0f, -1.0f); // Set up direction to -Z
}

void Camera::SetFrontView(const glm::vec3& target, float distance)
{
    position = target + glm::vec3(0.0f, 0.0f, distance); // Move camera in front of the target
    front = glm::normalize(target - position); // Look at the target
    right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f))); // Adjust right vector
    world_up = glm::vec3(0.0f, 1.0f, 0.0f); // Set up direction to +Y
}

void Camera::SetSideView(const glm::vec3& target, float distance)
{
    position = target + glm::vec3(distance, 0.0f, 0.0f); // Move camera to the side of the target
    front = glm::normalize(target - position); // Look at the target
    right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f))); // Adjust right vector
    world_up = glm::vec3(0.0f, 1.0f, 0.0f); // Set up direction to +Y
}
