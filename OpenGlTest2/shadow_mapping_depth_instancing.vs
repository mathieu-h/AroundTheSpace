#version 330 core

layout (location = 0) in vec3 aPositionVertex;
layout (location = 3) in mat4 instanceMatrix;

uniform mat4 lightSpaceMatrix;
uniform mat4 modelMatrix;

void main()
{
    gl_Position = lightSpaceMatrix * modelMatrix * instanceMatrix * vec4(aPositionVertex, 1.0f);
}  