#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec3 aNormal;

out vec3 FragPos;
out vec3 Normal;
out vec3 VertexColor;

uniform mat4 transform;
uniform mat4 model;

void main()
{
    Normal = aNormal;
    FragPos = aPos;
    VertexColor = aColor;
    gl_Position = transform * model * vec4(aPos, 1.0);
}      