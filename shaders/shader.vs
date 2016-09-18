#version 330 core
layout (location = 0) in vec3 position;


out vec3 ourColor;


uniform mat4 transform;

void main()
{
    gl_Position = transform * vec4(position, 1.0f);
    ourColor = vec3(1.0f,0.5f,0.2f);
}
