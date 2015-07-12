#version 330 core
in vec3 aPositionVertex;
in vec2 aTextCoords;
in vec3 aPositionNormal;
//in vec3 originPos;
layout (location = 3) in mat4 instanceMatrix;

out vec2 TexCoord;
out vec3 PositionNormal;	
out vec3 FragPos;

uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform float time;
uniform vec3 viewPos;
uniform vec3 originPos;
uniform vec3 starPos;
uniform float planetSpeed;
uniform float timeOffset;

uniform vec3 startPos;

uniform vec2 offsets[10];

void main()
{
	vec4 finalPos = projectionMatrix * viewMatrix * modelMatrix * instanceMatrix * vec4(aPositionVertex.x, aPositionVertex.y,aPositionVertex.z, 1.0f);
	
	//vec2 offset = offsets[gl_InstanceID];
	//vec3 aOffset = aPositionVertex.xyz + offset;
	gl_Position = finalPos;
	TexCoord = vec2(aTextCoords.x,aTextCoords.y);
	PositionNormal = mat3(transpose(inverse(modelMatrix * instanceMatrix))) * aPositionNormal;
	FragPos = vec3(modelMatrix * instanceMatrix * vec4(aPositionVertex, 1.0f));
}