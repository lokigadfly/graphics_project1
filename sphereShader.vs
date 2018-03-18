#version 330
// Input vertex data, different for all executions of this shader.
layout (location = 0) in vec3 vertexPosition_modelspace;

// Output data ; will be interpolated for each fragment.
out vec3 FragPos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    gl_Position = projection * view * model *vec4(vertexPosition_modelspace, 1);
}


