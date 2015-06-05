#version 330 core
layout (location = 0) in vec3 aPositionVertex;
layout (location = 1) in vec2 aTextCoords;
layout (location = 2) in vec3 aPositionNormal;
layout (location = 3) in mat4 instanceMatrix;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

out vec2 TexCoord;
out vec3 PositionNormal;
out vec3 FragPos;

uniform vec3 viewPos;

void main()
{
    gl_Position = projectionMatrix * viewMatrix * instanceMatrix * vec4(aPositionVertex, 1.0f); 
	TexCoord = vec2(aTextCoords.x, 1.0 - aTextCoords.y);
	PositionNormal = mat3(transpose(inverse(instanceMatrix))) * aPositionNormal;
	FragPos = vec3(instanceMatrix * vec4(aPositionVertex, 1.0f));
}