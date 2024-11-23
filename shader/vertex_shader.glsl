#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
  
out vec3 ourColor;

uniform mat4 transform;

void main()
{

    // [WORK]
    // // Construct an identity matrix
    // mat4 identity = mat4(1.0);

    // // Translation matrix: Move object backward in Z-axis by -2.0
    // mat4 translation = mat4(
    //     1.0, 0.0, 0.0, 0.0,
    //     0.0, 1.0, 0.0, 0.0,
    //     0.0, 0.0, 1.0, -2.0,
    //     0.0, 0.0, 0.0, 1.0
    // );

    // // Rotation matrix: Rotate 45 degrees around Y-axis
    // float angle = radians(45.0);
    // float c = cos(angle);
    // float s = sin(angle);
    // mat4 rotation = mat4(
    //     c,    0.0,  s,    0.0,
    //     0.0,  1.0,  0.0,  0.0,
    //     -s,   0.0,  c,    0.0,
    //     0.0,  0.0,  0.0,  1.0
    // );

    // // Scaling matrix: Scale uniformly by 2.0
    // mat4 scaling = mat4(
    //     2.0, 0.0, 0.0, 0.0,
    //     0.0, 2.0, 0.0, 0.0,
    //     0.0, 0.0, 2.0, 0.0,
    //     0.0, 0.0, 0.0, 1.0
    // );

    // // Combine transformations: scale -> rotate -> translate
    // mat4 model = translation * rotation * scaling;

    // // Apply the model transformation to the vertex position
    // gl_Position = model * vec4(aPos, 1.0);

    // [ORG]
    gl_Position = transform * vec4(aPos, 1.0);
    ourColor = aColor;
}      