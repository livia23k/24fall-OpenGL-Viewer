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
    yaw = 0.0f;
    pitch = 0.0f;

    world_up = glm::vec3{0.0f, 1.0f, 0.0f};

    unit_angle = 1.f;
    unit_sensitivity = 0.001f;

    angle_sensitivity = 0.15f;
    mouse_sensitivity = 0.1f;

    this->look_at_model(glm::vec3(0.0f, 0.0f, 0.0f), 1.0f);
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
