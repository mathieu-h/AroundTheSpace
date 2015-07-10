#version 330 core
in vec3 aPositionVertex;
in vec2 aTextCoords;
in vec3 aPositionNormal;

out vec2 TexCoord;
out vec3 PositionNormal;	
out vec3 FragPos;

uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform float time;
uniform vec3 viewPos;

uniform vec3 startPos;

uniform vec2 offsets[10];

void main()
{
	float distancePlanets = distance(vec3(0.0f), startPos);
	float camX = sin(time*0.01) * distancePlanets;
	float camZ = cos(time*0.01) * distancePlanets;
			
	vec4 finalPos = projectionMatrix * viewMatrix * vec4(aPositionVertex.x + camX, aPositionVertex.y + startPos.y,aPositionVertex.z+camZ, 1.0f);
	
	//vec2 offset = offsets[gl_InstanceID];
	//vec3 aOffset = aPositionVertex.xyz + offset;
	gl_Position = finalPos;
	TexCoord = vec2(aTextCoords.x, 1.0 - aTextCoords.y);
	PositionNormal = mat3(transpose(inverse(modelMatrix))) * aPositionNormal;
	FragPos = vec3(modelMatrix * vec4(aPositionVertex, 1.0f));
}