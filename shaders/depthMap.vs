#version 330 core

layout (location = 0) in vec3 aPositionVertex;
layout (location = 1) in vec2 aTextCoords;

uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;

out vec2 TexCoords;

void main()
{
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(aPositionVertex, 1.0f);
	TexCoords = aTextCoords;
}  