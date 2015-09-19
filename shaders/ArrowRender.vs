#version 330 core

in vec3 aPositionVertex;
in vec3 aPositionNormal;
in vec2 aTextCoords;

out vec2 TexCoords;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

void main()
{
    gl_Position = projectionMatrix * modelMatrix * vec4(aPositionVertex.xy, 0.0, 1.0);
    TexCoords = aTextCoords;
}  