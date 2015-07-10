#version 330 core

layout (location = 0) in vec3 aPositionVertex;
layout (location = 1) in vec2 aTextCoords;
layout (location = 2) in vec3 aPositionNormal;
layout (location = 3) in mat4 instanceMatrix;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform float time;
uniform vec3 viewPos;

out vec3 PositionNormal;
out vec3 FragPos;
out vec2 TextCoords;

void main()
{
	
	vec4 finalPos = projectionMatrix * viewMatrix * instanceMatrix  * vec4((aPositionVertex), 1.0f);
	gl_Position = vec4(finalPos);
	TextCoords = aTextCoords;
	PositionNormal = mat3(transpose(inverse(instanceMatrix))) * aPositionNormal;
	FragPos = vec3(instanceMatrix * vec4(aPositionVertex, 1.0f));
}